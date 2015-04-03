macro (add_agent agent)
  add_library(${agent} SHARED ${ARGN})
  set_target_properties(${agent} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)
endmacro()

add_agent(saverTemplate template.cpp)
add_agent(thiefTemplate template.cpp)
