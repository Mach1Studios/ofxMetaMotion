#include "ofApp.h"

#include "metawear/core/metawearboard.h"
#include "metawear/core/module.h"

#include "metawear/core/status.h"
#include "metawear/core/debug.h"
#include "metawear/core/logging.h"
#include "metawear/core/macro.h"

#include "metawear/sensor/accelerometer_bosch.h"
#include "metawear/sensor/accelerometer_mma8452q.h"
#include "metawear/sensor/accelerometer.h"

#include "metawear/peripheral/led.h"

static unordered_map<const void*, MblMwFnIntVoidPtrArray> notify_handlers;

void configure_acc(MblMwMetaWearBoard* board)
{
    // Set ODR to 25Hz or closest valid frequency
    mbl_mw_acc_set_odr(board, 25.f);

    // Set range to +/-4g or closest valid range
    mbl_mw_acc_set_range(board, 4.f);

    // Write the config to the sensor
    mbl_mw_acc_write_acceleration_config(board);
}

void check_acc_type(MblMwMetaWearBoard* board)
{
    auto acc_type = mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_ACCELEROMETER);
    switch (acc_type) {
    case MBL_MW_MODULE_TYPE_NA:
        // should never reach this case statement
        printf("No accelerometer on this board\n");
        break;
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        printf("Acc Type = MMA8452Q\n");
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        printf("Acc Type = BMI160\n");
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        printf("Acc Type = BMA255\n");
        break;
    default:
        printf("Unknown type\n");
        break;
    }
}

static void read_gatt_char(void* context, const void* caller, const MblMwGattChar* characteristic,
    MblMwFnIntVoidPtrArray handler)
{
    // replace with platform specific BluetoothGatt code

    cout << setw(8) << setfill('0') << hex
         << "Reading characteristic: {service_uuid_high: " << characteristic->service_uuid_high
         << ", service_uuid_low: " << characteristic->service_uuid_low
         << ", uuid_high: " << characteristic->uuid_high
         << ", uuid_low: " << characteristic->uuid_low
         << "}" << dec << endl;

    if (characteristic->uuid_high == 0x00002a2400001000 && characteristic->uuid_low == 0x800000805f9b34fb) {
        // simulate response for reading model characteristic
        const uint8_t model[] = "4";
        handler(caller, model, 1);
    }
    else if (characteristic->uuid_high == 0x00002a2600001000 && characteristic->uuid_low == 0x800000805f9b34fb) {
        // simulate response for reading firmware characteristic
        const uint8_t firmware[] = "1.2.5";
        handler(caller, firmware, 5);
    }
}

static void write_gatt_char(void* context, const void* caller, MblMwGattCharWriteType write_type,
    const MblMwGattChar* characteristic, const uint8_t* value, uint8_t length)
{
    // replace with platform specific BluetoothGatt code

    cout << "write type: " << write_type << ", value: " << endl;
    cout << hex << setw(2) << setfill('0') << "[0x" << (int)value[0];
    for (uint8_t i = 1; i < length; i++) {
        cout << ", 0x" << (int)value[i];
    }
    cout << "]}" << dec << endl;
}

static void enable_char_notify(void* context, const void* caller, const MblMwGattChar* characteristic,
    MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready)
{
    // replace with platform specific BluetoothGatt code
    notify_handlers.insert({ caller, handler });
    // call the 'ready' function pointer when the enable notification requeset has finished
    ready(caller, MBL_MW_STATUS_OK);
}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(200);
    threadedmmc.setup();
    threadedmmc.start();

    MblMwBtleConnection btle_conn = { nullptr, write_gatt_char, read_gatt_char, enable_char_notify };
    MblMwMetaWearBoard* board = mbl_mw_metawearboard_create(&btle_conn);

    mbl_mw_metawearboard_initialize(board, nullptr, [](void* context, MblMwMetaWearBoard* board, int32_t status) -> void {
        if (!status) {
            printf("Error initializing board: %d\n", status);
        }
        else {
            printf("The board is initialized \n");
        }
    });

    cout << "model = " << mbl_mw_metawearboard_get_model(board) << endl;
    cout << "model = " << mbl_mw_metawearboard_get_model_name(board) << endl;
    mbl_mw_logging_stop(board);
    mbl_mw_logging_clear_entries(board);
    mbl_mw_macro_erase_all(board);
    mbl_mw_debug_reset_after_gc(board);
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetBackgroundColor(30);

    auto threadFrame = threadedmmc.getThreadFrameNum();
    ofSetColor(255, 0, 0);
    ofDrawBitmapString("app frame: " + ofToString(ofGetFrameNum()), 20, 20);
    ofDrawBitmapString("thread frame: " + ofToString(threadFrame), 20, 35);
    ofDrawBitmapString("diff: " + ofToString(int64_t(ofGetFrameNum()) - threadFrame), 20, 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    //reset orientation
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
