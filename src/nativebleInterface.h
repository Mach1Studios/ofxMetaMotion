#include "nativeble/NativeBleController.h"

#include <iostream>
#include <string>
#include <csignal>
#include <thread>

#define SCAN_TIMEOUT_MS 2000

#define NORDIC_UART_SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define NORDIC_UART_CHAR_RX      "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define NORDIC_UART_CHAR_TX      "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

#define METAMOTION_READ_SERVICE_UUID    "0000180a-0000-1000-8000-00805f9b34fb"
#define METAMOTION_READ_UUID            "00002a26-0000-1000-8000-00805f9b34fb"
#define METAMOTION_NOTIFY_SERVICE_UUID  "326a9000-85cb-9195-d9dd-464cfbbae75a"
#define METAMOTION_NOTIFY_UUID          "326a9006-85cb-9195-d9dd-464cfbbae75a"
#define METAMOTION_WRITE_SERVICE_UUID   "326a9000-85cb-9195-d9dd-464cfbbae75a"
#define METAMOTION_WRITE_UUID           "326a9001-85cb-9195-d9dd-464cfbbae75a"

class nativebleInterface {
public:
    NativeBLE::CallbackHolder ble_events;
    NativeBLE::NativeBleController ble;
    std::vector<NativeBLE::DeviceDescriptor> devices;
    bool connected;

    void setup() {
        // Setup callback functions
        ble_events.callback_on_device_disconnected = [](std::string msg) {
            std::cout << "Disconnected: " << msg << std::endl;
            return;
        };
        ble_events.callback_on_device_connected = []() {
            std::cout << "Connected" << std::endl;
        };
        ble_events.callback_on_scan_found = [&](NativeBLE::DeviceDescriptor device) {
            devices.push_back(device);
        };
        ble_events.callback_on_scan_start = []() {
            std::cout << "Scanning for " << SCAN_TIMEOUT_MS << " ms..." << std::endl;
        };
        ble_events.callback_on_scan_stop = []() {
            std::cout << std::endl << "Scan complete." << std::endl;
        };

        ble.setup(ble_events);
        ble.scan_timeout(SCAN_TIMEOUT_MS);
    }
    
    void exit() {
        ble.disconnect();
        ble.dispose();
    }
    
    void rescanDevices(){
        ble.scan_timeout(SCAN_TIMEOUT_MS);
    }
    
    void listDevices(){
        std::cout << devices.size() << " devices found:" << std::endl;
        for (int i = 0; i < devices.size(); i++) {
            std::cout << "  " << i << ": " << devices[i].name << " (" << devices[i].address << ")" << std::endl;
        }
    }
    
    int findMetaMotionDevice(){
        for (int i = 0; i < devices.size(); i++) {
            if (devices[i].name.find("MetaWear") != std::string::npos || devices[i].name.find("Mach1-") != std::string::npos) {
                std::cout << "Auto found MetaMotion: " << devices[i].address << '\n';
                return i;
            }
        }
        return -1;
    }
    
    bool connect(int deviceIndex) {
        ble.connect(devices[deviceIndex].address);
        connected = true;
        return true;
    }
    
    bool disconnect() {
        ble.disconnect();
    }
};
