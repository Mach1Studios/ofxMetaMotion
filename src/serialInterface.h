#include "libusb.h"
#include <iostream>
#include <string>

#define METAMOTION_READ_SERVICE_UUID    "0000180a-0000-1000-8000-00805f9b34fb"
#define METAMOTION_READ_UUID            "00002a26-0000-1000-8000-00805f9b34fb"
#define METAMOTION_NOTIFY_SERVICE_UUID  "326a9000-85cb-9195-d9dd-464cfbbae75a"
#define METAMOTION_NOTIFY_UUID          "326a9006-85cb-9195-d9dd-464cfbbae75a"
#define METAMOTION_WRITE_SERVICE_UUID   "326a9000-85cb-9195-d9dd-464cfbbae75a"
#define METAMOTION_WRITE_UUID           "326a9001-85cb-9195-d9dd-464cfbbae75a"

/*
GATT_DIS =                  '0000180A-0000-1000-8000-00805f9b34fb'
GATT_MW_CHAR_COMMAND =      '326a9001-85cb-9195-d9dd-464cfbbae75a'
GATT_MW_CHAR_NOTIFICATION = '326a9006-85cb-9195-d9dd-464cfbbae75a'
*/

class SerialInterface {
public:
    ofSerial serial;
    vector <ofSerialDeviceInfo> devices = serial.getDeviceList();
    bool bFoundUSBDevice = false;
    bool connected;
    uint16_t vendor_id = 0x1915;
    uint16_t product_id = 0xD978;
    std::string devicePath;
    
    int res                      = 0;  /* return codes from libusb functions */
    libusb_device_handle* handle = 0;  /* handle for USB device */
    int kernelDriverDetached     = 0;  /* Set to 1 if kernel driver detached */
    libusb_device **devs;
    ssize_t cnt;

    void setup() {
        /* Initialise libusb. */
        res = libusb_init(0);
        if (res != 0) {
            fprintf(stderr, "Error initialising libusb.\n");
        }

        cnt = libusb_get_device_list(NULL, &devs);
        if (cnt < 0){
            libusb_exit(NULL);
            fprintf(stderr, "Error initialising libusb and exiting.\n");
        }
        
        listDevices(devs);
        libusb_free_device_list(devs, 1);
    }
    
    void exit() {
        disconnect();
        libusb_exit(0);
    }
    
    void rescanDevices(){
        bFoundUSBDevice = false;
    }
    
    void listDevices(libusb_device **devs){
        // libusb search
        libusb_device *dev;
        int i = 0, j = 0;
        uint8_t path[8];

        while ((dev = devs[i++]) != NULL) {
            struct libusb_device_descriptor desc;
            int r = libusb_get_device_descriptor(dev, &desc);
            if (r < 0) {
                fprintf(stderr, "failed to get device descriptor");
                return;
            }

            printf("%04x:%04x (bus %d, device %d)",
                desc.idVendor, desc.idProduct,
                libusb_get_bus_number(dev), libusb_get_device_address(dev));

            r = libusb_get_port_numbers(dev, path, sizeof(path));
            if (r > 0) {
                printf(" path: %d", path[0]);
                for (j = 1; j < r; j++)
                    printf(".%d", path[j]);
            }
            printf("\n");
        }
    }
    
    std::string findMetaMotionDevice(){
        // reset bools for search
        bFoundUSBDevice = false;
        
        /* Get the first device with the matching Vendor ID and Product ID. If
         * intending to allow multiple demo boards to be connected at once, you
         * will need to use libusb_get_device_list() instead. Refer to the libusb
         * documentation for details. */
        //handle = libusb_open_device_with_vid_pid(0, 0x1915, 0xD978);
        
        struct libusb_device **devs;
        struct libusb_device *found = NULL;
        struct libusb_device *dev;
        struct libusb_device_handle *dev_handle = NULL;
        size_t i = 0;
        int r;

        if (libusb_get_device_list(0, &devs) < 0)
            return NULL;

        while ((dev = devs[i++]) != NULL) {
            struct libusb_device_descriptor desc;
            r = libusb_get_device_descriptor(dev, &desc);
            if (r < 0)
                goto out;
            if (desc.idVendor == vendor_id && desc.idProduct == product_id) {
                found = dev;
                libusb_get_device_address(found);
                printf("\taddress: %d\n", libusb_get_device_address(found));
                //devicePath = converter(libusb_get_device_address(found));
                break;
            }
        }

        if (found) {
            r = libusb_open(found, &dev_handle);
            
            connected = true;
            // connected?!
            
            if (r < 0)
                dev_handle = NULL;
        } else {
            connected = false;
        }

    out:
        libusb_free_device_list(devs, 1);
        handle = dev_handle;
        
        //
        
        if (!handle) {
            fprintf(stderr, "Unable to open device.\n");
        }
        
        /* Check whether a kernel driver is attached to interface #0. If so, we'll
         * need to detach it.
         */
        if (libusb_kernel_driver_active(handle, 0)) {
            res = libusb_detach_kernel_driver(handle, 0);
            if (res == 0) {
                kernelDriverDetached = 1;
            } else {
                fprintf(stderr, "Error detaching kernel driver.\n");
            }
        }

        /* Claim interface #0. */
        res = libusb_claim_interface(handle, 0);
        if (res != 0) {
            fprintf(stderr, "Error claiming interface.\n");
        }
        
        return libusb_get_device_address(dev);
    }
    
    bool disconnect() {
        res = libusb_release_interface(handle, 0);
        assert(res != 0);
        libusb_close(handle);
        connected = false;
        bFoundUSBDevice = false;
    }

};
