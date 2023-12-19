/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Yotam Yitzhak Peled
    	date: 1/3/23
    
******************************************************************************/

#include <string.h> /*strlen */


#include "bit_array.h"

#define B1ON1OFF 0xaaaaaaaaaaaaaaaa
#define B2ON2OFF 0xcccccccccccccccc
#define B4ON4OFF 0xf0f0f0f0f0f0f0f0
#define B8ON8OFF 0xff00ff00ff00ff00
#define B16ON16OFF 0xffff0000ffff0000
#define B32ON32OFF 0xffffffff00000000

int Reverse(char *str, int start, int end); 

/****************************** BitArrFlipBit ********************************/

bitarr_t BitArrFlipBit(bitarr_t bitarr, size_t index)
{
	return bitarr ^ 1 << index; 
}

/****************************** BitArrSetBit *********************************/

bitarr_t BitArrSetBit(bitarr_t bitarr, size_t index, int value)
{
	return (bitarr & ((1 << index) ^ ~0)) | (value << index);
}

/****************************** BitArrSetOnBit *******************************/

bitarr_t BitArrSetOnBit(bitarr_t bitarr,size_t index)
{
	return bitarr | (size_t)1 << index;
}

/****************************** BitArrSetOffBit ******************************/

bitarr_t BitArrSetOffBit(bitarr_t bitarr,size_t index)
{
	return bitarr & ~((size_t)1 << index);
}

/****************************** BitArrSetOnAll ******************************/

bitarr_t BitArrSetOnAll (bitarr_t bitarr)
{
	(void)bitarr;
	return ~0;
}

/****************************** BitArrSetOffAll ******************************/

bitarr_t BitArrSetOffAll(bitarr_t bitarr)
{
	(void)bitarr;
	return 0;
}

/****************************** BitArrGetBit *********************************/

int BitArrGetBit(bitarr_t bitarr, size_t index)
{
	return (bitarr >> index) & 1;
}

/****************************** BitArrCountOnBits ****************************/

size_t BitArrCountOnBits(bitarr_t bitarr)
{
	size_t counter = 0;
	while(bitarr)
	{
		counter++;
		bitarr &= bitarr -1;
	}
	return counter;
}

/****************************** BitArrCountOffBits ***************************/

size_t BitArrCountOffBits(bitarr_t bitarr)
{
	return 64 - BitArrCountOnBits(bitarr);
}

/****************************** BitArrRotateLeft *****************************/

bitarr_t BitArrRotateLeft(bitarr_t bitarr, size_t num)
{
	num %= 64;
	return (bitarr << num) | (bitarr >> (64 - num)); 
}

/****************************** BitArrRotateRight ****************************/

bitarr_t BitArrRotateRight(bitarr_t bitarr, size_t num)
{
	num %= 64;
	return (bitarr >> num) | (bitarr << (64 - num)); 
}

/****************************** BitArrToString *******************************/

char* BitArrToString(bitarr_t bitarr, char *buffer)
{
	size_t i = 0;
	
	while(i < 64)
	{
		buffer[63-i] = BitArrGetBit(bitarr,i) + '0';
		i++;
	}
	buffer[i] = '\0';
	return buffer;

}

/****************************** BitArrMirror *********************************/

bitarr_t BitArrMirror(bitarr_t bitarr)
{	
	bitarr = (bitarr & B32ON32OFF) >> 32 | (bitarr & ~B32ON32OFF) << 32;
	bitarr = (bitarr & B16ON16OFF) >> 16 | (bitarr & ~B16ON16OFF) << 16;
	bitarr = (bitarr & B8ON8OFF) >> 8 | (bitarr & ~B8ON8OFF) << 8;
	bitarr = (bitarr & B4ON4OFF) >> 4 | (bitarr & ~B4ON4OFF) << 4;
	bitarr = (bitarr & B2ON2OFF) >> 2 | (bitarr & ~B2ON2OFF) << 2;
	bitarr = (bitarr & B1ON1OFF) >> 1 | (bitarr & ~B1ON1OFF) << 1;

	return bitarr;
}

/*****************************************************************************/

int Reverse(char *str, int start, int end)
{
	char temp = str[start];
    while (start < end)
    {
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}	
	return 0;
}
