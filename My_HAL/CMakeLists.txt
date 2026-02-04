add_library(My_HAL STATIC
    Src/hal_gpio.c
)

target_include_directories(My_HAL
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(My_HAL 
    PUBLIC  
        STM32_HAL 
)