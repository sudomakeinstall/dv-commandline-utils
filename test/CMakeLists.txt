file(GLOB MYTESTS src/*.cxx)

foreach(MYTEST ${MYTESTS})

  get_filename_component(EXNAME ${MYTEST} NAME_WE)
  add_executable(${EXNAME} ${MYTEST})
  target_link_libraries(${EXNAME}
    ${PROJECT_NAME}_dependencies
    )

vtk_module_autoinit(
  TARGETS ${EXNAME}
  MODULES ${VTK_LIBRARIES}
  )

endforeach()
