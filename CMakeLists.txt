cmake_minimum_required(VERSION 3.14)
project(MyVectorProject C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

add_library(dst_core STATIC
    src/vec.c
    src/cstr.c
)

target_include_directories(dst_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_library(test_framework STATIC
    test/framework/framework.c
)
target_include_directories(test_framework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/test
)

add_executable(run_tests
    test/vector/test_vec_free.c
    test/vector/test_vec_init.c
    test/vector/test_vec_push.c
    test/vector/test_vec_clear.c
    test/vector/test_vec_at.c
    test/vector/test_vec_remove.c
    
    test/cstring/test_cstr_free.c
    test/cstring/test_cstr_appendn.c

    test/test_main.c
)

target_link_libraries(run_tests PRIVATE
    dst_core
    test_framework
)

enable_testing()
add_test(NAME VectorUnitTest COMMAND run_tests)