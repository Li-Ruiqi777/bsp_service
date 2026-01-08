cmake_minimum_required(VERSION 3.10)
project(bsp-service VERSION 1.0.0 LANGUAGES CXX)

include(cmake/functions.cmake)
include(cmake/toolchain.cmake)

list(APPEND CMAKE_PREFIX_PATH "/home/lrq/linux/nfs/qtrootfs/usr/")

find_package(cppzmq REQUIRED)
find_package(cereal CONFIG REQUIRED)

# 公共模块源文件
set(COMMON_SOURCES
    src/common/zmq_wrapper.cpp
    src/common/config.cpp
    src/common/utils.cpp
)

# io-service 源文件
set(IO_SERVICE_SOURCES
    src/io_service/io_service.cpp
    src/io_service/io_handler.cpp
    src/io_service/main.cpp
)

# 创建 io-service 可执行文件
add_executable(io-service
    ${COMMON_SOURCES}
    ${IO_SERVICE_SOURCES}
)

# 设置包含目录
target_include_directories(io-service PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

# 设置链接器搜索路径
list(APPEND CMAKE_EXE_LINKER_FLAGS "-L/home/lrq/linux/nfs/qtrootfs/usr/lib")
# 链接库
target_link_libraries(io-service
    cppzmq
    cereal::cereal
    bsp
    pthread
)

# 创建测试客户端（预留）
add_executable(test_io_client
    test/test_io_client.cpp
    ${COMMON_SOURCES}
)

target_include_directories(test_io_client PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(test_io_client PRIVATE
    cppzmq 
    pthread
    cereal::cereal
)

# 安装规则
# DESTINATION为 CMAKE_INSTALL_PREFIX
set(CMAKE_INSTALL_PREFIX /home/lrq/linux/nfs/qtrootfs/usr)

install(FILES configs/io_service.json
    DESTINATION etc/bsp-service
)

install(TARGETS io-service test_io_client
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)


# 打印配置信息
message(STATUS "BSP Service Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  BSP Library: ${BSP_LIB}")
message(STATUS "  BSP Include: ${BSP_INCLUDE_DIR}")
message(STATUS "  cppzmq version: ${cppzmq_VERSION}")
message(STATUS "  Cereal version: ${cereal_VERSION}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")

