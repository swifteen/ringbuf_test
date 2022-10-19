cmake_minimum_required(VERSION 3.1.0)
#project(test_ring_buffer VERSION 1.0.0 LANGUAGES CXX)
SET(CMAKE_BUILD_TYPE "Debug")


#这里实际上可以不必包含头文件，主要是方便IDE包含头文件
add_executable(test_ring_buffer
	ring_buffer.h
	ring_buffer.c
	test_ring_buffer.c
)
target_link_libraries(test_ring_buffer pthread)
#add_compile_options(-O0  -g -ggdb)

#target_include_directories(dcmtool PUBLIC ./dicomtk/)

add_executable(test_ringbuf
	ringbuf.h
	ringbuf.c
	test_ringbuf.c
)
target_link_libraries(test_ringbuf pthread)
set(CMAKE_CXX_FLAGS "-O0  -g -ggdb ${CMAKE_CXX_FLAGS}")

#链接Qt库
#target_link_libraries(dcmtool  PRIVATE zqecg_rw log4c dcmdata dcmnet ofstd oflog dcmsr dcmimage dcmimgle cmr i2d Qt5::Widgets Qt5::PrintSupport)



#安装目标
#install(TARGETS ${BIN_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/dicom)