#include "ring_buffer.h"

#include <assert.h>

ringBuffer_t testBuffer;
ringBuffer_t *buf = &testBuffer;

void RingBuffer_Test()
{
	int dumb = 0;
	bool result = false;

	RingBuffer_Init(buf, sizeof(int), 5);

	assert( RingBuffer_NumOfElements(buf) == 0 );

	result = RingBuffer_Pop(buf, &dumb);
	assert(result == false);

	int x1 = 1;
	int x2 = 2;
	int x3 = 3;
	int x4 = 4;
	int x5 = 5;
	int x6 = 6;

	result = RingBuffer_Push(buf, &x1);
	assert(result == true);

	result = RingBuffer_Pop(buf, &dumb);
	assert(result == true);
	assert(dumb = x1);

	result = RingBuffer_Push(buf, &x1);
	assert(result == true);
	result = RingBuffer_Push(buf, &x2);
	assert(result == true);
	result = RingBuffer_Push(buf, &x3);
	assert(result == true);
	result = RingBuffer_Push(buf, &x4);
	assert(result == true);
	result = RingBuffer_Push(buf, &x5);
	assert(result == true);


	assert( RingBuffer_NumOfElements(buf) == 5 );

	result = RingBuffer_Push(buf, &x6);
	assert(result == false);

	result = RingBuffer_Pop(buf, &dumb);
	assert(result == true);
	assert(dumb = x1);
	result = RingBuffer_Pop(buf, &dumb);
	assert(result == true);
	assert(dumb = x2);
	result = RingBuffer_Pop(buf, &dumb);
	assert(result == true);
	assert(dumb = x3);
	result = RingBuffer_Pop(buf, &dumb);
	assert(result == true);
	assert(dumb = x4);
	result = RingBuffer_Pop(buf, &dumb);
	assert(result == true);
	assert(dumb = x5);

	result = RingBuffer_Pop(buf, &dumb);
	assert(result == false);

	assert( RingBuffer_NumOfElements(buf) == 0 );



}
