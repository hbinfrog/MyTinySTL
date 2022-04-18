cmake_minimum_required(VERSION 3.21)
project(MyTinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(MyTinySTL main.cpp vector.h allocator.h construct.h iterator.h type_traits.h util.h memory.h algobase.h uninitialized.h exceptdef.h list.h functional.h)
