/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/bootrom.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// Our assembled program:
#include "iggx.pio.h"

#include "rickroll.h"
#define USE_DMA
#define DMA_BUFF_NUM_FRAMES (2)
#define DMA_BUFF_BIT_COUNT (64 * 64)
#define DMA_BUFF_BYTE_COUNT (DMA_BUFF_BIT_COUNT / 8)
#define DMA_BUFF_SIZE_BYTES (DMA_BUFF_BYTE_COUNT * 1) // Previously frames accounted here
#define DMA_BUFF_SIZE_XFERS (DMA_BUFF_SIZE_BYTES >> 2)
volatile uint8_t iggx_buf[DMA_BUFF_SIZE_BYTES]
    __attribute__((aligned(2 * sizeof(uint32_t*))));
volatile uint8_t iggx_buf_2[DMA_BUFF_SIZE_BYTES]
    __attribute__((aligned(2 * sizeof(uint32_t*))));

uint8_t* current_frame_ptr;
uint8_t* current_frame_write_ptr;
uint8_t second_buf[DMA_BUFF_SIZE_BYTES];
uint8_t scratch_buf[DMA_BUFF_SIZE_BYTES];
int current_frame = 0;
void increment_iggx_buffer()
{
	if (current_frame >= DMA_BUFF_NUM_FRAMES - 1) {
		current_frame           = 0;
		current_frame_ptr       = iggx_buf;
		current_frame_write_ptr = iggx_buf_2;
	} else {
		current_frame++;
		current_frame_ptr       = iggx_buf_2;
		current_frame_write_ptr = iggx_buf;
	}
}

void copy_video_frame(uint8_t* source, int frame_no, uint8_t* dest)
{
	memcpy(dest, source + (frame_no * sizeof(iggx_buf)), sizeof(iggx_buf));
}

uint iggx_dma;
void iggx_dma_isr(void)
{
	// Need to :
	// clear the interrupt
	// set the DMA start address
	// current_frame++;
	if (current_frame >= DMA_BUFF_NUM_FRAMES) {
		current_frame = 0;
	}
	dma_channel_set_read_addr(iggx_dma, current_frame_ptr, true);
	// dma_channel_set_read_addr(iggx_dma, &iggx_buf[current_frame * DMA_BUFF_SIZE_BYTES], true);
	// restart the DMA
	dma_hw->ints0 = 1u << iggx_dma;
	dma_start_channel_mask(1 << iggx_dma);
	// above restarts apparently
}

void translate_frame(uint8_t norm[], uint8_t iggx[])
{
	// Need to translate a normal array where the lsb is the first pixel, the
	// second the second , after 64 bits, 8 bytes, the row increments into where
	// bit 0 goes to row 0, pixel 7,
	// 1-> 0,15
	// 2-> 0,23
	// 3-> 0,31
	// 4-> 0,39
	// 5-> 0,47
	// 6-> 0,55
	// 7-> 0,63
	//
	// ...
	// 8 -> 0,6
	// 9 -> 0,14
	// 10-> 0,23
	// ...
	// 16-> 0,5
	//
	// From the IGGX side, byte 0 is composed of
	// LSB                       MSB
	// [0, 8, 16, 24, 32, 40, 48, 56]
	// which is really byte:bit number
	// [0:0, 1:0, 2:0, 3:0, 4:0, 5:0, 6:0, 7:0]
	// [0:1, 1:1, 2:1, 3:1, 4:1, 5:1, 6:1, 7:1]
	// [0:2, 1:2, 2:2, 3:2, 4:2, 5:2, 6:2, 7:2]
	int start_byte = 0;
	uint8_t byte   = 0;
	for (int row = 0; row < 64; row++) {
		start_byte = row * 8;
		for (int iggx_byte_i = start_byte; iggx_byte_i < start_byte + 8; iggx_byte_i++) {
			// printf("input: \r\n");
			// for (int i = start_byte; i < start_byte + 8; i++) {
			// 	printf("%02X ", norm[i]);
			// 	if ((i + 1) % 8 == 0) {
			// 		printf("\n");
			// 	}
			// }
			byte = 0;
			// for (int norm_byte = start_byte; norm_byte < start_byte + 8; norm_byte++) {
			iggx[iggx_byte_i] = 0;

			uint32_t norm_bit_mask = 1 << (iggx_byte_i % 8);
			for (int bit = 0; bit < 8; bit++) {
				int norm_byte = bit + start_byte;
				// uint8_t val = ((norm[norm_byte] & bit_mask) << bit_shift);
				if ((norm[norm_byte] & norm_bit_mask)) {
					byte |= 1 << bit;
				}
				// printf("row: %d, iggx_byte %d normal byte #: %d, 0x%x, Masking with 0x%x (%d), shifting %d, byte 0x%x\r\n", row, iggx_byte_i, norm_byte, norm[norm_byte], norm_bit_mask, (norm[norm_byte] & norm_bit_mask), bit, byte);
			}
			// printf("\r\n 0x%x \r\n", byte);
			iggx[iggx_byte_i] = byte;
		}
	}
}
void adc_setup()
{
	adc_init();
	adc_gpio_init(28);
	adc_gpio_init(27);
	adc_gpio_init(26);
}
float get_hv_v()
{

	adc_select_input(0);
	const float conversion_factor = 3.3f / (1 << 12);
	int result                    = adc_read();
	float sum                     = 0;
	for (int i = 0; i < 10; i++) {
		sum += adc_read();
	}
	sum /= 10;
	float v = sum * conversion_factor;
	v       = v * ((1e6 + 4.7e3) / 4.7e3);
	return v;
}
float get_battery_v()
{

	adc_select_input(1);
	const float conversion_factor = 3.3f / (1 << 12);
	int result                    = adc_read();
	float sum                     = 0;
	int count                     = 0;
	for (int i = 0; i < 10; i++) {
		sum += adc_read();
		count++;
	}
	sum /= count;
	float v = sum * conversion_factor;
	v       = v * ((1e3 + 47e3) / 1e3);
	return v;
	// 47K, 10K
}
int read_adc_buttons()
{
	adc_select_input(2);
	const float conversion_factor = 3.3f / (1 << 12);
	int result                    = adc_read();
	float sum                     = 0;
	for (int i = 0; i < 10; i++) {
		sum += adc_read();
	}
	sum /= 10;
	// 3V
	// 2.245
	// 1.65
	// 0.579
	//
	float v = sum * conversion_factor;
	printf("%f ", v);
	if (v < 0.05) {
		return 0;
	} else if (v < .1) {
		return 1;
	} else if (v < .7) {
		return 2;
	} else if (v < 1.6522) {
		return 3;
	} else if (v < 1.7) {
		return 4;
	} else {
		return 5;
	}
}

