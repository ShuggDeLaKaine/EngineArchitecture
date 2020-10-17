/** \file bufferLayout.cpp */
#include "engine_pch.h"
#include "rendering/bufferLayout.h"

namespace Engine
{

	void BufferLayout::addBufferElement(BufferElement element)
	{
		m_elements.push_back(element);
		calculateStrideAndOffset();
	}

	void BufferLayout::calculateStrideAndOffset()
	{
		/* WHAT THIS NEEDS TO DO:
		* needs to iterate over the elements in the buffer and work out for each element.
		* first offset 0, so set local offset var to 0.
		* next offset will be the accumulation of whatever the sizes were.
		* following one will be the size of all the previous elements added up.
		* stride is the size of all the previous elements added up, plus the last one.
		*/

		//create local offset variable and set to 0
		uint32_t l_offset = 0;	

		//range based loop
		for (auto& element : m_elements)
		{
			//set class offset to this local offset.
			element.m_offset = l_offset;
			//calculate the offset, add the size of this element to the offset.
			l_offset += element.m_size;
		}
		//set the stride as the local offset.
		m_stride = l_offset;
	}
}