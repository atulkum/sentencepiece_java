cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(sentencepiece_java)

find_package(SWIG REQUIRED)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)
include(UseSWIG)
include_directories(${JNI_INCLUDE_DIRS})

set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR} )


include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(sentencepiece_git
    GIT_REPOSITORY https://github.com/google/sentencepiece
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_library (sentencepiece_interface_cpp SHARED sentencepiece_interface.cc)
set_property(TARGET sentencepiece_interface_cpp PROPERTY POSITION_INDEPENDENT_CODE ON)
add_dependencies (sentencepiece_interface_cpp sentencepiece_git)
target_link_libraries(sentencepiece_interface_cpp sentencepiece)

set_property(SOURCE sentencepiece_interface.i PROPERTY CPLUSPLUS ON)
swig_add_library (sentencepiece_interface TYPE SHARED LANGUAGE java SOURCES sentencepiece_interface.i)
set_property(TARGET sentencepiece_interface PROPERTY POSITION_INDEPENDENT_CODE ON)
swig_link_libraries(sentencepiece_interface sentencepiece_interface_cpp)


add_custom_command(
    TARGET sentencepiece_interface 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sentencepiece_interface> ${CMAKE_CURRENT_BINARY_DIR}
)

add_jar(
    SentencepieceInterfaceJNI
    SOURCES
    ${CMAKE_SWIG_OUTDIR}/sentencepiece_interface.java
    ${CMAKE_SWIG_OUTDIR}/sentencepiece_interfaceJNI.java
)
add_dependencies( SentencepieceInterfaceJNI sentencepiece_interface )



set(CMAKE_JAVA_JAR_ENTRY_POINT Test)
add_jar(
    Test
    SOURCES Test.java
    ENTRY_POINT Test
)
add_dependencies( Test SentencepieceInterfaceJNI )