int main()
{
	volatile uint8_t* dma_buff_8 = (uint8_t*)&iggx_buf;
	int led_state                = 1;
	uint8_t value                = 0xaa;
	uint8_t value2               = 0xaa;
	int i;
	current_frame_ptr       = iggx_buf;
	current_frame_write_ptr = iggx_buf_2;
	stdio_init_all();
	set_sys_clock_khz(200000, true);
	adc_setup();
	// gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
	sleep_ms(2000); // Wait for serial to connect
	printf("Hello?");
	sleep_ms(1000);
	printf("1\r\n");
	// Choose which PIO instance to use (there are two instances)
	PIO pio = pio0;
#define HV_PIN 25
	gpio_init(HV_PIN);
	gpio_set_dir(HV_PIN, GPIO_OUT);
	gpio_put(HV_PIN, 1);
#define LED_PIN (12)

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	gpio_put(LED_PIN, 0);

	// for(;;);
	uint sm = pio_claim_unused_sm(pio, true);
#ifdef USE_DMA
	iggx_dma             = dma_claim_unused_channel(true);
	dma_channel_config c = dma_channel_get_default_config(iggx_dma);
	channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
	channel_config_set_dreq(&c, pio_get_dreq(pio, sm, true));
	channel_config_set_read_increment(&c, true);
	channel_config_set_write_increment(&c, false);

	dma_channel_configure(iggx_dma, &c, &pio->txf[sm], &iggx_buf,
	                      DMA_BUFF_SIZE_XFERS, false);
	irq_set_exclusive_handler(DMA_IRQ_0, iggx_dma_isr);
	dma_channel_set_irq0_enabled(iggx_dma, true);
	irq_set_enabled(DMA_IRQ_0, true);
	for (i = 0; i < DMA_BUFF_SIZE_BYTES; i++) {
		iggx_buf[i] = 0x03;
		// iggx_buf[i] = rand();
	}
#endif

	uint offset = pio_add_program(pio, &iggx_program);

	iggx_program_init(pio, sm, offset);

#ifdef USE_DMA
	sleep_ms(100);
	iggx_dma_isr(); // trigger the ISR, start the thing
	                // iggx_buf[1] = 0xf0;
#endif
	// sleep_ms(100);
	// sleep_ms(2000);
	// // pio_sm_put_blocking(pio, sm, 0x00000000);
	// // pio_sm_put_blocking(pio, sm, 0xff00ff00);
	// // pio_sm_put_blocking(pio, sm, 0xf0f0f0f0);
	// for (i = 0; i <= DMA_BUFF_SIZE_BYTES; i += 1) {
	// 	scratch_buf[i] = 0xaa;
	// }
	// translate_frame(scratch_buf, &iggx_buf[0 * DMA_BUFF_BYTE_COUNT]);
	// memcpy(iggx_buf, scratch_buf, sizeof(iggx_buf));
	// for (i = 0; i <= DMA_BUFF_SIZE_BYTES; i += 1) {
	// 	scratch_buf[i] = 0x55;
	// }
	// translate_frame(scratch_buf, &iggx_buf[1 * DMA_BUFF_BYTE_COUNT]);
	// for (i = 0; i <= DMA_BUFF_SIZE_BYTES; i += 1) {
	// 	scratch_buf[i] = 0x88;
	// }
	// translate_frame(scratch_buf, &iggx_buf[2 * DMA_BUFF_BYTE_COUNT]);
	// for (i = 0; i <= DMA_BUFF_SIZE_BYTES; i += 1) {
	// 	scratch_buf[i] = 0x11;
	// }
	// translate_frame(scratch_buf, &iggx_buf[3 * DMA_BUFF_BYTE_COUNT]);
	// //   for(;;){
	// //
	// // gpio_put(LED_PIN, led_state);
	// // led_state = !led_state;
	// //       sleep_ms(500);
	// //   }
	// //
	//
	// value  = 0x55;
	// value2 = 0xaa;
	// memset(second_buf, 0, sizeof(second_buf));
	// for (i = 0; i <= DMA_BUFF_SIZE_BYTES / 2; i += 2) {
	// 	second_buf[i]     = value;
	// 	second_buf[i + 1] = value2;
	// 	memcpy(iggx_buf, second_buf, sizeof(iggx_buf));
	// 	sleep_ms(10);
	// }
	//
	// sleep_ms(2000);
	// translate_frame(second_buf, iggx_buf);

	int pattern = 2;
#define MAX_PATTERNS 2
	memset(second_buf, 0, sizeof(second_buf));
	memset(iggx_buf, 0, sizeof(iggx_buf));
	memset(iggx_buf_2, 0, sizeof(iggx_buf_2));
	uint32_t next_time = to_ms_since_boot(get_absolute_time());
	value              = 0x55;
	value2             = 0xaa;
	int count          = 0;
	int frame          = 0;
	while (true) {
		uint32_t current_time = to_ms_since_boot(get_absolute_time());
		int button            = read_adc_buttons();
		// printf("%d\r\n", button);
		printf("pattern %d, \tbutton: %d\tframe: %d\t count: %d\t battery: %f\t HV: %f\r\n", pattern, button, current_frame, count, get_battery_v(), get_hv_v());
		if (button > 0) {
			pattern = button;
			memset(second_buf, 0, sizeof(second_buf));
			memset(iggx_buf_2, 0, sizeof(iggx_buf_2));
			memset(iggx_buf, 0, sizeof(iggx_buf));
			count = 0;
			value = 0xaa;
			sleep_ms(10); // sleep so the button can settle
		}
		if (pattern == 0 || pattern == 1) {
			if (current_time > next_time) {
				next_time = current_time + 2000;
				if (value == 0x55) {
					value  = 0xaa;
					value2 = 0x55;
				} else {
					value  = 0x55;
					value2 = 0xaa;
				}
			}
			for (i = 0; i <= DMA_BUFF_SIZE_BYTES / 2; i += 2) {
				second_buf[i]     = value;
				second_buf[i + 1] = value2;
			}
			memcpy(current_frame_write_ptr, second_buf, sizeof(iggx_buf));
			increment_iggx_buffer();
		} else if (pattern == 2) {
			if (current_time > next_time) {
				next_time = current_time + 100;
				frame++;
				if (frame >= num_frames) {
					frame = 0;
				}
                copy_video_frame(frames.bytes, frame, current_frame_write_ptr);
				increment_iggx_buffer();
			}
		} else if (pattern == 3) {
			if (current_time > next_time) {
				// memset(second_buf, 0, sizeof(second_buf));
				next_time = current_time + 700;
				if (value == 0x05) {
					value  = 0x30;
					value2 = 0x03;
				} else {
					value  = 0x03;
					value2 = 0x30;
				}
				// for (i = 0; i <= DMA_BUFF_SIZE_BYTES / 2; i += 2) {
				// 	// second_buf[i]     = value;
				// 	// second_buf[i + 1] = value2;
				// 	second_buf[i]     = 0;
				// 	second_buf[i + 1] = 0;
				// }
				// second_buf[24+count++] = 0xff;
				// if (count >= 64) {
				// 	count = 0;
				// }
				second_buf[count] = 0xff;
				translate_frame(second_buf, current_frame_write_ptr);
				second_buf[count++] = 0x00;
				// memcpy(current_frame_write_ptr, second_buf, sizeof(iggx_buf));
				increment_iggx_buffer();
				// memcpy(iggx_buf, second_buf, sizeof(iggx_buf));
			}
		}

		gpio_put(LED_PIN, led_state);
		led_state = !led_state;
        // reset_usb_boot(0,0);
		// sleep_ms(500);
	}
}
