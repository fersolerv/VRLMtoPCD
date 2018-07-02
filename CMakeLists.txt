PROJECT(VRMLtoPCD)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(VRMLtoPCD)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Modules/" ${CMAKE_MODULE_PATH})

find_package(Eigen REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${EIGEN_INCLUDE_DIRS})
add_definitions(-DEIGEN_USE_NEW_STDVECTOR
                 -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET)
                 
find_package( PugiXML )

find_package( Coin REQUIRED )

find_package( Kautham )
INCLUDE_DIRECTORIES(SYSTEM ${KAUTHAM_INCLUDE_DIRS})
                 
find_package(Qhull)

find_package(PCL 1.8 REQUIRED)
if (NOT PCL_SHARED_LIB OR WIN32)
set(QHULL_USE_STATIC ON)
endif(NOT PCL_SHARED_LIB OR WIN32)

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/include)
                     
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

message(STATUS "PCL_LIBRARY_DIRSc(${PCL_LABRARY_DIRS})")

SET(SRC src/VRMLtoPCD.cpp )
        
#SET(INCLUDES include/ContactConeGenerator.h
#             include/ConvexHull.h 
#             include/Mtools.h)
        
add_executable(VRMLtoPCD ${SRC})

target_link_libraries(VRMLtoPCD ${QHULL_LIBRARIES}
                                             ${KAUTHAM_LIBRARIES}
                                             ${COIN_LIBRARY}
                                             ${PUGI_XML_LIBRARIES}
                                             ${PCL_LIBRARIES}
                                             ${PCL_IO_LIBRARIES})
