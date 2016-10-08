cmake_minimum_required(VERSION 3.6)
project(Neural_Network)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    demo-source/Demo.cpp
#    research-run-source/Research-Run.cpp
#    research-run-source/sha256.cpp
#    research-run-source/sha256.h
    Color.h
    DLog.h
    Layer.h
    LayerBoss.h
    NetTemplates.h
    Neuron.h
    NeuronTypes.h
    POSI.h
    TestData.h
    Utility.cpp
    Utility.h)

add_executable(Neural_Network ${SOURCE_FILES})