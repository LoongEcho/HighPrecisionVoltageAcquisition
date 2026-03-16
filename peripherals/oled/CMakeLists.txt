add_library(oled)

target_sources(oled
    PRIVATE
    oled.c
    font.c
)

target_include_directories(oled
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(oled
    PUBLIC
        stm32cubemx
        delay
)
