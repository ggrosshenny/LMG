# Check CMAKE version
cmake_minimum_required( VERSION 3.0 )

project(LMG_project LANGUAGES C CXX)

if(MSVC)
  add_definitions("-D_USE_MATH_DEFINES")
else()
  add_compile_options(-std=c++11 -Wall -I ./SOIL/src -L ./SOIL/lib/libSOIL.a)
endif()

##################################################################################
# Package Management
##################################################################################

# OpenGL
find_package( OpenGL REQUIRED )
#find_package( GLUT REQUIRED )
find_package( GLUT )
#find_package( GLEW REQUIRED )
find_package( GLEW )
# SOIL
find_library(SOIL SOIL lib/libSOIL.a)

#OpenGL
if(NOT ${OPENGL_FOUND})
    message("OPENGL not found")
else()
  include_directories( ${OPENGL_INCLUDE_DIRS} )
endif()

# freeglut
if(NOT ${GLUT_FOUND})
    message("GLUT not found")
  include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/freeglut/include" )
else()
  include_directories( ${GLUT_INCLUDE_DIRS} )
endif()

# glew
if(NOT ${GLEW_FOUND})
    message("GLEW not found")
  include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glew/include" )
else() 
  include_directories( ${GLEW_INCLUDE_DIRS} )
endif()

# glm
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/glm" )

# Retrieve source files
file( GLOB incList "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
file( GLOB inlList "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" )
file( GLOB srcList "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
file( GLOB SoilInc "${CMAKE_CURRENT_SOURCE_DIR}/SOIL/src/*.h" )

# Group files in IDE (Visual Studio)
source_group( "Include" FILES ${incList} )
source_group( "Inline" FILES ${inlList} )
source_group( "Source" FILES ${srcList} )

# Target program
set( resList ${resList} ${SoilInc} )
set( resList ${resList} ${incList} )
set( resList ${resList} ${inlList} )
add_executable( ${PROJECT_NAME} ${srcList} ${resList} )

##################################################################################
# Linked libraries
##################################################################################

# Graphics

target_link_libraries( ${PROJECT_NAME} ${OPENGL_gl_LIBRARY} )
#OPENGL_LIBRARIES

if(NOT ${GLUT_FOUND})
  target_link_libraries( ${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/freeglut/lib/x64/freeglut.lib" )
else()
  target_link_libraries( ${PROJECT_NAME} ${GLUT_glut_LIBRARY} )
  #GLUT_LIBRARIES
endif()

if(NOT ${GLEW_FOUND})
  target_link_libraries( ${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glew/lib/Release/x64/glew32.lib" )
else()
  target_link_libraries( ${PROJECT_NAME} ${GLEW_LIBRARIES} )
endif()

target_link_libraries( ${PROJECT_NAME} ${SOIL} )
#SOIL_LIBRARIES
