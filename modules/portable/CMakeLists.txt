#
#   Author  : github.com/luncliff (luncliff@gmail.com)
#
add_library(coroutine_portable INTERFACE)

if(USE_PORTABLE_HEADER)
    message(STATUS "Customizaton")
    message(STATUS "  <coroutine/frame.h> is suppressing <experimental/coroutine>")
    message(STATUS)
    set_target_properties(coroutine_portable
    PROPERTIES
        INTERFACE_COMPILE_DEFINITIONS "USE_PORTABLE_COROUTINE_HANDLE"
    )
endif()

target_include_directories(coroutine_portable
INTERFACE
    $<BUILD_INTERFACE:${MODULE_INTERFACE_DIR}>
    $<BUILD_INTERFACE:${GSL_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    if(WIN32)
        # 'target_compile_options' removes duplicated -Xclang directive.
        # avoide the removal using cmake flag variable
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -fcoroutines-ts" PARENT_SCOPE)
        target_compile_options(coroutine_portable
        INTERFACE
            /std:c++latest
            -fms-compatibility -Xclang -fcoroutines-ts
        )
        # in this case, we always use portable header
        set_target_properties(coroutine_portable
        PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "USE_PORTABLE_COROUTINE_HANDLE;_RESUMABLE_FUNCTIONS_SUPPORTED"
        )
        
    elseif(UNIX OR APPLE)
        target_compile_options(coroutine_portable
        INTERFACE
            -std=c++2a -stdlib=libc++
            -fcoroutines-ts
        )
    endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    target_compile_options(coroutine_portable
    INTERFACE
        -std=gnu++2a
        -fcoroutines -fconcepts # -fno-exceptions
    )

elseif(MSVC)
    set_target_properties(coroutine_portable
    PROPERTIES
        INTERFACE_LINK_FLAGS "${LINK_FLAGS} /errorReport:send"
    )
    target_compile_options(coroutine_portable
    INTERFACE
        /std:c++latest /await
    )

endif()

install(FILES   ${MODULE_INTERFACE_DIR}/coroutine/frame.h
                ${MODULE_INTERFACE_DIR}/coroutine/return.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/coroutine
)
install(TARGETS         coroutine_portable
        EXPORT          coroutine-config
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)
