cmake_minimum_required(VERSION 3.12)
project(omq)

set(CMAKE_CXX_STANDARD 11)

# 设置库的源文件
set(SRC_FILES
    src/parameter.cpp
    src/pickle.cpp
    src/reqrep.cpp
    src/bus.cpp
    src/udp.cpp
    src/utils.cpp
    src/omq.cpp
)
find_package(nng REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)

link_libraries(${Python_LIBRARIES})
include_directories(include)
include_directories(${Python_INCLUDE_DIRS})
# 生成动态链接库
add_library(omq SHARED ${SRC_FILES})
# 示例代码可执行文件
add_executable(test test/test1.cpp)
# 链接动态链接库
target_link_libraries(test omq)
find_package(nng REQUIRED)
target_link_libraries(test nng::nng)#添加动态库文件
target_compile_definitions(test PRIVATE NNG_ELIDE_DEPRECATED)#添加动态库文件)

