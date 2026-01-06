message(STATUS "Linking with system libraries.")

if(NEX_BUILD_PLATFORM MATCHES "WINDOWS")
    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            glad_gles2
    )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            GL
    )
endif()

if(EMSCRIPTEN)
    install(
        FILES
            ${CMAKE_BINARY_DIR}/${NEX_APP_EXECUTABLE}.js
            ${CMAKE_BINARY_DIR}/${NEX_APP_EXECUTABLE}.data
            ${CMAKE_BINARY_DIR}/${NEX_APP_EXECUTABLE}.wasm
            ${CMAKE_SOURCE_DIR}/platform/web/favicon.ico
            ${CMAKE_SOURCE_DIR}/platform/web/index.html
        DESTINATION web
    )
else()
    install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/assets
        DESTINATION bin
    )
endif()
