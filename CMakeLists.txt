cmake_minimum_required(VERSION 3.4)
project(DataForge VERSION 1.0)

# C++标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(ENABLE_CUSTOM_COMPILER_FLAGS Off)
set(DOCUMENTATION False)

# 包含的头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/DataForge)
include_directories(${PROJECT_SOURCE_DIR}/libs/mosquitto/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/curl/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/curlpp/include)

# 枚举头文件
file(GLOB_RECURSE INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/DataForge/*.h)

# 指定引用的外部库的搜索路径
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libs)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# 查找当前目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
aux_source_directory(src DIR_SRCS)

# 添加子目录（如果你有子项目或库）
add_subdirectory(libs)

# 指定生成目标
add_executable(DataForge ${DIR_SRCS})

# 确保链接到libs目录下编译的libcurl
target_link_libraries(DataForge PRIVATE libcurl curlpp mosquittopp cjson serial sqlite3)

# 添加 Http 单元测试
add_executable(HttpPostGet examples/HttpPostGet.cc src/HttpClient.cc)
target_link_libraries(HttpPostGet PRIVATE curlpp libcurl)

# 添加 MQTT 客户端 单元测试测试
add_executable(MqttClientTest examples/MqttClientTest.cc src/MqttClient.cc)
target_link_libraries(MqttClientTest PRIVATE mosquittopp)

# 添加 JSON 解析器单元测试
add_executable(JsonParserTest examples/JsonParserTest.cc src/JsonParser.cc)
target_link_libraries(JsonParserTest PRIVATE cjson)

# 添加 libserial 串口单元测试
add_executable(SerialPortTest examples/SerialPortTest.cc src/SerialPort.cc)
target_link_libraries(SerialPortTest PRIVATE serial)

# 添加 WebInterface 单元测试
add_executable(WebInterfaceTest examples/WebInterfaceTest.cc src/WebInterface.cc)
target_link_libraries(WebInterfaceTest PRIVATE curlpp libcurl)

# 添加 DatabaseManager 单元测试
add_executable(DatabaseManagerTest examples/DatabaseManagerTest.cc src/DatabaseManager.cc)
target_link_libraries(DatabaseManagerTest PRIVATE sqlite3)

# 添加 DataProcessor 单元测试
add_executable(DataProcessorTest 
    examples/DataProcessorTest.cc 
    src/DataProcessor.cc 
    src/HttpClient.cc 
    src/MqttClient.cc 
    src/JsonParser.cc 
    src/SerialPort.cc 
    src/WebInterface.cc 
    src/DatabaseManager.cc
)
target_link_libraries(DataProcessorTest PRIVATE libcurl curlpp mosquittopp cjson serial sqlite3)

enable_testing()

add_test(NAME HttpPostGetTest COMMAND HttpPostGet)
add_test(NAME MqttClientTest COMMAND MqttClientTest)
add_test(NAME JsonParserTest COMMAND JsonParserTest)
add_test(NAME SerialPortTest COMMAND SerialPortTest)
add_test(NAME WebInterfaceTest COMMAND WebInterfaceTest)
add_test(NAME DatabaseManagerTest COMMAND DatabaseManagerTest)
add_test(NAME DataProcessorTest COMMAND DataProcessorTest)