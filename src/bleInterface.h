#include "simpleble/SimpleBLE.h"

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

class bleInterface {
public:
    std::vector<SimpleBLE::Peripheral> devices;
    SimpleBLE::Adapter ble;
    std::vector<SimpleBLE::Adapter> ble_list;
    bool connected;

    void setup() {
        // Setup callback functions
        ble_list = SimpleBLE::Adapter::get_adapters();
        
        if (ble_list.size() == 0) {
            std::cout << "No adapter was found." << std::endl;
        }

        ble.set_callback_on_scan_start([]() { std::cout << "Scan started." << std::endl; });

        ble.set_callback_on_scan_stop([]() { std::cout << "Scan stopped." << std::endl; });

        ble.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) {
            std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
            devices.push_back(peripheral);
        });

        // Scan for 5 seconds and return.
        ble.scan_for(SCAN_TIMEOUT_MS);

        std::cout << "The following devices were found:" << std::endl;
        for (int i = 0; i < devices.size(); i++) {
            std::cout << "[" << i << "] " << devices[i].identifier() << " [" << devices[i].address() << "]"
                      << std::endl;
        }
    }
    
    void exit() {
        ble.scan_stop();
    }

    void rescanDevices(){
        ble.scan_for(SCAN_TIMEOUT_MS);
    }
    
    void listDevices(){
        std::cout << devices.size() << " devices found:" << std::endl;
        for (int i = 0; i < devices.size(); i++) {
            std::cout << "  " << i << ": " << devices[i].identifier() << " (" << devices[i].address() << ")" << std::endl;
        }
    }
    
    int findMetaMotionDevice(){
        for (int i = 0; i < devices.size(); i++) {
            if (devices[i].identifier().find("MetaWear") != std::string::npos || devices[i].identifier().find("Mach1-M") != std::string::npos) {
                std::cout << "Auto found MetaMotion: " << devices[i].address() << '\n';
                return i;
            }
        }
        return -1;
    }
    
    bool connect(int deviceIndex) {
        devices[deviceIndex].connect();
        connected = true;
        return true;
    }
    
    bool disconnect(int deviceIndex) {
        devices[deviceIndex].connect();
    }
};
