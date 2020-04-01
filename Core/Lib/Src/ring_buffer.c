/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"

int i=0;
bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0))
	{
	    ringBuffer->size = dataBufferSize;
        ringBuffer->head = 0;
        ringBuffer->tail = 0;
        ringBuffer->table = dataBuffer;
        ringBuffer->length = 0;
		return true;
	}
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) 
	{
	    
	    for (;ringBuffer->length>0; ringBuffer->length--)
	    {
	       ringBuffer->table[ringBuffer->tail] = 0;
	       ringBuffer->tail = ringBuffer->tail + 1;
	       ringBuffer->tail %= ringBuffer->size;
	    }
		return true;
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  assert(ringBuffer);	
    if (ringBuffer->length && ringBuffer != 0)
	    return false;
	else
	    return true;
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) 
	{
		return ringBuffer->length;
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer)
	{
		return ringBuffer->size;
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) 
	{
	    if(ringBuffer->length != ringBuffer->size)
	    {
	    ringBuffer->table[ringBuffer->head] = c;
	    ringBuffer->head = (ringBuffer->head + 1);
        
        ringBuffer->head %= ringBuffer->size;
        ringBuffer->length++;
        return true;
	    }
	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);
	
  if ((ringBuffer) && (c))
    {
        if (ringBuffer->length>0)
        {
    		*c = ringBuffer->table[ringBuffer->tail];
    		ringBuffer->tail = ringBuffer->tail + 1; //% ringBuffer->size;
    		ringBuffer->tail %= ringBuffer->size;
    		ringBuffer->length--;
    		return true;
        }
        else
            return false;
	}
	return false;
}

