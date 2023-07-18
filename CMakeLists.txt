cmake_minimum_required(VERSION 3.12)
project(omq)

set(CMAKE_CXX_STANDARD 11)

# 设置库的源文件
set(SRC_FILES
    src/bus.cpp
    src/parameter.cpp
    src/pickle.cpp
    src/reqrep.cpp
    src/udp.cpp
    src/utils.cpp
    src/omq.cpp
)
find_package(nng REQUIRED)
target_link_libraries(omq nng::nng)#添加动态库文件
target_compile_definitions(omq PRIVATE NNG_ELIDE_DEPRECATED)#添加动态库文件)

include_directories(include)

# 生成动态链接库
add_library(omq SHARED ${SRC_FILES})

# 示例代码可执行文件
add_executable(test test/test.cpp)

# 链接动态链接库
target_link_libraries(test omq)

# # ####bus#######

# cmake_minimum_required(VERSION 3.10)
# project(omq)

# # 设置 C 标准
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# # 添加可执行文件
# set(src ${CMAKE_CURRENT_SOURCE_DIR}/src)
# set(include ${CMAKE_CURRENT_SOURCE_DIR}/include)
# set (test ${CMAKE_CURRENT_SOURCE_DIR}/test)

# # 设置文件位置
# message("src:${src}")
# include_directories(${include})

# add_library(parameter ${src}/parameter.cpp)
# target_include_directories(parameter PUBLIC ${include})

# add_library(utils ${src}/utils.cpp)
# add_library(pickle ${src}/pickle.cpp)
# add_library(reqrep ${src}/reqrep.cpp ${src}/parameter.cpp)
# add_library(bus ${src}/bus.cpp ${src}/parameter.cpp)
# add_library(udp ${src}/udp.cpp ${src}/parameter.cpp)

# target_include_directories(pickle PUBLIC ${include})
# target_include_directories(utils PUBLIC ${include})
# # target_include_directories(udp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_include_directories(reqrep PUBLIC ${include})
# target_include_directories(bus PUBLIC ${include})
# target_include_directories(udp PUBLIC ${include})

# # 查找并导入 Python 库
# find_package(Python REQUIRED COMPONENTS Interpreter Development)

# # 添加包含目录和链接库
# include_directories(${Python_INCLUDE_DIRS})
# link_libraries(${Python_LIBRARIES})
# add_executable(test ${src}/bus.cpp ${src}/parameter.cpp ${src}/reqrep.cpp ${src}/utils.cpp ${test}/test.cpp ${src}/pickle.cpp ${src}/udp.cpp)

# # 添加头文件搜索路径
# target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# # add_library(utils.cpp SHARED utils.cpp)#添加动态库文件
# find_package(nng REQUIRED)
# target_link_libraries(test nng::nng)#添加动态库文件
# target_compile_definitions(test PRIVATE NNG_ELIDE_DEPRECATED)#添加动态库文件)
# target_link_libraries(udp parameter)
# target_link_libraries(bus parameter)
# target_link_libraries(test parameter)
# target_link_libraries(test utils)
# # target_link_libraries(test udp)
# target_link_libraries(test reqrep)
# target_link_libraries(test bus)
# target_link_libraries(test pickle)
# target_link_libraries(test udp)

