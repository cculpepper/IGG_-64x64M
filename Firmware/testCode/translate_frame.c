#include <stdint.h>
#include <stdio.h>

void translate_frame(uint8_t norm[], uint8_t iggx[]);
void print_frame(uint8_t arr[], int size)
{
	// Print the iggx array to verify the result
	for (int i = 0; i < size; i++) {
		printf("%02X ", arr[i]);
		if ((i + 1) % 8 == 0) {
			printf("\n");
		}
	}
}

int main()
{
	// Define the size of the norm and iggx arrays
	// Assuming norm array has 64*8 bits (512 bits = 64 bytes)
	// and iggx array has the same size
	int norm_size = 64;
	int iggx_size = 64;
	// Allocate the iggx array
	uint8_t iggx[iggx_size];

	for (int i = 0; i < iggx_size; i++) {
		iggx[i] = 0;
	}
	// Allocate and initialize the norm array
	uint8_t norm[norm_size];
	for (int i = 0; i < norm_size; i++) {
		norm[i] = (uint8_t)0 * i; // Example initialization, fill with some values
	}
	// norm[0] = 0xff;
	//
	norm[12] = 0xff;
	norm[23] = 0xff;
	norm[63] = 0x55;
	// norm[62] = 0xf0;
	printf("input array:\n");
	print_frame(norm, norm_size);

	// Call the translate_frame function
	translate_frame(norm, iggx);

	printf("iggx array:\n");
	print_frame(iggx, iggx_size);

	return 0;
}
#define NUM_ROWS 8

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
	for (int row = 0; row < 8; row++) {
		start_byte = row * 8;
		for (int iggx_byte_i = start_byte; iggx_byte_i < start_byte + 8; iggx_byte_i++) {
			printf("input: \r\n");
			for (int i = start_byte; i < start_byte + 8; i++) {
				printf("%02X ", norm[i]);
				if ((i + 1) % 8 == 0) {
					printf("\n");
				}
			}
			byte = 0;
			// for (int norm_byte = start_byte; norm_byte < start_byte + 8; norm_byte++) {

			uint32_t norm_bit_mask = 1 << (iggx_byte_i % 8);
			for (int bit = 0; bit < 8; bit++) {
				int norm_byte = bit + start_byte;
				// uint8_t val = ((norm[norm_byte] & bit_mask) << bit_shift);
				if ((norm[norm_byte] & norm_bit_mask)) {
					byte |= 1 << bit;
				}
				printf("row: %d, iggx_byte %d normal byte #: %d, 0x%x, Masking with 0x%x (%d), shifting %d, byte 0x%x\r\n", row, iggx_byte_i, norm_byte, norm[norm_byte], norm_bit_mask, (norm[norm_byte] & norm_bit_mask), bit, byte);
			}
			printf("\r\n 0x%x \r\n", byte);
			iggx[iggx_byte_i] = byte;
		}
	}
}
