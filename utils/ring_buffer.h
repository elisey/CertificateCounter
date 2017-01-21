#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "stddef.h"

typedef struct	{
	uint8_t *ptrBuffer;
	unsigned int wr;
	unsigned int rd;
	unsigned int numOfElements;

	unsigned int maxNumOfElements;
	size_t elementSize;
} ringBuffer_t;

void RingBuffer_Init(ringBuffer_t* this, size_t elementSize, unsigned int numOfElements);
void RingBuffer_Clear(ringBuffer_t *this);
bool RingBuffer_Push(ringBuffer_t *this, void *ptrElement);
bool RingBuffer_Pop(ringBuffer_t *this, void *ptrElement);
unsigned int RingBuffer_NumOfElements(ringBuffer_t *this);
