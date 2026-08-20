
#include <crc.h>
#include <stdint.h>

uint32_t CRC32_Calculate(const uint8_t *data, uint32_t length){

	uint32_t crc = 0xFFFFFFFFU;

	for (uint32_t i = 0; i < length; i++)
	{
	    crc ^= data[i];

		for (uint8_t bit=0; bit<8; bit++){
			if (crc & 1U){
				crc = (crc >> 1) ^ ( 0xEDB88320);
			}
			else{
				crc = (crc >> 1);
			}
		}
	}
	crc ^= 0xFFFFFFFFU;
	return crc;
}
