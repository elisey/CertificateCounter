#include "ring_buffer.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

static unsigned int incrementRingBufferIndex(unsigned int index, unsigned int maxIndexValue);

void RingBuffer_Init(ringBuffer_t* this, size_t elementSize, unsigned int numOfElements)
{
	this->elementSize = elementSize;
	this->maxNumOfElements = numOfElements;

	this->ptrBuffer = calloc(numOfElements, elementSize);
	assert(this->ptrBuffer != NULL);

	RingBuffer_Clear(this);
}

void RingBuffer_Clear(ringBuffer_t *this)
{
	this->numOfElements = 0;
	this->rd = 0;
	this->wr = 0;
}

bool RingBuffer_Push(ringBuffer_t *this, void *ptrElement)
{
	if (this->numOfElements == this->maxNumOfElements)	{
		return false;
	}

	uint8_t *ptrDst = &(this->ptrBuffer[ this->wr * this->elementSize ]);
	memcpy( ptrDst, ptrElement, this->elementSize );
	this->wr = incrementRingBufferIndex(this->wr, this->maxNumOfElements);
	this->numOfElements++;

	assert(this->numOfElements <= this->maxNumOfElements);
	return true;
}

bool RingBuffer_Pop(ringBuffer_t *this, void *ptrElement)
{
	if (this->numOfElements == 0)	{
		return false;
	}

	uint8_t *ptrSrc = &(this->ptrBuffer[ this->rd * this->elementSize ]);
	memcpy( ptrElement, ptrSrc, this->elementSize );
	this->rd = incrementRingBufferIndex(this->rd, this->maxNumOfElements);

	assert(this->numOfElements > 0);
	this->numOfElements--;
	return true;
}

unsigned int RingBuffer_NumOfElements(ringBuffer_t *this)
{
	return this->numOfElements;
}

static unsigned int incrementRingBufferIndex(unsigned int index, unsigned int maxIndexValue)
{
	index++;
	if (index >= maxIndexValue)	{
		index = 0;
	}
	return index;
}
