add_library(sys)

target_sources(sys
    PRIVATE
        sys.c
)

target_include_directories(sys
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sys
    PUBLIC stm32cubemx 
)

