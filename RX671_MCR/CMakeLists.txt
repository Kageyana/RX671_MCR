#Project version minimum
cmake_minimum_required(VERSION 3.16.4)
#Project name
project(RX671_MCR VERSION 1.0.0)
enable_language(C ASM)

#source files directorys
FILE (GLOB_RECURSE Source_Files ${CMAKE_CURRENT_SOURCE_DIR}
	"src/*.c" "src/*.cpp" "src/*.s" "src/*.S" "src/*.h"
	"generate/*.c" "generate/*.cpp" "generate/*.s" "generate/*.S" "generate/*.h"
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/generate
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/smc_gen/general
    ${CMAKE_CURRENT_SOURCE_DIR}/src/smc_gen/r_pincfg
    ${CMAKE_CURRENT_SOURCE_DIR}/src/smc_gen/r_config
    ${CMAKE_CURRENT_SOURCE_DIR}/src/smc_gen/r_bsp
)

SET(CMAKE_CXX_FLAGS "-ffunction-sections -fdata-sections -fdiagnostics-parseable-fixits -Wstack-usage=100 -g2 -misa=v3 -mlittle-endian-data")
SET(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
SET(CMAKE_ASM_FLAGS ${CMAKE_C_FLAGS} ${ASM_OPTIONS})
SET(CMAKE_H_COMPILER)
SET(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/linker_script.ld")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generate/linker_script.ld")
    SET(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/generate/linker_script.ld")
else()
    SET(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/linker_script.ld")
endif()
SET(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -Wl,-Map=${PROJECT_NAME}.map -Wl,--start-group -Wl,--end-group -nostartfiles -Wl,-e_PowerON_Reset")

SET(ASM_FILES generate/start.S)
SET(ALL_FILES ${Source_Files})

add_executable(${PROJECT_NAME}.elf ${ALL_FILES})
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${PROJECT_NAME}.bin
COMMAND ${CMAKE_C_OBJCOPY} ARGS -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)
