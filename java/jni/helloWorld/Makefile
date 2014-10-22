#java -Djava.library.path=. HelloJNI
JAVA_JNI_HEADERS := /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers/

all:
	gcc -I$(JAVA_JNI_HEADERS) -shared -o libhello.dylib helloJNI.c
