meta:
	ADDON_NAME = ofxMetaMotion
	ADDON_DESCRIPTION = MetaMotion IMU controller openFrameworks addon
	ADDON_AUTHOR = Mach1
	ADDON_TAGS = "metamotion" "hardware" "imu"
	ADDON_URL = "https://github.com/Mach1Studios/ofxMetaMotion"

common:

osx:
	ADDON_LIBS += libs/lib/osx/libmetawear.a
	ADDON_LIBS += libs/lib/osx/libnativeble-static.a
vs:
	ADDON_LIBS += libs/lib/vs/MetaWear.Win32.lib
	ADDON_LIBS += libs/lib/vs/libnativeble-static.lib

linux64:

linux:

linuxarmv7l:
