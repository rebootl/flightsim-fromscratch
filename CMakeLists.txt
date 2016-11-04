cmake_minimum_required(VERSION 3.6.2)

project(myprog)

# find SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)

find_package(OpenGL REQUIRED)

# find osg (OpenSceneGraph)
find_package(osg)
find_package(osgViewer)
find_package(osgDB)
find_package(osgGA)

# include JSBSim
# (needed for the linker to find it)
include_directories(JSBSim/src)

# build my program
add_executable( myprog myprog.cpp
                FDMInterface.cpp FDMInterface.h
                Model.cpp Model.h
                InputInterface.cpp InputInterface.h )

# build JSBSim
set(LIBRARY_VERSION 1)
add_subdirectory(JSBSim/src)

# link JSBSim
target_link_libraries(myprog libJSBSim)

# include and link SFML
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(myprog ${SFML_LIBRARIES})

include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(myprog ${OPENGL_LIBRARIES})

# include and link OSG by macro (needed)

macro( config_project PROJNAME LIBNAME )
    include_directories( ${${LIBNAME}_INCLUDE_DIR} )
    target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()

config_project(myprog OSG)
config_project(myprog OSGVIEWER)
config_project(myprog OSGDB)
config_project(myprog OSGGA)
