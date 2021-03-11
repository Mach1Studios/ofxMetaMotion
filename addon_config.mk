meta:
	ADDON_NAME = ofxMetaMotion
	ADDON_DESCRIPTION = MetaMotion IMU controller openFrameworks addon
	ADDON_AUTHOR = Mach1
	ADDON_TAGS = "metamotion" "hardware" "imu"
	ADDON_URL = "https://github.com/Mach1Studios/ofxMetaMotion"

common:
linux:
linux64:
linuxarmv6l:
linuxarmv7l:
android/armeabi-v7a:
android/x86:
osx:
	ADDON_LIBS += libs/libmetawear/lib/osx/libmetawear.a
	ADDON_LIBS += libs/libnativeble/lib/osx/libnativeble-static.a
ios:
tvos:
msys2:
vs:
	ADDON_LIBS += libs/libmetawear/lib/vs/x64/Release/MetaWear.Win32.lib
	ADDON_LIBS += libs/libnativeble/lib/vs/x64/Release/nativeble-static.lib
