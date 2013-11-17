#==============================================================================
# generate_avr_firmware(
#       name
#       [MCU avr_mcu_name]
#       [FCPU cpu_frequency]
#       [SRCS src1 src2 .. srcN]
#       [LIBS lib1 lib2 .. libN]        # optional
#   )
#==============================================================================
# generate_avr_library(
#       library_name
#       [MCU avr_mcu_name]
#       [FCPU cpu_frequency]
#       [SRCS src1 src2 .. srcN]
#       [LIBS lib1 lib2 .. libN]        # optional
#   )
#==============================================================================

cmake_minimum_required(VERSION 2.8.5)
include(CMakeParseArguments)

SET(AVR_COMPILE_FLAGS "-Os -std=gnu99 -Wall -Wstrict-prototypes -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(AVR_LINK_FLAGS "-lm")


macro(GENERATE_PARSE_ARGUMENTS ARGUMENTS)

    message(STATUS "Parsing arguments: ${ARGUMENTS}")

    cmake_parse_arguments(INPUT             # prefix
                          ""                # Options 
                          "MCU;FCPU"        # One value keywords 
                          "SRCS;LIBS"       # Multi Value Keywords
                          ${ARGUMENTS}           # Arguments to parse
                          )

    message(STATUS "MCU: ${INPUT_MCU}")
    message(STATUS "FCPU: ${INPUT_FCPU}")
    message(STATUS "SRCS: ${INPUT_SRCS}")
    message(STATUS "LIBS: ${INPUT_LIBS}")
    message(STATUS "Unexpected options: ${INPUT_UNPARSED_ARGUMENTS}")

    if(NOT INPUT_MCU)
        message(FATAL_ERROR "MCU not set")
    endif()

    if(NOT INPUT_FCPU)
        message(FATAL_ERROR "FCPU not set")
    endif()

    if(NOT INPUT_SRCS)
        message(FATAL_ERROR "SRCS not set")
    endif()

endmacro()

function(GENERATE_AVR_FIRMWARE INPUT_NAME)
    message(STATUS "Generating AVR firmware ${INPUT_NAME}")

    generate_parse_arguments("${ARGN}") 
    
    add_executable(${INPUT_NAME} ${INPUT_SRCS})

    if(INPUT_LIBS)
        message(STATUS "Linking libraries: ${INPUT_LIBS}")
        target_link_libraries(${INPUT_NAME} ${INPUT_LIBS})
    endif()
    
    set_target_properties(${INPUT_NAME} PROPERTIES
        COMPILE_FLAGS "${AVR_COMPILE_FLAGS} -mmcu=${INPUT_MCU} -DF_CPU=${INPUT_FCPU}")
        
    set_target_properties(${INPUT_NAME} PROPERTIES
        LINK_FLAGS "${AVR_COMPILE_FLAGS} ${AVR_LINK_FLAGS} -mmcu=${INPUT_MCU} -DF_CPU=${INPUT_FCPU}")

    setup_programmer(${INPUT_MCU})
endfunction()

function(GENERATE_AVR_LIBRARY INPUT_NAME)
    message(STATUS "Generating AVR library ${INPUT_NAME}")
      
    generate_parse_arguments("${ARGN}") 

    add_library(${INPUT_NAME} STATIC ${INPUT_SRCS})

    if(INPUT_LIBS)
        message(STATUS "Linking libraries: ${INPUT_LIBS}")
        target_link_libraries(${INPUT_NAME} ${INPUT_LIBS})
    endif()

    set_target_properties(${INPUT_NAME} PROPERTIES
        COMPILE_FLAGS "${AVR_COMPILE_FLAGS} -mmcu=${INPUT_MCU} -DF_CPU=${INPUT_FCPU}")

endfunction()

function(SETUP_PROGRAMMER MCU_NAME)
    find_program(AVRDUDE avrdude)
    IF(NOT AVRDUDE)
            message(WARNING "'avrdude' program not found. 'upload' and 'info' targets will not be available!")
    ELSE(NOT AVRDUDE)
    
        SET(PROG_ID "usbtiny")
        SET(PROG_PART ${MCU_NAME})
        SET(HEXFORMAT "ihex")
    
        add_custom_target(upload
                ${AVRDUDE}
                        -c ${PROG_ID}
                        -p ${PROG_PART}
                        #-P ${PROGR_PORT} -e
                        -U flash:w:${PROJECT_NAME}.hex
                DEPENDS ${PROJECT_NAME}.hex ${PROJECT_NAME}.ee.hex
                VERBATIM)
    
        add_custom_target(info
                ${AVRDUDE} -v
                        -c ${PROG_ID}
                        -p ${PROG_PART}
                        #-P ${PROGR_PORT} -e
                        -U hfuse:r:high.txt:r -U lfuse:r:low.txt:r
                VERBATIM)
    
        add_custom_command(
                OUTPUT ${PROJECT_NAME}.hex
                COMMAND /usr/bin/${CMAKE_SYSTEM_NAME}-objcopy --strip-all -j .text -j .data -O ${HEXFORMAT} ${PROJECT_NAME} ${PROJECT_NAME}.hex
                DEPENDS ${PROJECT_NAME}
                VERBATIM
        )
    
        add_custom_command(
                OUTPUT ${PROJECT_NAME}.ee.hex
                COMMAND /usr/bin/${CMAKE_SYSTEM_NAME}-objcopy --strip-all -j .eeprom --change-section-lma .eeprom=0
                                   -O ${HEXFORMAT} ${PROJECT_NAME} ${PROJECT_NAME}.ee.hex
                DEPENDS ${PROJECT_NAME}
                VERBATIM
        )
    
        find_program(SREC_CAT srec_cat)
        IF(NOT SREC_CAT)
                message(WARNING "'srec_cat' program not found. 'upload-fuses' target will not be available!")
                message(STATUS "Please install 'srec_cat': 'sudo apt-get install srecord'")
        ELSE(NOT SREC_CAT)
            add_custom_target(upload-fuses
                    ${AVRDUDE}
                            -c ${PROG_ID}
                            -p ${PROG_PART}
                            #-P ${PROGR_PORT} -e
                            -U hfuse:w:${PROJECT_NAME}.hfuse.hex
                            -U lfuse:w:${PROJECT_NAME}.lfuse.hex
                    DEPENDS ${PROJECT_NAME}.hfuse.hex ${PROJECT_NAME}.lfuse.hex
                    VERBATIM
            )
                    
              
            add_custom_command(
                    OUTPUT ${PROJECT_NAME}.fuse
                    COMMAND /usr/bin/${CMAKE_SYSTEM_NAME}-objcopy --strip-all -j .fuse --change-section-lma .fuse=0 -O ${HEXFORMAT} ${PROJECT_NAME} ${PROJECT_NAME}.fuse
                    DEPENDS ${PROJECT_NAME}
                    VERBATIM
            )
        
            add_custom_command(
                    OUTPUT ${PROJECT_NAME}.lfuse.hex
                    COMMAND ${SREC_CAT} ${PROJECT_NAME}.fuse -Intel -crop 0 1 -offset 0 -O ${PROJECT_NAME}.lfuse.hex -Intel
                    DEPENDS ${PROJECT_NAME}.fuse
                    VERBATIM
            )
            
            add_custom_command(
                    OUTPUT ${PROJECT_NAME}.hfuse.hex
                    COMMAND ${SREC_CAT} ${PROJECT_NAME}.fuse -Intel -crop 1 2 -offset -1 -O ${PROJECT_NAME}.hfuse.hex -Intel
                    DEPENDS ${PROJECT_NAME}.fuse
                    VERBATIM
            )
    
        ENDIF(NOT SREC_CAT)
    ENDIF(NOT AVRDUDE)
endfunction()
