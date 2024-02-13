cmake_minimum_required(VERSION 3.10)

project(VulkanRenderer)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.h)

file(GLOB_RECURSE SHADERS ../shaders/*.hlsl)

if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
    add_executable(VulkanRenderer ${SOURCES} ${HEADERS} ${SHADERS})
    target_include_directories(VulkanRenderer
		PUBLIC $ENV{VULKAN_SDK}/Include
		PUBLIC ${VULKAN_INCLUDE_DIR}
	)
    target_link_libraries(VulkanRenderer PUBLIC $ENV{VULKAN_SDK}/Lib)
endif (WIN32)

set_source_files_properties(${SHADERS} PROPERTIES
    VS_TOOL_OVERRIDE "None"
    # Tip: Swap "None" for "FXCompile" to have them be compiled by VS.(Great for D3D11/12)
)