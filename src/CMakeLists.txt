# cmake_minimum_required(VERSION 3.10)
# project(test)

# # 设置 C 标准
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# # 添加可执行文件

# add_library(parameter parameter.cpp)
# add_library(utils utils.cpp)
# # add_library(udp udp.cpp)
# add_library(reqrep reqrep.cpp)
# target_include_directories(parameter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# # target_include_directories(udp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_include_directories(reqrep PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add_executable(test test.cpp parameter.cpp utils.cpp reqrep.cpp)

# # 添加头文件搜索路径
# target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# # add_library(utils.cpp SHARED utils.cpp)#添加动态库文件
# find_package(nng REQUIRED)
# target_link_libraries(test nng::nng)#添加动态库文件
# target_compile_definitions(test PRIVATE NNG_ELIDE_DEPRECATED)#添加动态库文件
# target_link_libraries(test parameter)
# target_link_libraries(test utils)
# # target_link_libraries(test udp)
# target_link_libraries(test reqrep)


# ####bus#######

cmake_minimum_required(VERSION 3.10)
project(test)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 添加可执行文件

add_library(parameter parameter.cpp)
add_library(utils utils.cpp)
# add_library(udp udp.cpp)
add_library(reqrep reqrep.cpp)
add_library(bus bus.cpp)
target_include_directories(parameter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_include_directories(udp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(reqrep PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(bus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test test1.cpp parameter.cpp utils.cpp reqrep.cpp bus.cpp)

# 添加头文件搜索路径
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# add_library(utils.cpp SHARED utils.cpp)#添加动态库文件
find_package(nng REQUIRED)
target_link_libraries(test nng::nng)#添加动态库文件
target_compile_definitions(test PRIVATE NNG_ELIDE_DEPRECATED)#添加动态库文件
target_link_libraries(test parameter)
target_link_libraries(test utils)
# target_link_libraries(test udp)
target_link_libraries(test reqrep)
target_link_libraries(test bus)