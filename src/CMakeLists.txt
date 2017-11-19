cmake_minimum_required(VERSION 2.8.12)

project(dv-image-information)

set(CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "cxx compiler flags" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra" CACHE STRING "cxx debug compiler flags" FORCE)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "build type" FORCE)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(EXNAME "dv-convert-dicom-series-to-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-image")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-image-information")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-resample-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-resample-from-reference")
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

set(EXNAME "dv-collapse-4d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-permute-axes-3d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-flip-along-axes-3d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-change-information-3d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-binary-threshold")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-dicom-information")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-mesh-to-stl")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-mesh-to-stl-multiple")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-labels-to-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})
