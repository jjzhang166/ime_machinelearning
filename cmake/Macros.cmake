macro(add_example target source)
  add_executable(${target} ${source})
  target_link_libraries(${target} ${ARGN})
  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/examples)
endmacro(add_example)

