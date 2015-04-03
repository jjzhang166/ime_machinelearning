set(SRCS
    gfx.cpp
    draw.cpp
    physics/world.cpp
    physics/body.cpp)
  add_library(gfxclient ${SRCS})
target_link_libraries(gfxclient ${GLFW_STATIC_LIBRARIES} ${BOX2D_LIBRARIES})
