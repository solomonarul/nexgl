include(sdl3.cmake)
include(stb_image.cmake)
include(tomlplusplus.cmake)

if(NEX_BUILD_PLATFORM MATCHES "WINDOWS")
    add_subdirectory(glad_gles2)
endif()