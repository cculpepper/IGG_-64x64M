add_executable(iggx_pio)

pico_generate_pio_header(iggx_pio ${CMAKE_CURRENT_LIST_DIR}/iggx.pio)

target_sources(iggx_pio PRIVATE iggx.c)

target_link_libraries(iggx_pio PRIVATE
        pico_stdlib
        hardware_pio
        hardware_dma
        hardware_adc
        )

pico_sdk_init()
pico_add_extra_outputs(iggx_pio)
pico_enable_stdio_usb(iggx_pio 1)
pico_enable_stdio_uart(iggx_pio 0)

# add url via pico_set_program_url
example_auto_set_url(iggx_pio)
