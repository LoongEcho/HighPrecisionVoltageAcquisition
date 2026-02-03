add_library(peripherals)

target_sources(peripherals
    PRIVATE
    oled.c
    font.c
    ad7190.c
)

target_include_directories(peripherals
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(peripherals
    PUBLIC
        stm32cubemx
        delay
)
