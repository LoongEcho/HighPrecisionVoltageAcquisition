add_library(delay)

target_sources(delay
    PRIVATE
        delay.c
)

target_include_directories(delay
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(delay
    PUBLIC 
        stm32cubemx
        sys
)
