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
find_package(nng REQUIRED)

# find_package(nng REQUIRED)
# target_link_libraries(omq PRIVATE nng::nng)  # 链接nng库
# target_compile_definitions(omq PRIVATE NNG_ELIDE_DEPRECATED)
# 示例代码可执行文件
add_executable(test test/test.cpp)
# 链接动态链接库
target_link_libraries(test  omq)

find_package(nng REQUIRED)
target_link_libraries(test nng::nng)#添加动态库文件
target_compile_definitions(test PRIVATE NNG_ELIDE_DEPRECATED)#添加动态库文件)

install(TARGETS omq
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        PUBLIC_HEADER DESTINATION include)
install(FILES include/omq.h include/bus.h include/parameter.h 
        include/pickle.h include/reqrep.h include/udp.h include/utils.h 
        DESTINATION include/omq)

