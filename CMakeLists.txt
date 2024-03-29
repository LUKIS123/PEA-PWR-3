cmake_minimum_required(VERSION 3.27)
project(PEA_PWR_3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -fno-rtti")

set(SOURCE_FILES main.cpp)

add_executable(PEA_PWR_3 main.cpp
        Matrix/ATSPMatrix.h
        Matrix/ATSPMatrix.cpp
        MeasureTime/Timer.h
        MeasureTime/Timer.cpp
        FileUtils/DataFileUtility.h
        FileUtils/DataFileUtility.cpp
        RandomDataGenerator/RandomDataGenerator.h
        RandomDataGenerator/RandomDataGenerator.cpp
        Solution/GeneticAlgorithm.h
        Solution/GeneticAlgorithm.cpp
        Flow/ActionResult.h
        Flow/AppController.h
        Flow/AppController.cpp
        Flow/ConsoleView.h
        Flow/ConsoleView.cpp
        Flow/Enums.h
        Utils/GreedyAlgorithm.h
        Utils/GreedyAlgorithm.cpp
        Utils/GASubject.h
        Utils/GASubject.cpp
        Utils/CrossoverMethods.h
        Utils/CrossoverMethods.cpp)
