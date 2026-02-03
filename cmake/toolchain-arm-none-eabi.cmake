set (CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(MCPU "-mcpu=cortex-m3")
set(MARCH "")
set(MFLOAT "-mfloat-abi=soft")

set(COMMON_FLAGS
	"${MCPU} ${MARCH} -mthumb ${MFLOAT}"
)

set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS_INIT
	"${COMMON_FLAGS} -Wl,--gc-sections"
)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)



