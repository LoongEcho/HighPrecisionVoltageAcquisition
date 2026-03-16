add_library(ad7190)

target_sources(ad7190
    PRIVATE
    ad7190.c
)

target_include_directories(ad7190
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ad7190
    PUBLIC
        stm32cubemx
        delay
)
