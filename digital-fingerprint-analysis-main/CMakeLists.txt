cmake_minimum_required(VERSION 3.22)
project(Projet)

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-O3" )

INCLUDE_DIRECTORIES(/include)
LINK_DIRECTORIES(/lib)

ADD_EXECUTABLE(starter1 test/starter1_test.cpp src/starter1.cpp)
ADD_EXECUTABLE(main1_simu test/main1_simu_test.cpp src/main1_simu.cpp src/main1_rest.cpp src/starter1.cpp)
ADD_EXECUTABLE(main1_rest test/main1_rest_test.cpp src/main1_rest.cpp src/starter1.cpp)
ADD_EXECUTABLE(main3_simu test/main3_simu_test.cpp src/main3_simu.cpp src/starter3.cpp src/starter1.cpp include/part3.hpp)
ADD_EXECUTABLE(main3_rest test/main3_rest_test.cpp src/main3_rest.cpp src/starter1.cpp)

add_definitions(-DOPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)
find_package(OpenCV REQUIRED)
target_link_libraries(starter1 ${OpenCV_LIBS})
target_link_libraries(main1_simu ${OpenCV_LIBS})
target_link_libraries(main1_rest ${OpenCV_LIBS})
target_link_libraries(main3_simu ${OpenCV_LIBS})
target_link_libraries(main3_rest ${OpenCV_LIBS})