cmake_minimum_required(VERSION 3.5)
project(msys)

#========================================

set(DEVICE atxmega128a1)
set(FREQ 32000000UL)

#========================================

set(CMAKE_C_COMPILER /opt/local/bin/avr-gcc)

set(CMAKE_C_FLAGS "-Wall -gdwarf-2 -DF_CPU=${FREQ} -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -mmcu=${DEVICE}")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--start-group -Wl,-lm -Wl,--end-group -Wl,-L${BLUEOS_SOURCE_PATH} -Wl,--gc-sections -mmcu=${DEVICE}")
include_directories(src/BlueOS/config src/BlueOS/source)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")

#========================================

aux_source_directory(src sources)
aux_source_directory(src/BlueOS/source blueOSSources)
set(SOURCE_FILES ${sources} ${blueOSSources})

#========================================

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex)
