#Project version minimum
cmake_minimum_required(VERSION 3.16.4)
#Project name
project(RX671_MCR VERSION 1.0.0)
enable_language(C ASM)

#source files directorys
FILE (GLOB_RECURSE Source_Files ${CMAKE_CURRENT_SOURCE_DIR}
	"src/*.c" "src/*.cpp" "src/*.s" "src/*.S" "src/*.h" "inc/*.h"
	"generate/*.c" "generate/*.cpp" "generate/*.s" "generate/*.S" "generate/*.h"
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/smc_gen/general
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_pincfg
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_config
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_bsp
    ${CMAKE_SOURCE_DIR}/src/smc_gen
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_MTU1
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_TPU5
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_SCI10
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_MTU3
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_S12AD0
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_MTU2
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_SCI2
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_MTU7
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_PORT
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_SCI1
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_MTU4
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_MTU0
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_SCI0
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_ICU
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_S12AD1
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/src/smc_gen/Config_DMAC0
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_sdhi_rx
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_sdc_sdmem_rx
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_cmt_rx
	${CMAKE_SOURCE_DIR}/src/smc_gen/r_cmt_rx/src
	${CMAKE_SOURCE_DIR}/src/smc_gen/r_tfat_rx
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_tfat_rx/src
	${CMAKE_SOURCE_DIR}/src/smc_gen/r_sys_time_rx
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_sys_time_rx/src
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_tfat_driver_rx
    ${CMAKE_SOURCE_DIR}/src/smc_gen/r_tfat_driver_rx/src
    
    
)

SET(CMAKE_CXX_FLAGS "-std=gnu99 -ffunction-sections -fdata-sections -fdiagnostics-parseable-fixits -Wstack-usage=100 -g2 -misa=v3 -mlittle-endian-data -Og -fno-strict-aliasing")
SET(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
SET(CMAKE_ASM_FLAGS ${CMAKE_C_FLAGS} ${ASM_OPTIONS})
SET(CMAKE_H_COMPILER)
SET(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/linker_script.ld")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generate/linker_script.ld")
    SET(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/generate/linker_script.ld")
else()
    SET(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/linker_script.ld")
endif()
SET(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -Wl,--start-group -lm -lc -lgcc -Wl,--end-group -nostartfiles -Wl,-e_PowerON_Reset -v -Wl,--Map=${BuildArtifactFileBaseName}.map")

SET(ASM_FILES generate/start.S)
SET(ALL_FILES ${Source_Files})

add_executable(${PROJECT_NAME}.elf ${ALL_FILES})
target_link_libraries(${PROJECT_NAME}.elf m)
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${PROJECT_NAME}.bin POST_BUILD
COMMAND ${CMAKE_C_OBJCOPY} ARGS -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)
