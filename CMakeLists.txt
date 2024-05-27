cmake_minimum_required(VERSION 3.27)
project(Tanzanite)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/proj/*.cpp" "src/proj/*.h")
file(GLOB_RECURSE RESOURCE_FILES "src/resource/*")

include_directories(${CMAKE_SOURCE_DIR}/ext)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/tanzanite-run)

# Static linking flags for MinGW
if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

add_executable(Tanzanite ${PROJECT_SOURCE_FILES} ${RESOURCE_FILES})

if(WIN32 AND NOT MINGW)
    target_link_libraries(Tanzanite PRIVATE ws2_32 vcruntime msvcrt ucrt)
elseif(MINGW)
    target_link_libraries(Tanzanite PRIVATE ws2_32)
endif()
