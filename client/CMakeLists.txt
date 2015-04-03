set(GFX_SRCS
    gfx.cpp
    draw.cpp
    gui/imgui.cpp)

set(PHYSICS_SRCS
    physics/world.cpp
    physics/body.cpp)

if (GRAPHICS_OK)
  add_library(gfx ${GFX_SRCS})
  target_link_libraries(gfx ${GLFW_STATIC_LIBRARIES})
endif()

if (GRAPHICS_OK AND PHYSICS_OK)
  message(STATUS "Building client ok!")
  add_library(client ${PHYSICS_SRCS})
  target_link_libraries(client gfx ${BOX2D_LIBRARIES})
endif()
