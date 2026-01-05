cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
option(NEX_ASSET_LOADER_TOML OFF)

message(STATUS "NEX_BUILD_PLATFORM = ${NEX_BUILD_PLATFORM}")

if(NEX_BUILD_PLATFORM MATCHES "VITA")
    include("cmake/vita_pre.cmake")
endif()

project(nex)
add_library(
    ${PROJECT_NAME}
        "src/core/audio.cpp"
        "src/core/file.cpp"
        "src/core/state.cpp"
        "src/core/delta.cpp"
        "src/core/assetmanager.cpp"
        "src/core/inputmanager.cpp"
        "src/gl/defines.cpp"
        "src/gl/window.cpp"
        "src/gl/mtsdf/font.cpp"
        "src/gl/mtsdf/text.cpp"
        "src/gl/assets/shader.cpp"
        "src/gl/assets/texture.cpp"
        "src/gl/assets/texturearray.cpp"
        "src/gl/rendertexture.cpp"
        "src/prefabs/timer.cpp"
        "src/prefabs/sprite.cpp"
        "src/prefabs/spriteatlas.cpp"
)
add_subdirectory(lib)

if(NEX_ASSET_LOADER_TOML)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
            TOML_EXCEPTIONS=0
            TOML_HEADER_ONLY=0
            TOML_ENABLE_FORMATTERS=0
            TOML_ENABLE_UNRELEASED_FEATURES=0
    )
endif()

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        "inc/nex"
    PUBLIC
    	"inc"
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS ON
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror -fno-exceptions>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Werror -fno-exceptions>
)

if(NEX_BUILD_PLATFORM MATCHES "VITA")
    include("cmake/vita_post.cmake")
else()
    include("cmake/other_post.cmake")
endif()
