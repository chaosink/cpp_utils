cmake_minimum_required(VERSION 3.2.1)
project(cpp_utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_executable(ChangeMonitor_test
	ChangeMonitor/ChangeMonitor_test.cpp
)

add_executable(ScopedAssignment_test
	ScopedAssignment/ScopedAssignment_test.cpp
)

add_executable(Toggle_test
	Toggle/Toggle_test.cpp
)
target_link_libraries(Toggle_test
	GL
	glfw
)
