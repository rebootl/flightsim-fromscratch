cmake_minimum_required(VERSION 3.6.2)

project(myprog)

# include osg (OpenSceneGraph)
find_package(osg)
find_package(osgViewer)
find_package(osgDB)
find_package(osgGA)

# build my program
add_executable(myprog myprog.cpp)

# build JSBSim
set(LIBRARY_VERSION 1)
add_subdirectory(JSBSim/src)

# include and link JSBSim
include_directories(JSBSim/src)

target_link_libraries(myprog libJSBSim)

# include and link OSG by macro (needed)

macro( config_project PROJNAME LIBNAME )
    include_directories( ${${LIBNAME}_INCLUDE_DIR} )
    target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()

config_project(myprog OSG)
config_project(myprog OSGVIEWER)
config_project(myprog OSGDB)
config_project(myprog OSGGA)
