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
    NativeBLE::NativeBleController ble;
    std::vector<NativeBLE::DeviceDescriptor> devices;
    bool connected;

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
