add_executable(money_saver main.cpp game.cpp)
target_link_libraries(money_saver dl)
set_target_properties(money_saver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)
file(COPY terrain.map DESTINATION ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)

add_library(saverTemplate SHARED agentTemplate.cpp)
set_target_properties(saverTemplate PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)

add_library(thiefTemplate SHARED agentTemplate.cpp)
set_target_properties(thiefTemplate PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)

if (GRAPHICS_OK)
  add_executable(money_saver_gfx game.cpp gfx.cpp)
  target_link_libraries(money_saver_gfx dl gfx)
  set_target_properties(money_saver_gfx PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)
endif()
