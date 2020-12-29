cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

##################
## Set Policies ##
##################

# Double colons must indicate an imported/alias target.
cmake_policy(SET CMP0028 NEW)

# Search prefixes specified by <PackageName>_ROOT.
cmake_policy(SET CMP0074 NEW)

######################
## Project Settings ##
######################

project("dv-commandline-utils" LANGUAGES CXX C)

set(${PROJECT_NAME}_VERSION_MAJOR "2020")
set(${PROJECT_NAME}_VERSION_MINOR "12")
set(${PROJECT_NAME}_VERSION_PATCH "29")
set(${PROJECT_NAME}_VERSION
  "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")

###################
## CMake Modules ##
###################

set(${PROJECT_NAME}_CMAKE_DIR ${${PROJECT_NAME}_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${${PROJECT_NAME}_CMAKE_DIR} ${CMAKE_MODULE_PATH})

include(dvInitializeBuildType)
include(dvPreventInSourceBuilds)
include(dvPreventInBuildInstalls)

#############
## Options ##
#############

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
#
#set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -O0 -fprofile-arcs -ftest-coverage" CACHE STRING "cxx debug compiler flags" FORCE)
#set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -O0 -fprofile-arcs -ftest-coverage" CACHE STRING "cxx debug compiler flags" FORCE)

##########################
## Third Party Packages ##
##########################

## RapidJSON

find_package(RapidJSON QUIET REQUIRED)

## Boost

find_package(Boost QUIET REQUIRED COMPONENTS
  program_options
  filesystem
  system
)

## ITK
## Generated from a script in ITK's source tree:
## Utilities/Maintenance/WhatModulesITK.py

find_package(ITK QUIET REQUIRED COMPONENTS
  ITKAnisotropicSmoothing
  ITKBinaryMathematicalMorphology
  ITKCommon
  ITKConnectedComponents
  ITKIOImageBase
  ITKIOMeshBase
  ITKIOXML
  ITKImageCompose
  ITKImageFunction
  ITKImageFusion
  ITKImageGrid
  ITKImageIntensity
  ITKImageStatistics
  ITKLabelMap
  ITKMathematicalMorphology
  ITKMesh
  ITKQuadEdgeMesh
  ITKQuadEdgeMeshFiltering
  ITKReview
  ITKTestKernel
  ITKThresholding
  ITKTransform
  ITKVtkGlue
  ITKImageIO
  ITKMeshIO
  Cuberille
  MeshNoise
  GenericLabelInterpolator
  IOMeshSTL
  SubdivisionQuadEdgeMeshFilter
  ITKDVUtilities
)

include(${ITK_USE_FILE})

## VTK
## Generated from a script in VTK's source tree:
## /path/to/vtk/src/Utilities/Maintenance/FindNeededModules.py \
##   -j /path/to/vtk/bin/modules.json \
##   -s /path/to/dv-commandline-utils/src

find_package(VTK
 COMPONENTS
    CommonColor
    CommonCore
    CommonDataModel
    CommonMath
    CommonTransforms
    FiltersCore
    FiltersExtraction
    FiltersGeneral
    FiltersModeling
    FiltersSources
    FiltersVerdict
    IOGeometry
    IOImage
    IOLegacy
    IOPLY
    IOXML
    ImagingCore
    InteractionStyle
    RenderingAnnotation
    RenderingCore
    RenderingFreeType # implements VTK::RenderingCore
    RenderingOpenGL2  # implements VTK::RenderingCore
    RenderingUI       # implements VTK::RenderingCore
)

#if(${APPLE})
#  find_package(Qt5 COMPONENTS
#               Core
#               Gui
#               Widgets)
#else()
#  find_package(Qt5 COMPONENTS
#               Core
#               Gui
#               Widgets
#               X11Extras)
#endif()

# CGAL

find_package(CGAL QUIET REQUIRED)

# Eigen

find_package(Eigen3 REQUIRED NO_MODULE)

# This library

file(GLOB SRC src/*.cxx)

add_library(${CMAKE_PROJECT_NAME} ${SRC})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/includes/>"
  "$<INSTALL_INTERFACE:include>"
  )
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${RapidJSON_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${ITK_LIBRARIES}
  ${VTK_LIBRARIES}
)

# Pull it all together...

add_library(${PROJECT_NAME}_dependencies INTERFACE)
target_link_libraries(${PROJECT_NAME}_dependencies INTERFACE
  Boost::program_options
  Boost::filesystem
  Boost::system
  ${ITK_LIBRARIES}
  ${VTK_LIBRARIES}
  ${PROJECT_NAME}
  CGAL::CGAL
  Eigen3::Eigen
  )

#####################
## Build Utilities ##
#####################

file(GLOB UTILS utils/*.cxx)

foreach(UTIL ${UTILS})

  get_filename_component(EXNAME ${UTIL} NAME_WE)
  add_executable(${EXNAME} ${UTIL})
  target_link_libraries(${EXNAME} ${PROJECT_NAME}_dependencies)

  vtk_module_autoinit(
    TARGETS ${EXNAME}
    MODULES ${VTK_LIBRARIES}
    )

endforeach()

########################################################
## Make the project discoverable by find_package(...) ##
########################################################

file(RELATIVE_PATH INSTALL_ROOT_REL_CONFIG_INSTALL_DIR
  "${dv-commandline-utils_BINARY_DIR}"
  "${dv-commandline-utils_SOURCE_DIR}")

export(
  TARGETS ${CMAKE_PROJECT_NAME}
  NAMESPACE ${CMAKE_PROJECT_NAME}::
  FILE "${${CMAKE_PROJECT_NAME}_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake"
)

export(PACKAGE ${CMAKE_PROJECT_NAME})

configure_file("${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake.in"
  "${${CMAKE_PROJECT_NAME}_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake" @ONLY)

#############
## Testing ##
#############

option(BUILD_TESTING "Build the testing tree." OFF)

include(CTest)

if(BUILD_TESTING)

  add_subdirectory(test)

endif()

