cmake_minimum_required(VERSION 3.4)
project(DataForge VERSION 1.0)

# C++标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(ENABLE_CUSTOM_COMPILER_FLAGS Off)
set(DOCUMENTATION False)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# 包含的头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/DataForge)
include_directories(${PROJECT_SOURCE_DIR}/libs/mosquitto/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/curl/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/libserial/src)
include_directories(${PROJECT_SOURCE_DIR}/libs/cjson)
include_directories(${PROJECT_SOURCE_DIR}/libs/curlpp/include)
include_directories(
   ${PROJECT_SOURCE_DIR}/libs/poco/ApacheConnector/include
   ${PROJECT_SOURCE_DIR}/libs/poco/CppParser/include
   ${PROJECT_SOURCE_DIR}/libs/poco/CppUnit/include
   ${PROJECT_SOURCE_DIR}/libs/poco/Crypto/include
   ${PROJECT_SOURCE_DIR}/libs/poco/Data/include
   ${PROJECT_SOURCE_DIR}/libs/poco/Encodings/include
   ${PROJECT_SOURCE_DIR}/libs/poco/Foundation/include
   ${PROJECT_SOURCE_DIR}/libs/poco/JSON/include
   ${PROJECT_SOURCE_DIR}/libs/poco/MongoDB/include
   ${PROJECT_SOURCE_DIR}/libs/poco/Net/include
   ${PROJECT_SOURCE_DIR}/libs/poco/NetSSL_OpenSSL/include
   ${PROJECT_SOURCE_DIR}/libs/poco/NetSSL_Win/include
   ${PROJECT_SOURCE_DIR}/libs/poco/openssl/build/include
   ${PROJECT_SOURCE_DIR}/libs/poco/PDF/include
   ${PROJECT_SOURCE_DIR}/libs/poco/Redis/include
   ${PROJECT_SOURCE_DIR}/libs/poco/SevenZip/include
   ${PROJECT_SOURCE_DIR}/libs/poco/Util/include
   ${PROJECT_SOURCE_DIR}/libs/poco/XML/include
   ${PROJECT_SOURCE_DIR}/libs/poco/Zip/include
)

# 枚举头文件
file(GLOB_RECURSE INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/DataForge/*.h)

# 指定引用的外部库的搜索路径
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libs)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# 查找当前目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
aux_source_directory(src DIR_SRCS)

# libserial arguments
set(LIBSERIAL_BUILD_DOCS False)
set(LIBSERIAL_PYTHON_ENABLE False)
# offline, my boy
set(LIBSERIAL_ENABLE_TESTING False)

# curl arguments
set(BUILD_STATIC_CURL False)
# disable cjson uninstall target or Poco will complain
set(ENABLE_CJSON_UNINSTALL False)

set(thirdparty_libraries
	curl
	curlpp
	cjson
	libserial
	mosquitto
	poco
)
set(poco_targets
# Poco::ApacheConnector
# Poco::CppParser
# Poco::CppUnit
# Poco::Crypto
# Poco::Data
# Poco::Encodings
Poco::Foundation
Poco::JSON
# Poco::MongoDB
Poco::Net
# Poco::NetSSL_OpenSSL
# Poco::NetSSL_Win
# Poco::PDF
# Poco::Redis
# Poco::SevenZip
# Poco::Util
# Poco::XML
Poco::Zip
)
set(thirdparty_link_libraries
	cjson
	${libcurl_static}
	curlpp
	libserial_static
	mosquittopp
	sqlite3
	${poco_targets}
)

# 添加子目录
foreach(lib IN LISTS thirdparty_libraries)
	message("[thirdparty] including library ${lib}")
	add_subdirectory("libs/${lib}")
endforeach()

# for libserial
link_directories(${CMAKE_BINARY_DIR}/lib)

set(link_libraries
	${thirdparty_link_libraries}
)

# 指定生成目标
add_executable(DataForge ${DIR_SRCS})

# 确保链接到libs目录下编译的libs
target_link_libraries(DataForge PRIVATE ${link_libraries})

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
target_link_libraries(WebInterfaceTest PRIVATE curlpp libcurl ${poco_targets})

# 添加 DatabaseManager 单元测试
add_executable(DatabaseManagerTest examples/DatabaseManagerTest.cc src/DatabaseManager.cc)
target_link_libraries(DatabaseManagerTest PRIVATE sqlite3)

# 添加 DataProcessorTest 单元测试
add_executable(DataProcessorTest examples/DataProcessorTest.cc src/DataProcessor.cc)
target_link_libraries(DataProcessorTest PRIVATE )

# 添加 DataForgeEngine 单元测试
add_executable(DataForgeEngineTest
    examples/DataForgeEngineTest.cc
    src/DataForgeEngine.cc
    src/HttpClient.cc
    src/MqttClient.cc
    src/JsonParser.cc
    src/SerialPort.cc
    src/WebInterface.cc
    src/DatabaseManager.cc
)
target_link_libraries(DataForgeEngineTest PRIVATE ${link_libraries})

enable_testing()

add_test(NAME HttpPostGetTest COMMAND HttpPostGet)
add_test(NAME MqttClientTest COMMAND MqttClientTest)
add_test(NAME JsonParserTest COMMAND JsonParserTest)
add_test(NAME SerialPortTest COMMAND SerialPortTest)
add_test(NAME WebInterfaceTest COMMAND WebInterfaceTest)
add_test(NAME DatabaseManagerTest COMMAND DatabaseManagerTest)
add_test(NAME DataProcessorTest COMMAND DataProcessorTest)
add_test(NAME DataForgeEngineTest COMMAND DataForgeEngineTest)
