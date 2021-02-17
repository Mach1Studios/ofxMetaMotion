# ofxMetaMotion

MetaMotion addon for OpenFrameworks, automatically searches for MetaMotion device via BLE device name `MetaWear`, connects to the device and streams the Euler orientation angles from the MetaMotion device.

## Setup

#### MacOS
- Add `CoreBluetooth.framework` to the project's target Frameworks within the xcodeproj

## Quick example: 

`ofApp.h`

```cpp    
    metamotionController mmc;
```

`ofApp.cpp `

```cpp
    // in ofApp::setup()
    mmc.setup();
    
    // in ofApp::update()
    mmc.update(); // update handles device searching and auto-connection
    mmc.getAngle();
```
