# The name of the target operating system.
set(CMAKE_SYSTEM_NAME Generic)

# Which compilers to use for C and C++.
set(CMAKE_C_COMPILER "$ENV{NDK_HOST_NAME}-gcc")
set(CMAKE_CXX_COMPILER "$ENV{NDK_HOST_NAME}-g++")

set(ANDROID 1)
