option(TOMLPLUSPLUS_VENDORED OFF)

if(TOMLPLUSPLUS_VENDORED)
	message(STATUS "TOML++: Using Github version in ${PROJECT_NAME}.")
	include(FetchContent)
	FetchContent_Declare(
	    tomlplusplus
	    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
	    GIT_TAG master
	)
	FetchContent_MakeAvailable(tomlplusplus)
	target_link_libraries(
	    ${PROJECT_NAME}
	    PUBLIC
	        tomlplusplus::tomlplusplus
	)
else()
	message(STATUS "TOML++: Using local version in ${PROJECT_NAME}.")
	find_package(tomlplusplus REQUIRED)
	target_link_libraries(
	    ${PROJECT_NAME}
	    PUBLIC
			tomlplusplus
	)
endif()
