/** \file bufferLayout.h */
#pragma once

#include "shaderDataType.h"
#include <vector>

namespace Engine
{
	/** \class BufferElement
	* Class to take data for each individual element that the buffer layout is made up of.
	* Things like the data type, the size of the take data type, normalised, offset of data type.
	*/
	class BufferElement
	{
	public:
		ShaderDataType m_dataType;		//!< what type of data is this element (float, int etc)
		uint32_t m_size;				//!< int for the size.
		uint32_t m_offset;				//!< int for the length of the offset.
		bool m_normalised;				//!< bool for whether buffer is normalised.

		BufferElement() {};		//!< default constructor.
		BufferElement(ShaderDataType dataType, bool normalised = false) : 
			m_dataType(dataType),
			m_size(SDT::size(dataType)),
			m_offset(0),
			m_normalised(normalised)
		{} //!< constructor with params. Takes dataType and whether it is normalised (set to false, so if left blank will be default set to false); with initialisor list linked to above vars (offset init to 0 as this will need calculating).
	private:

	};

	/* \class BufferLayout
	* Class that's an abstraction of the notion of a buffer layout.
	*/
	class BufferLayout
	{
	public:
		BufferLayout() {};			//!< default constructor.
		BufferLayout(const std::initializer_list<BufferElement>& element) :
			m_elements(element)
		{
			calculateStrideAndOffset();
		}							//!< constructor with params, returns the calculation of stride and offset.
		inline uint32_t getStride() { return m_stride; }		//!< get the length of the stride.
		void addBufferElement(BufferElement element);			//!< add a buffer element.
		inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }				//!< 
		inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }					//!< 
		inline std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }	//!< 
		inline std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }		//!< 
	private:
		std::vector<BufferElement> m_elements;	//!< buffer elements.
		uint32_t m_stride;						//!< width in bytes of a buffer line.
		void calculateStrideAndOffset();		//!< calculate the stride and offset based on the elements. 

	};
}
