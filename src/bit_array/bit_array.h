/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Yotam Yitzhak Peled
    	date: 1/3/23
    
******************************************************************************/

#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__
#include <stddef.h>/*size_t*/

typedef size_t bitarr_t;

/* 
	index should be between 0 and 63 
	value should be 0 or 1
*/


/* Flip the value of an indexed bit */
bitarr_t BitArrFlipBit(bitarr_t bitarr, size_t index);

/* Set the bit at index location to value*/
bitarr_t BitArrSetBit(bitarr_t bitarr, size_t index, int value);

/* Set the bit on at index location*/
bitarr_t BitArrSetOnBit(bitarr_t bitarr,size_t index);

/* Set off the bit at index location*/
bitarr_t BitArrSetOffBit(bitarr_t bitarr,size_t index);

/* Set all bits on */
bitarr_t BitArrSetOnAll(bitarr_t bitarr);

/* Set off all bits */
bitarr_t BitArrSetOffAll(bitarr_t bitarr);

/* Gets the bit at index location */
int BitArrGetBit(bitarr_t bitarr, size_t index);

/* Count all the bits that are on */
size_t BitArrCountOnBits(bitarr_t bitarr);

/* Count all the bits that are Off */
size_t BitArrCountOffBits(bitarr_t bitarr);

/* Rotate the bits Left num times */
bitarr_t BitArrRotateLeft(bitarr_t bitarr, size_t num);

/* Rotate the bits Right num times */
bitarr_t BitArrRotateRight(bitarr_t bitarr, size_t num);

/* Return a string of the bits */
char* BitArrToString(bitarr_t bitarr, char *buffer);

/* Mirror the bit array */
bitarr_t BitArrMirror(bitarr_t bitarr);

#endif /* __BIT_ARRAY_H__*/

