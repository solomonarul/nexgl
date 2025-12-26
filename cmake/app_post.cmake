if(NEX_BUILD_PLATFORM MATCHES "VITA")
	vita_create_self(${NEX_APP_EXECUTABLE}.self ${NEX_APP_EXECUTABLE} UNSAFE NOASLR)
	vita_create_vpk(
	    ${NEX_APP_EXECUTABLE}.vpk
	    ${VITA_TITLEID} ${NEX_APP_EXECUTABLE}.self
	    VERSION ${VITA_VERSION}
	    NAME ${VITA_APP_NAME}
	    FILE platform/vita/sce_sys sce_sys
	    FILE platform/vita/module module
	    FILE assets assets
	)
elseif(NEX_BUILD_PLATFORM MATCHES "WEB")
	set_target_properties(
	    ${NEX_APP_EXECUTABLE}
	    PROPERTIES LINK_FLAGS
	        "-sUSE_SDL=3 -sALLOW_MEMORY_GROWTH=1 --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets"
	)
else()
	install(
	    TARGETS
	        ${NEX_APP_EXECUTABLE}
	)
endif()
