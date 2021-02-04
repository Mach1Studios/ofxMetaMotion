# ofxWitmotion

### WORK IN PROGRESS ###

MetaMotion addon for OpenFrameworks

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
    mmc.connectDevice(int DEVICE_ID);
    
    // in ofApp::update()
    mmc.update();
    mmc.getEulerAngle();
```
