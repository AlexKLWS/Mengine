BUILD_TYPE=Debug

echo Starting dependencies build $BUILD_TYPE configuration...

ANDROID_SDK=~/Library/Android/sdk
ANDROID_NDK=$ANDROID_SDK/ndk-bundle
ANDROID_SYSROOT="$ANDROID_NDK/sysroot"
MAKE_PROGRAM="$ANDROID_SDK/cmake/3.6.4111459/bin/ninja"
TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake"
CMAKE_EXE="$ANDROID_SDK/cmake/3.6.4111459/bin/cmake"
CMAKE_PATH="$PWD/../../CMake/Depends_Android_SDL"
BUILD_TEMP_DIR="$PWD/../../build_temp/build_android_sdl_$BUILD_TYPE"

mkdir -p $BUILD_TEMP_DIR/$BUILD_TYPE
pushd $BUILD_TEMP_DIR/$BUILD_TYPE

$CMAKE_EXE -G "Android Gradle - Ninja" -DANDROID_PLATFORM=android-15 -DANDROID_ARM_NEON=TRUE -DANDROID_ABI=armeabi-v7a -DANDROID_STL=c++_shared -DANDROID_TOOLCHAIN=clang -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CONFIGURATION_TYPES:STRING=$BUILD_TYPE -DCMAKE_MAKE_PROGRAM=$MAKE_PROGRAM -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE -DCMAKE_SYSROOT_COMPILE=$ANDROID_SYSROOT -DCMAKE_C_FLAGS="-std=c11" -DCMAKE_CXX_FLAGS="-std=c++14 -frtti -fexceptions" $CMAKE_PATH

$CMAKE_EXE --build /. --config $BUILD_TYPE

popd

echo Done