set(EXTERNAL_DIR "${CMAKE_BINARY_DIR}/external/stb")
file(MAKE_DIRECTORY "${EXTERNAL_DIR}")

set(STB_DEST "${EXTERNAL_DIR}/stb_image.h")

if(NOT EXISTS "${STB_DEST}")
    message(STATUS "Downloading external stb_image.h to ${STB_DEST}.")
    file(
        DOWNLOAD "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" "${STB_DEST}"
        EXPECTED_MD5 "27932e6fb3a2f26aee2fc33f2cb4e696"
        SHOW_PROGRESS
    )
else()
    message(STATUS "Using external stb_image.h from ${STB_DEST} in ${PROJECT_NAME}.")
endif()

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${EXTERNAL_DIR})
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        stb_image
)
