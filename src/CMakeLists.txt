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

##############################
## dv-commandline-utils-lib ##
##############################

file(GLOB SRC src/*.cxx)

include_directories("includes/")
add_library(dv-commandline-utils-lib ${SRC})

################################
## Utilities Depending on ITK ##
################################

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#set(EXNAME "dv-convert-dicom-series-to-volume")
#add_executable(${EXNAME} ${EXNAME}.cxx)
#target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-scale-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-translate-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-binarize-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-permute-axes-3d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-labels-to-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-decimate-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-subdivide-mesh-loop")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-add-mesh-noise")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-mesh-similarity-vertex-distance")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-mesh-area-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-extract-interface-between-segments")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-smooth-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-enforce-boundary-between-labels")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-anisotropic-smoothing")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-compare-image-information")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-convert-image")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-image-information")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-resample-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-resample-from-reference")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-collapse-4d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-flip-along-axes")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-change-information-3d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-binary-threshold")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-extract-connected-components")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-fill-holes-in-binary-image")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-binary-opening")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-segmentation-error")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-change-labels")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-extract-orthogonal-slice")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-read-dicom-tag")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                     )

set(EXNAME "dv-tile-images")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                    )
set(EXNAME "dv-window-image")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                    )
set(EXNAME "dv-overlay-labels")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME}
                      ${ITK_LIBRARIES}
                      dv-commandline-utils-lib
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                    )

##
## Utilities depending upon CPD
##

option(BUILD_CPD_UTILS "Build utilities depending upon CPD." OFF)

if(${BUILD_CPD_UTILS})

  find_package(Cpd REQUIRED)

  set(EXNAME "dv-nonrigid-cpd")
  add_executable(${EXNAME} ${EXNAME}.cxx)
  target_link_libraries(${EXNAME}
                        dv-commandline-utils-lib
                        Cpd::Library-C++
                        ${ITK_LIBRARIES}
                        ${Boost_PROGRAM_OPTIONS_LIBRARY}
                        ${Boost_FILESYSTEM_LIBRARY}
                        ${Boost_SYSTEM_LIBRARY}
                       )

endif()

##
## Utilities depending upon VTK
##

option(BUILD_VTK_UTILS "Build utilities depending upon VTK." OFF)

if(${BUILD_VTK_UTILS})

  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})

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

  set(EXNAME "dv-mesh-quality")
  add_executable(${EXNAME} ${EXNAME}.cxx)
  target_link_libraries(${EXNAME} ${VTK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

  set(EXNAME "dv-plot-vectors")
  add_executable(${EXNAME} ${EXNAME}.cxx)
  target_link_libraries(${EXNAME} ${VTK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

  set(EXNAME "dv-visualize-meshes")
  add_executable(${EXNAME} ${EXNAME}.cxx)
  target_link_libraries(${EXNAME}
                        dv-commandline-utils-lib
                        ${VTK_LIBRARIES}
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
