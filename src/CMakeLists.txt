
set(SOURCES
    CControl.cpp
    CSwitch.cpp
    CTaskMgr.cpp
    CTimer.cpp
    CWiper.cpp
    main.cpp
    )

generate_avr_firmware(wiper
                      SRCS ${SOURCES}
                      MCU atmega8
                      FCPU 8000000UL 
                      )

