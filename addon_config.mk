meta:
	ADDON_NAME = ofxMetaMotion
	ADDON_DESCRIPTION = MetaMotion IMU controller openFrameworks addon
	ADDON_AUTHOR = Mach1
	ADDON_TAGS = "metamotion" "hardware" "imu"
	ADDON_URL = "https://github.com/Mach1Studios/ofxMetaMotion"

common:

osx:
	ADDON_LIBS += libs/lib/osx/libmetawear.dylib
	ADDON_LIBS += libs/lib/osx/libmetawear.dylib.0
	ADDON_LIBS += libs/lib/osx/libmetawear.dylib.0.19.1

vs:

linux64:

linux:

linuxarmv7l:
