cmake_minimum_required(VERSION 3.11)


if (POLICY CMP0074)
     cmake_policy(SET CMP0074 NEW)
endif()


include(cmake/vcpkg_dependencies.cmake)
SET(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/../vcpkg")
SET(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
install_vcpkg(6ac6267f512dcec29171f4cbe62a040a974fde04)

project(PracticeProblem VERSION 0.0.0.1 DESCRIPTION "Practice Problem")


# Preprocessor definition Required for older packages
add_compile_definitions("_USE_MATH_DEFINES")
add_compile_definitions("_ENABLE_EXTENDED_ALIGNED_STORAGE")

set_vcpkg_dependencies(
	eigen3
	gtest
)

find_package(Eigen3 CONFIG REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message(STATUS CMAKE_MODULE_PATH=${CMAKE_MODEL_PATH})
find_package(GTest CONFIG REQUIRED)

if (WIN32)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /D RELWITHDEBINFO")
endif (WIN32)

set(INCLUDE_FILES
	src/TerrainDistanceCalculator.h
)

set (SOURCE_FILES
	src/TerrainDistanceCalculator.cpp
)

set (PLUGIN_NAME Plugin)
	
add_library(${PLUGIN_NAME} SHARED 
	${INCLUDE_FILES} 
	${SOURCE_FILES}
	)

target_compile_definitions(${PLUGIN_NAME} PRIVATE "Plugin_EXPORTS")

target_include_directories(${PLUGIN_NAME} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/external/include
	${EIGEN3_INCLUDE_DIRS}
)

set(PROJECT_BINARY_DEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../PracticeProblemUnity/Assets/Plugins CACHE PATH "location of the dll to be used with Unity")

ADD_CUSTOM_COMMAND(TARGET ${PLUGIN_NAME}
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PLUGIN_NAME}>/$<TARGET_FILE_NAME:${PLUGIN_NAME}> ${PROJECT_BINARY_DEST_DIR}
)

set(TEST_DATA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/test/data CACHE PATH "location of shared tutorial resources")
set(TEST_RESULT_PATH ${CMAKE_CURRENT_BINARY_DIR}/test/results CACHE PATH "location of test result data")

set (TEST_SOURCE_FILES
	src/test/Test_TerrainDistanceCalculator.cpp
)

set(TEST_INCLUDE_FILES 
	${INCLUDE_FILES}
	
)

set (TESTAPP_NAME Test)

enable_testing()
	
add_executable(${TESTAPP_NAME}
	${TEST_INCLUDE_FILES}
	${TEST_SOURCE_FILES}
	)

target_compile_definitions(${TESTAPP_NAME} PRIVATE 
	"-DTEST_DATA_PATH=\"${TEST_DATA_PATH}\""
	"-DTEST_RESULT_PATH=\"${TEST_RESULT_PATH}\""
)

target_include_directories(${TESTAPP_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/external/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${EIGEN3_INCLUDE_DIRS}
)

target_link_libraries(${TESTAPP_NAME} PRIVATE 
	${PLUGIN_NAME}
	GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)

add_test(${TESTAPP_NAME} ${TESTAPP_NAME})
