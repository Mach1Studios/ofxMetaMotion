//
//  metamotionController.cpp
//  Created by Mach1 on 01/28/21.
//
//  References can be found at 
//

#include "metamotionController.h"

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
    nativeble.listDevices();
    isConnected = false;
    resetOrientation();
}

void metamotionController::update(){
    if (nativeble.devices.size() < 1) { // keep checking for devices if there are none found
        nativeble.listDevices();
    } else {
        while (!nativeble.connected) { // wait for commandline input prompt of device to connect to
            int device;
            std::cin >> device;
            if (device >= nativeble.devices.size()) {
                std::cout << "Device index out of range." << std::endl;
            }
            nativeble.connect(device);
        }
    }
    
    if(isConnected){ // when connected section
    }
}

void metamotionController::connectDevice(int deviceID) {
    if (nativeble.connect(deviceID)){
        isConnected = true;
    }
}

void metamotionController::disconnectDevice() {
    nativeble.exit();
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
        sprintf(buffer, "0x%02x", data_bytes[i]);
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
    
    auto data_handler = [](void* context, const MblMwData* data) -> void {
        // Cast value to MblMwCartesianFloat*
        auto euler = (MblMwCartesianFloat*) data->value;
        printf("(%.3fg, %.3fg, %.3fg)\n", euler->x, euler->y, euler->z);
    };

    auto fusion_signal = mbl_mw_sensor_fusion_get_data_signal(board, MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE);
    mbl_mw_datasignal_subscribe(fusion_signal, nullptr, data_handler);

    // Start
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
