cmake_minimum_required(VERSION 3.11)

project(GPUParticleSimulation)
set(TARGET_NAME GPUParticleSimulation)

set(CMAKE_CXX_STANDARD 17)

# define the include directories
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

# list include files to variables
file (GLOB_RECURSE ENGINE_INCLUDES
	LIST_DIRECTORIES false
	"includes/*.h"
)
file(GLOB CORE_SOURCE
	"src/*.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

add_executable(${TARGET_NAME} ${CORE_SOURCE} ${ENGINE_INCLUDES})

set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX ".debug")

target_link_libraries(${TARGET_NAME} d3d11 dxgi d3dcompiler)
