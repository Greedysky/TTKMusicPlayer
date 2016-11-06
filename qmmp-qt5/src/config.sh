NDK=C:/androidtk/android-ndk-r10b
PLATFORM=$NDK/platforms/android-8/arch-arm/
PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt/windows
CC=$PREBUILT/bin/arm-linux-androideabi-gcc
./configure --build=arm --host=arm-linux --enable-static --with-pic CC="$CC --sysroot=$PLATFORM"