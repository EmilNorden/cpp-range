cmake_minimum_required (VERSION 3.8.0)
project (wh VERSION 0.1 LANGUAGES CXX)

# # # General project requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CXX_FEATURES
	cxx_auto_type
	cxx_constexpr
	cxx_lambdas
	cxx_nullptr
	cxx_override
	cxx_range_for
	cxx_right_angle_brackets
	cxx_static_assert
	cxx_strong_enums
	cxx_variadic_macros
	cxx_variadic_templates)


# # # CMake Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${wh_SOURCE_DIR}/cmake")

# # # General compile flags
add_definitions(-DENABLE_ASSERTIONS)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")

	message(STATUS "DOING THE THINGGGGG")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")

endif()


file(GLOB_RECURSE CPP_SOURCES src/*.cpp)

add_executable(wh
    ${CPP_SOURCES})
