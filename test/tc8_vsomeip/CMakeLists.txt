cmake_minimum_required(VERSION 2.8)



project(tc8vsomeip)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x")


SET(VSOMEIP_RUNTIME_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include_directories(
    ${VSOMEIP_RUNTIME_PATH}/out/include/
)
link_directories(
    ${VSOMEIP_RUNTIME_PATH}/out/lib/
)



add_executable(tc8vsomeip
	NESomeIPTc8Server.cpp
	NESomeIPTc8Process.cpp
)

target_link_libraries(tc8vsomeip vsomeip3)


add_executable(tc8daemon
	Tc8Server.cpp
)

