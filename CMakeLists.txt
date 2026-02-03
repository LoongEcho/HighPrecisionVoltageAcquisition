cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#
####################################################################################
##                                    setting
####################################################################################
# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME SYS_Power)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_TOOLCHAIN_FILE
	${CMAKE_SOURCE_DIR}/cmake/toolchain-arm-none-eabi.cmake
)
# Core project settings
project(${CMAKE_PROJECT_NAME})

message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# delete the redundancy configuration
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -g -fdata-sections -ffunction-sections")
# delete the function of not invoked
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")

####################################################################################
##                                    add executable
####################################################################################
# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

add_custom_command(TARGET SYS_Power POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:SYS_Power> $<TARGET_FILE:SYS_Power>.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:SYS_Power> $<TARGET_FILE:SYS_Power>.hex
    COMMENT "Generating BIN and HEX files from ELF"
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
	-T${CMAKE_SOURCE_DIR}/STM32F103XX_FLASH.ld
	-Wl,-Map=${CMAKE_PROJECT_NAME}.map
	)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

add_subdirectory(SYSTEM/sys)
add_subdirectory(SYSTEM/delay)
add_subdirectory(peripherals)
# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths

    
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    
)

# Add include paths

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} 
    stm32cubemx   
    sys
    delay
    peripherals

    # Add user defined libraries
)
