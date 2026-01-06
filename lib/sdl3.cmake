option(SDL3_VENDORED OFF)
option(SDL3_MIXER_VENDORED OFF)

if(SDL3_VENDORED)
    include(FetchContent)

    if(EMSCRIPTEN)
        set(SDL_EMSCRIPTEN ON CACHE BOOL "" FORCE)
    endif()

    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)

    message("SDL3: Using Github version in ${PROJECT_NAME}.")
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.x
    )
    FetchContent_MakeAvailable(SDL3)

    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            SDL3::SDL3
    )
else()
    message(STATUS "SDL3: Using local version in ${PROJECT_NAME}.")
    find_package(SDL3 REQUIRED)

    if(NEX_BUILD_PLATFORM MATCHES "WINDOWS")
        target_link_libraries(
            ${PROJECT_NAME}
            PUBLIC
                SDL3::SDL3
        )
    else()
        target_link_libraries(
            ${PROJECT_NAME}
            PUBLIC
                SDL3
        )
    endif()
endif()

if(SDL3_MIXER_VENDORED)
    include(FetchContent)

    if(EMSCRIPTEN)
        set(SDL_EMSCRIPTEN ON CACHE BOOL "" FORCE)
    endif()

    message("SDL3_mixer: Using Github version in ${PROJECT_NAME}.")
    FetchContent_Declare(
        SDL3_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
        GIT_TAG main
    )
    FetchContent_MakeAvailable(SDL3_mixer)

    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            SDL3_mixer::SDL3_mixer
    )
else()
    message(STATUS "SDL3_mixer: Using local version in ${PROJECT_NAME}.")
    find_package(SDL3_mixer REQUIRED)
    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            SDL3_mixer
    )
endif()
