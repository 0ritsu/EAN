cmake_minimum_required(VERSION 3.20)
project(EAN LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
  set(QT_LIBS Qt6::Widgets)
else()
  find_package(Qt5 COMPONENTS Widgets REQUIRED)
  set(QT_LIBS Qt5::Widgets)
endif()

add_executable(EAN
  src/main.cpp
  src/gui/MainWindow.h
  src/gui/MainWindow.cpp
  src/gui/MainWindow.ui
  src/core/Parser.h
  src/core/Parser.cpp
  src/core/CholeskySolver.h
  src/core/CholeskySolver.cpp
  src/core/IntervalSolver.h
  src/core/IntervalSolver.cpp
  src/core/IterationLogger.h
  src/core/IterationLogger.cpp
)

target_include_directories(EAN PRIVATE src)

target_link_libraries(EAN PRIVATE ${QT_LIBS})
