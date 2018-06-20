cmake_minimum_required(VERSION 3.2.1)
project(cpp_utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(ChangeMonitor_test
	ChangeMonitor/ChangeMonitor_test.cpp
)

add_executable(ScopedAssignment_test
	ScopedAssignment/ScopedAssignment_test.cpp
)
