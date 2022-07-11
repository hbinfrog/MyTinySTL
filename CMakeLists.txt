cmake_minimum_required(VERSION 3.21)
project(MyTinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(MyTinySTL main.cpp vector.h allocator.h construct.h iterator.h type_traits.h util.h memory.h algobase.h uninitialized.h exceptdef.h list.h functional.h functional.h deque.h stack.h queue.h algo.h set.h map.h hashtable.h algorithm.h basic_string.h string.h rb_tree.h unordered_map.h unordered_set.h)
