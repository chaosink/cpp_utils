cmake_minimum_required(VERSION 3.2.1)
project(cpp_utils LANGUAGES CXX)

# compiling options
set(CMAKE_CXX_STANDARD 17)
if(MSVC)
	set(EMBREE_MAX_ISA "SSE4.2" CACHE STRING "Selects highest ISA to support.")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox /openmp -d2SSAOptimizer-") # -d2SSAOptimizer- for MSVC bug
else()
	set(CXX_WARNINGS "-Wall -Wextra -Wpointer-arith -Wcast-align -fstrict-aliasing")
	if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
		set(CXX_WARNINGS "${CXX_WARNINGS} -Wno-maybe-uninitialized")
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS} -fvisibility-inlines-hidden -O3 -fopenmp")

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif()

include_directories(
	common
)

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

add_executable(Button_test
	Button/Button_test.cpp
)
target_link_libraries(Button_test
	GL
	glfw
)
