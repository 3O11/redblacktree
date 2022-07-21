add_executable(RBTreeBenchmarks
	benchmark.cpp
)

set_property(TARGET RBTreeBenchmarks PROPERTY CXX_STANDARD 20)

target_link_libraries(RBTreeBenchmarks
	RedBlackTree
)
