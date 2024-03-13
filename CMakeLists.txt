cmake_minimum_required(VERSION 3.4)
project(DataForge VERSION 1.0)

# C++标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(ENABLE_CUSTOM_COMPILER_FLAGS Off)

# 包含的头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/DataForge)
include_directories(${PROJECT_SOURCE_DIR}/libs/cjson)
include_directories(${PROJECT_SOURCE_DIR}/libs/curl/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/curlpp/include)

# 枚举头文件
file(GLOB_RECURSE INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/DataForge/*.h)

# 指定引用的外部库的搜索路径
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libs)

# 查找当前目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
aux_source_directory(src DIR_SRCS)

# 添加子目录（如果你有子项目或库）
add_subdirectory(libs)

# 指定生成目标
add_executable(DataForge ${DIR_SRCS})

# 确保链接到libs目录下编译的libcurl
target_link_libraries(DataForge PRIVATE libcurl curlpp)

# 添加单元测试
add_executable(HttpPostGet examples/HttpPostGet.cc src/HttpClient.cc)
target_link_libraries(HttpPostGet PRIVATE curlpp libcurl)
# set_target_properties(HttpPostGet PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
# )

enable_testing()

add_test(NAME HttpPostGetTest COMMAND HttpPostGet)