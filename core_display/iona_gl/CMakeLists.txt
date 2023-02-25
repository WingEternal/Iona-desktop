target_sources(${APP_NAME} 
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_handle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_handle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_object_base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_widget.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/l2d.h
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_model.h
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_tex_manager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_tex_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_touch_manager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_touch_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_allocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/l2d_pal.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/data_ring.h
    ${CMAKE_CURRENT_SOURCE_DIR}/data_ring.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/data_ring_model.cpp 
)
