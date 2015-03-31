add_library(gfxclient gfx.cpp body.cpp world.cpp draw.cpp)
target_link_libraries(gfxclient ${GLFW_STATIC_LIBRARIES} ${BOX2D_LIBRARIES})
