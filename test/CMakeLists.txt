cmake_minimum_required(VERSION 3.12)

# create_ctest( ... )
function(create_ctest FILE_NAME)
    set(TEST_NAME test_${FILE_NAME})

    # create a test exe with the given name ...
    add_executable(${TEST_NAME} ${FILE_NAME}.cpp)
    add_test(${FILE_NAME} ${TEST_NAME})
    
    set_target_properties(${TEST_NAME}
    PROPERTIES
        CXX_STANDARD 20
    )
    target_include_directories(${TEST_NAME}
    PRIVATE
        ${GSL_INCLUDE_DIR}
    )
    target_link_libraries(${TEST_NAME}
    PRIVATE
        coroutine_portable
    )
    target_compile_definitions(${TEST_NAME}
    PRIVATE
        CMAKE_TEST NDEBUG
    )
    if(WIN32)
    elseif(APPLE)
        target_link_libraries(${TEST_NAME}
        PRIVATE
            c++
        )
    else()
        target_link_libraries(${TEST_NAME}
        PRIVATE
            stdc++ c++
        )
    endif()
endfunction()

#
#   <coroutine/frame.h>
#   <coroutine/return.h>
#
create_ctest( return_frame_destroy_with_handle )
create_ctest( return_frame_destroy_with_return )
create_ctest( return_frame_not_coroutine )
create_ctest( return_frame_not_subroutine )
