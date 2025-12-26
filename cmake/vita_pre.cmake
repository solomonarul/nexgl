if(NOT DEFINED ENV{VITASDK})
    message(FATAL_ERROR "Please set VITASDK environment variable")
endif()

set(CMAKE_TOOLCHAIN_FILE $ENV{VITASDK}/share/vita.toolchain.cmake CACHE PATH "")
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
