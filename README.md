# ofxMetaMotion

MetaMotion addon for OpenFrameworks, automatically searches for MetaMotion device via BLE device name `MetaWear`, connects to the device and streams the Euler orientation angles from the MetaMotion device.

## Setup

#### MacOS
- Add `CoreBluetooth.framework` to the project's target Frameworks within the xcodeproj
- Add the following *Run Script* to your *Build Phases*
```
rsync -aved "$OF_PATH/addons/ofxMetaMotion/libs/libmetawear/lib/osx/libmetawear.dylib" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/";
install_name_tool -id @executable_path/../Frameworks/libmetawear.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/libmetawear.dylib";
install_name_tool -change libmetawear.dylib @executable_path/../Frameworks/libmetawear.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
```

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
