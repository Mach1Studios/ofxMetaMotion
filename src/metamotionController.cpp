//
//  metamotionController.cpp
//  Created by Mach1 on 01/28/21.
//
//  References can be found at 
//

#include "metamotionController.h"
#include "ofMain.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::time_point;

metamotionController::metamotionController() {

}

metamotionController::~metamotionController() {
    nativeble.exit();
}

//----------------------------------------------------- setup.
void metamotionController::setup() {
    //getDeviceIDs();
    nativeble.setup();
    resetOrientation();
}

void metamotionController::update(){
    if (!nativeble.connected){
        if (nativeble.devices.size() < 1) { // if there are no found devices search again
            nativeble.rescanDevices();
        } else if (nativeble.devices.size() > 0){ // if there are found devices
            metaMotionDeviceIndex = nativeble.findMetaMotionDevice(); // store autofound index
            if (metaMotionDeviceIndex == -1){ // but they are not MetaMotion search again
                nativeble.listDevices();
                nativeble.rescanDevices();
            } else if (metaMotionDeviceIndex > -1) { // connect to found device in case the above didnt work
                nativeble.connect(metaMotionDeviceIndex);
                
                // setup meta motion
                MblMwBtleConnection btleConnection;
                btleConnection.context = this;
                btleConnection.write_gatt_char = write_gatt_char;
                btleConnection.read_gatt_char = read_gatt_char;
                btleConnection.enable_notifications = enable_char_notify;
                btleConnection.on_disconnect = on_disconnect;
                board = mbl_mw_metawearboard_create(&btleConnection);
                mbl_mw_metawearboard_initialize(board, this, [](void* context, MblMwMetaWearBoard* board, int32_t status) -> void {
                    auto dev_info = mbl_mw_metawearboard_get_device_information(board);
                    cout << "firmware revision number = " << dev_info->firmware_revision << endl;
                    cout << "model = " << mbl_mw_metawearboard_get_model(board) << endl;
                    auto *wrapper = static_cast<metamotionController *>(context);
                    wrapper->enable_fusion_sampling(wrapper->board);
                    });
            }
        }
    }
    
    if(nativeble.connected){ // when connected section
        // pass device from nativeble to metamotion here
    }
}

void metamotionController::disconnectDevice() {
    nativeble.exit();
    mbl_mw_metawearboard_free(board);
}

void metamotionController::data_printer(void* context, const MblMwData* data) {
    // Print data as 2 digit hex values
    uint8_t* data_bytes = (uint8_t*) data->value;
    string bytes_str("[");
    char buffer[5];
    for (uint8_t i = 0; i < data->length; i++) {
        if (i) {
            bytes_str += ", ";
        }
        //sprintf(buffer, "0x%02x", data_bytes[i]);
        bytes_str += buffer;
    }
    bytes_str += "]";

    // Format time as YYYYMMDD-HH:MM:SS.LLL
    time_point<system_clock> then(milliseconds(data->epoch));
    auto time_c = system_clock::to_time_t(then);
    auto rem_ms= data->epoch % 1000;

    cout << "{timestamp: " << put_time(localtime(&time_c), "%Y%m%d-%T") << "." << rem_ms << ", "
        << "type_id: " << data->type_id << ", "
        << "bytes: " << bytes_str.c_str() << "}"
        << endl;
}

void metamotionController::configure_sensor_fusion(MblMwMetaWearBoard* board) {
    // set fusion mode to ndof (n degress of freedom)
    mbl_mw_sensor_fusion_set_mode(board, MBL_MW_SENSOR_FUSION_MODE_NDOF);
    // set acceleration rangen to +/-8G, note accelerometer is configured here
    mbl_mw_sensor_fusion_set_acc_range(board, MBL_MW_SENSOR_FUSION_ACC_RANGE_8G);
    // write changes to the board
    mbl_mw_sensor_fusion_write_config(board);
}

void metamotionController::enable_fusion_sampling(MblMwMetaWearBoard* board) {
    // Write the config to the sensor
    configure_sensor_fusion(board);
    
    auto fusion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE);
    
    mbl_mw_datasignal_subscribe(fusion_signal, this, [](void* context, const MblMwData* data) -> void {
        auto *wrapper = static_cast<metamotionController *>(context);
        
        auto euler = (MblMwCartesianFloat*)data->value;
        wrapper->outputEuler = (MblMwCartesianFloat*)data->value;
        printf("(%.3f, %.3f, %.3f)\n", euler->x, euler->y, euler->z);
        // x = yaw
        // y = roll
        // z = pitch
    });
    
    // Start
    mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE);
    mbl_mw_sensor_fusion_start(board);
}

void metamotionController::disable_fusion_sampling(MblMwMetaWearBoard* board) {
    auto fusion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE);
    mbl_mw_datasignal_unsubscribe(fusion_signal);
    mbl_mw_sensor_fusion_stop(board);
}

void metamotionController::resetOrientation() {
}

void metamotionController::tare() {
    
}

string HighLow2Uuid(const uint64_t high, const uint64_t low){
    uint8_t *u_h = (uint8_t *)&(high);
    uint8_t *u_l = (uint8_t *)&(low);

    char UUID[38];
    sprintf(UUID, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
            u_h[7], u_h[6], u_h[5], u_h[4], u_h[3], u_h[2], u_h[1], u_h[0],
            u_l[7], u_l[6], u_l[5], u_l[4], u_l[3], u_l[2], u_l[1], u_l[0]
            );
    
    return string(UUID);
}

void metamotionController::read_gatt_char(void *context, const void *caller, const MblMwGattChar *characteristic,
                                          MblMwFnIntVoidPtrArray handler) {
    auto *wrapper = static_cast<metamotionController *>(context);

    wrapper->nativeble.ble.read(HighLow2Uuid(characteristic->service_uuid_high, characteristic->service_uuid_low), HighLow2Uuid(characteristic->uuid_high, characteristic->uuid_low), [&, handler, caller](const uint8_t* data, uint32_t length) {
        handler(caller,data,length);
   });
}


void metamotionController::write_gatt_char(void *context, const void *caller, MblMwGattCharWriteType writeType,
                                          const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length){
    auto *wrapper = static_cast<metamotionController *>(context);
    wrapper->nativeble.ble.write_command(HighLow2Uuid(characteristic->service_uuid_high, characteristic->service_uuid_low), HighLow2Uuid(characteristic->uuid_high, characteristic->uuid_low), std::string((char*)value, int(length)));
}


void metamotionController::enable_char_notify(void *context, const void *caller, const MblMwGattChar *characteristic,
                                             MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready) {
   auto *wrapper = static_cast<metamotionController *>(context);
    wrapper->nativeble.ble.notify(HighLow2Uuid(characteristic->service_uuid_high, characteristic->service_uuid_low), HighLow2Uuid(characteristic->uuid_high, characteristic->uuid_low), [&,handler,caller](const uint8_t* data, uint32_t length) {
        handler(caller,data,length);
    });
    ready(caller, MBL_MW_STATUS_OK);
}

void metamotionController::on_disconnect(void *context, const void *caller, MblMwFnVoidVoidPtrInt handler) {
    auto *wrapper = static_cast<metamotionController *>(context);
    handler(caller, MBL_MW_STATUS_OK);
}
