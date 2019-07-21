cmake_minimum_required(VERSION 3.9.5 FATAL_ERROR)

######################
## Project Settings ##
######################

cmake_policy(SET CMP0028 NEW)

project(dv-commandline-utils CXX C)

set(dv-commandline-utils_VERSION_MAJOR "0")
set(dv-commandline-utils_VERSION_MINOR "0")
set(dv-commandline-utils_VERSION_PATCH "1")
set(dv-commandline-utils_VERSION
    "${dv-commandline-utils_VERSION_MAJOR}.${DVCppUtils_VERSION_MINOR}.${DVCppUtils_VERSION_PATCH}")

###################
## CMake Modules ##
###################

set(dv-commandline-utils_CMAKE_DIR ${dv-commandline-utils_SOURCE_DIR}/CMake)
set(CMAKE_MODULE_PATH ${dv-commandline-utils_CMAKE_DIR} ${CMAKE_MODULE_PATH})

include(DVInitializeBuildType)
include(DVPreventInSourceBuilds)
include(DVPreventInBuildInstalls)

#############
## Options ##
#############

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pg" CACHE STRING "cxx debug compiler flags" FORCE)

################################
## Utilities Depending on ITK ##
################################

find_package(VTK REQUIRED)

if(${APPLE})
  find_package(Qt5 COMPONENTS
               Core
               Gui
               Widgets)
else()
  find_package(Qt5 COMPONENTS
               Core
               Gui
               Widgets
               X11Extras)
endif()

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)

find_package(RapidJSON REQUIRED)
include_directories(
    "includes/"
    ${Boost_INCLUDE_DIRS}
    ${RapidJSON_INCLUDE_DIRS}
    )

##############################
## dv-commandline-utils-lib ##
##############################

file(GLOB SRC src/*.cxx)
add_library(dv-commandline-utils-lib ${SRC})
target_link_libraries(dv-commandline-utils-lib ${VTK_LIBRARIES})

set(EXNAMES "dv-convert-dicom-series-to-volume"
            "dv-convert-mesh"
            "dv-scale-mesh"
            "dv-translate-mesh"
            "dv-binarize-mesh"
            "dv-permute-axes-3d"
            "dv-convert-labels-to-mesh"
            "dv-decimate-mesh"
            "dv-subdivide-mesh-loop"
            "dv-add-mesh-noise"
            "dv-mesh-similarity-vertex-distance"
            "dv-mesh-area-volume"
            "dv-extract-interface-between-segments"
            "dv-smooth-mesh"
            "dv-assign-materials-to-mesh-faces"
   )

foreach(EXNAME ${EXNAMES})

  add_executable(${EXNAME} utils/${EXNAME}.cxx)
  target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

endforeach()

set(EXNAMES "dv-label-geometry"
            "dv-enforce-boundary-between-labels"
            "dv-anisotropic-smoothing"
            "dv-compare-image-information"
            "dv-convert-image"
            "dv-image-information"
            "dv-resample-volume"
            "dv-resample-from-reference"
            "dv-collapse-4d"
            "dv-flip-along-axes"
            "dv-change-information-3d"
            "dv-binary-threshold"
            "dv-extract-connected-components"
            "dv-fill-holes-in-binary-image"
            "dv-fill-holes-in-segmentation"
            "dv-binary-opening"
            "dv-segmentation-error"
            "dv-change-labels"
            "dv-extract-orthogonal-slice"
            "dv-read-dicom-tag"
            "dv-tile-images"
            "dv-window-image"
            "dv-overlay-labels"
            "dv-decimate-mesh-with-materials"
   )

foreach(EXNAME ${EXNAMES})

add_executable(${EXNAME} utils/${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

endforeach()

set(EXNAMES "dv-write-arrow-from-start-and-end"
            "dv-mesh-quality"
            "dv-plot-vectors"
   )

foreach(EXNAME ${EXNAMES})

  add_executable(${EXNAME} utils/${EXNAME}.cxx)
  target_link_libraries(${EXNAME} ${VTK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

  vtk_module_autoinit(
    TARGETS ${EXNAME}
    MODULES ${VTK_LIBRARIES}
    )
endforeach()

set(EXNAME "dv-visualize-meshes")
add_executable(${EXNAME} utils/${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      dv-commandline-utils-lib
                      ${VTK_LIBRARIES}
                      ${ITK_LIBRARIES}
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

vtk_module_autoinit(
  TARGETS dv-visualize-meshes
  MODULES ${VTK_LIBRARIES}
  )

##
## Utilities depending upon CPD
##

option(BUILD_CPD_UTILS "Build utilities depending upon CPD." OFF)

if(${BUILD_CPD_UTILS})

  find_package(Cpd REQUIRED)

  set(EXNAME "dv-nonrigid-cpd")
  add_executable(${EXNAME} utils/${EXNAME}.cxx)
  target_link_libraries(${EXNAME}
                        dv-commandline-utils-lib
                        Cpd::Library-C++
                        ${ITK_LIBRARIES}
                        ${Boost_PROGRAM_OPTIONS_LIBRARY}
                        ${Boost_FILESYSTEM_LIBRARY}
                        ${Boost_SYSTEM_LIBRARY}
                       )

endif()

#############
## Testing ##
#############

option(BUILD_TESTING "Build the testing tree." OFF)

include(CTest)

if(BUILD_TESTING)

  add_subdirectory(tests)

endif()
