if(NEX_BUILD_PLATFORM MATCHES "VITA")
	vita_create_self(${NEX_APP_EXECUTABLE}.self ${NEX_APP_EXECUTABLE} UNSAFE NOASLR)
	vita_create_vpk(
	    ${NEX_APP_EXECUTABLE}.vpk
	    ${VITA_TITLEID} ${NEX_APP_EXECUTABLE}.self
	    VERSION ${VITA_VERSION}
	    NAME ${VITA_APP_NAME}
	    FILE platform/vita/sce_sys sce_sys
	    FILE $ENV{VITASDK}/arm-vita-eabi/modules/libGLESv2.suprx module/libGLESv2.suprx
		FILE $ENV{VITASDK}/arm-vita-eabi/modules/libgpu_es4_ext.suprx module/libgpu_es4_ext.suprx
		FILE $ENV{VITASDK}/arm-vita-eabi/modules/libIMGEGL.suprx module/libIMGEGL.suprx
		FILE $ENV{VITASDK}/arm-vita-eabi/modules/libpvrPSP2_WSEGL.suprx module/libpvrPSP2_WSEGL.suprx
	    FILE assets assets
	)
elseif(NEX_BUILD_PLATFORM MATCHES "WEB")
	set_target_properties(
	    ${NEX_APP_EXECUTABLE}
	    PROPERTIES LINK_FLAGS
	        "-sUSE_SDL=3 -sALLOW_MEMORY_GROWTH=1 --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets"
	)
elseif(NEX_BUILD_PLATFORM MATCHES "WINDOWS")
	install(FILES $<TARGET_RUNTIME_DLLS:${NEX_APP_EXECUTABLE}> TYPE BIN)
	install(
	    TARGETS
	        ${NEX_APP_EXECUTABLE}
	)
else()
	install(
	    TARGETS
	        ${NEX_APP_EXECUTABLE}
	)
endif()

