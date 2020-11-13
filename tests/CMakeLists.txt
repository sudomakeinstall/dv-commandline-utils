######################################
# Extra libraries needed for testing #
######################################

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

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

find_package(CGAL QUIET COMPONENTS)

###########
## Tests ##
###########

set(EXNAMES "dvAccelerationRegularizerTest"
            "dvGetListOfColorsTest"
            "dvCameraStateTest"
            "dvEdgeLengthRegularizerTest"
            "dvStringOperationsTest"
            "dvVectorOperationsTest"
            "dvMathTest"
            "dvProgressTest"
            "dvJoinTest"
            "dvCycleTest"
            "dvGlobFilesTest"
            "dvCyclicMeanTest"
            "dvXORSwapTest"
            "dvReverseFindIfTest"
            "dvSoftMaxTest"
            "dvDeleteIsolatedPointsTest"
            "dvMeshToQuadEdgeMeshTest"
            "dvVisualizeMeshesInteractorStyleTest"
            "dvVisualizeMeshesFrameTest"
            "dvMeshViewTest"
            "dvTextViewTest"
            "dvSegmentationViewTest"
            "dvSegmentationToLabeledPointSetTest"
            "dvLabeledITKPointSetWriterTest"
            "dvLabeledITKPointSetReaderTest"
            "dvLabeledVTKPointSetReaderTest"
            "dvLabeledITKPointSetToPointSetMapTest"
            "dvITKTriangleMeshToVTKPolyDataTest"
            "dvSubdivisionCellDataTest"
            "dvAdditiveGaussianNoiseMeshFilterCellDataTest"
            "dvDelaunayConformingQuadEdgeMeshFilterCellDataTest"
            "dvDecimationQuadEdgeMeshFilterCellDataTest"
            "dvCuberilleQuadEdgeMeshTest"
            "dvPolyDataWriterCellDataTest"
            "dvVTKPolyDataToITKTriangleMeshTest"
            "dvRefineThenLoopTest"
            "dvCuberilleQEMeshSharedEdgeTest"
            "dvExtractConnectedComponentsTest"
   )

foreach(EXNAME ${EXNAMES})

  add_executable(${EXNAME} src/${EXNAME}.cxx)
  target_link_libraries(${EXNAME} dv-commandline-utils-lib ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${CERES_LIBRARIES})
  add_test(NAME ${EXNAME} COMMAND ${EXNAME} ${CMAKE_SOURCE_DIR}/tests/testdata/)

  vtk_module_autoinit(
    TARGETS ${EXNAME}
    MODULES ${VTK_LIBRARIES}
    )

endforeach()


if (CGAL_FOUND)

  set(EXNAME "dvGenerateInitialModelTest")
  add_executable(${EXNAME} src/${EXNAME}.cxx)
  target_link_libraries(${EXNAME}
    dv-commandline-utils-lib
    CGAL::CGAL
    ${ITK_LIBRARIES}
    ${VTK_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${CERES_LIBRARIES})
  add_test(NAME ${EXNAME} COMMAND ${EXNAME} ${CMAKE_SOURCE_DIR}/tests/testdata/)

  vtk_module_autoinit(
    TARGETS ${EXNAME}
    MODULES ${VTK_LIBRARIES}
    )

endif()
