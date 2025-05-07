cmake_minimum_required(VERSION 3.10)
project(LibraryManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# 查找MySQL
find_package(MySQL REQUIRED)

# 包含目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${MySQL_INCLUDE_DIRS})

# 源文件
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 可执行文件
add_executable(library_server ${SOURCES})

# 链接库
target_link_libraries(library_server ${MySQL_LIBRARIES} pthread)