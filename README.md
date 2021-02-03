# ofxWitmotion

### WORK IN PROGRESS ###

MetaMotion addon for OpenFrameworks

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
    mmc.getAcceleration();
    mmc.getGyro();
    mmc.getAngle();
    mmc.getMagneticField();
```
