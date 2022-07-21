cmake_minimum_required(VERSION 3.20)

project("RedBlackTree")

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -fconcepts)
endif()

# Add GTest dependency
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory("redblack-tree")
add_subdirectory("tests")
add_subdirectory("benchmarks")
