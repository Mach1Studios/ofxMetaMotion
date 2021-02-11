#include "nativeble/NativeBleController.h"

#include <iostream>
#include <string>
#include <csignal>
#include <thread>

#define SCAN_TIMEOUT_MS 5000

#define NORDIC_UART_SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define NORDIC_UART_CHAR_RX "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define NORDIC_UART_CHAR_TX "6e400003-b5a3-f393-e0a9-e50e24dcca9e"


class nativebleInterface {
public:
    NativeBLE::CallbackHolder                   ble_events;
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
            fflush(stdout);
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
    
    void listDevices(){
        std::cout << devices.size() << " devices found:" << std::endl;
        for (int i = 0; i < devices.size(); i++) {
            std::cout << "  " << i << ": " << devices[i].name << " (" << devices[i].address << ")" << std::endl;
        }
    }
    
    bool connect(int deviceIndex) {
        std::cout << devices.size() << " devices found:" << std::endl;
        for (int i = 0; i < devices.size(); i++) {
            std::cout << "  " << i << ": " << devices[i].name << " (" << devices[i].address << ")" << std::endl;
        }
        if (deviceIndex >= devices.size()) {
            std::cout << "Device index out of range." << std::endl;
            connected = false;
            return false;
        } else {
            ble.connect(devices[deviceIndex].address);
            connected = true;
            return true;
        }
    }
};
