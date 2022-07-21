add_executable(RBTreeTests
	tests.cpp
)

add_executable(RBTreeFuzzer
	fuzzer.cpp
)

set_property(TARGET RBTreeTests PROPERTY CXX_STANDARD 20)
set_property(TARGET RBTreeFuzzer PROPERTY CXX_STANDARD 20)

enable_testing()

target_link_libraries(RBTreeTests
	RedBlackTree
	gtest_main
)

target_link_libraries(RBTreeFuzzer
	RedBlackTree
)

include(GoogleTest)
gtest_discover_tests(RBTreeTests)
