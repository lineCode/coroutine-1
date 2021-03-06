#
#   Author  : github.com/luncliff (luncliff@gmail.com)
#
add_library(coroutine_net
    ${MODULE_INTERFACE_DIR}/coroutine/net.h
    resolver.cpp
)
add_library(coro::net ALIAS coroutine_net)

set_target_properties(coroutine_net
PROPERTIES
    CXX_STANDARD    17
    WINDOWS_EXPORT_ALL_SYMBOLS  true
)

target_include_directories(coroutine_net
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(coroutine_net
PUBLIC
    coro::event
PRIVATE
    ${CMAKE_DL_LIBS}
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(coroutine_net
    PUBLIC
        FORCE_STATIC_LINK
    )
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)

    target_compile_options(coroutine_net
    PRIVATE
        -Wall
        -Wno-unused-private-field
        -Wno-unused-function
        -Wno-c++98-compat
        -Wno-reserved-id-macro
        -Wno-missing-prototypes
        -ferror-limit=5
    )
    if(WIN32)
        if(BUILD_SHARED_LIBS)
            target_compile_definitions(coroutine_net
            PRIVATE
                _WINDLL
            )
        endif()
    elseif(UNIX)
        target_compile_options(coroutine_net
        PUBLIC
            -fPIC
        PRIVATE
            -fno-rtti -fvisibility=hidden
        )
    endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    target_compile_options(coroutine_net
    PUBLIC
        -fPIC
    PRIVATE
        -Wall -Wno-unknown-pragmas
        -fno-rtti -fvisibility=hidden
        # -fno-exceptions
    )
elseif(MSVC)
    target_compile_options(coroutine_net
    PRIVATE
        /W4
    )
endif()

if(WIN32)
    target_sources(coroutine_net
    PRIVATE
        io_windows.cpp
    )
    target_link_libraries(coroutine_net
    PUBLIC
        kernel32 ws2_32 mswsock
    )
elseif(APPLE)
    target_sources(coroutine_net
    PRIVATE
        io_darwin.cpp
    )
elseif(UNIX) # actually we can't sure it's a linux here ...
    target_sources(coroutine_net
    PRIVATE
        io_linux.cpp
    )
endif()
if(NOT MSVC)
    if(${CMAKE_BUILD_TYPE} MATCHES Debug)
        # code coverage option can lead to compiler crash
        # list(APPEND CMAKE_CXX_FLAGS "--coverage")
        target_compile_options(coroutine_event
        PRIVATE
            -g -O0
        )
    else()
        target_compile_options(coroutine_event
        PRIVATE
            -O3
        )
    endif()
endif()

install(FILES   ${MODULE_INTERFACE_DIR}/coroutine/net.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/coroutine
)
install(TARGETS         coroutine_net
        EXPORT          coroutine-config
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
