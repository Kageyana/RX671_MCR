set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR GCCRX)

# Set the environment root directory
# It can be used to specify the target environment location
# e.g compiler's location. This variable is most useful when crosscompiling.
# (Should avoid spaces in the path or have to escape them)
SET(CMAKE_FIND_ROOT_PATH "C:/ProgramData/GCC for Renesas RX 8.3.0.202411-GNURX-ELF/rx-elf/rx-elf/bin")

if (NOT CMAKE_FIND_ROOT_PATH)
  message(WARNING "Toolchain path not defined.\n"
                  "Builds scripts will search toolchain binaries in environment paths.\n"
                  "Use CMAKE_FIND_ROOT_PATH variable to set a specific toolchain folder.")
elseif (NOT CMAKE_FIND_ROOT_PATH MATCHES "/$")
  set(CMAKE_FIND_ROOT_PATH "${CMAKE_FIND_ROOT_PATH}/")
endif()

if (CMAKE_HOST_WIN32 )
  SET(BINARY_FILE_EXT ".exe")
else()
  SET(BINARY_FILE_EXT "")
endif()

# CMake variables for compiler, assembler, native build system
# Specify the C compiler
set(CMAKE_C_COMPILER ${CMAKE_FIND_ROOT_PATH}rx-elf-gcc${BINARY_FILE_EXT})
set(CMAKE_CXX_COMPILER ${CMAKE_FIND_ROOT_PATH}rx-elf-gcc${BINARY_FILE_EXT})
set(CMAKE_SYSROOT ${sysroot_target})
set(CMAKE_C_OBJCOPY ${CMAKE_FIND_ROOT_PATH}rx-elf-objcopy${BINARY_FILE_EXT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
