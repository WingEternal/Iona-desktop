target_sources(${APP_NAME} 
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/app_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/app_msg_handler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/app_native_ev_filter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/app_widget_container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/app_config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app_msg_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app_native_ev_filter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app_widget_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app_thread_helper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    
)
