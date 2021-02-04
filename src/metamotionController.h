#pragma once

#include "ofMain.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <csignal>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include <sys/types.h>
#include <errno.h>

#ifndef WIN32
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#endif

#include "nativebleInterface.h"

#include "metawear/core/metawearboard.h"
#include "metawear/core/module.h"

#include "metawear/core/status.h"
#include "metawear/core/debug.h"
#include "metawear/core/logging.h"
#include "metawear/core/macro.h"
#include "metawear/peripheral/led.h"

#include "metawear/core/data.h"
#include "metawear/core/datasignal.h"
#include "metawear/core/types.h"
#include "metawear/sensor/sensor_common.h"
#include "metawear/sensor/sensor_fusion.h"

class metamotionController {
public:
    metamotionController();
    ~metamotionController();
    
    void setup();
    void update();
    
    // returns available bluetooth devices
    std::vector<string> getDeviceIDs();
    
    // ble connection functions
    void connectDevice(int deviceID);
    void disconnectDevice();
    
    // get functions for data
    float* getEulerAngle();
    
    // ble
    nativebleInterface nativeble;
    std::vector<NativeBLE::DeviceDescriptor> devices;
    
    void data_printer(void* context, const MblMwData* data);
    void configure_sensor_fusion(MblMwMetaWearBoard* board);
    void enable_fusion_sampling(MblMwMetaWearBoard* board);
    void disable_fusion_sampling(MblMwMetaWearBoard* board);

    void resetOrientation();
    void tare();
    
    // to check if a device connected currently
    bool isConnected;
};
