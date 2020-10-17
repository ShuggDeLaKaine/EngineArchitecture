/** \file shaderDataType.h */
#pragma once
#include <cstdint>

namespace Engine
{
	/* \enum shaderDataType.h 
	* Enum class for all the shader data types.
	*/
	enum class ShaderDataType
	{
		None = 0, 
		Float, Float2, Float3, Float4,

	};

	//SDT- Shader Data Type
	namespace SDT
	{
		static uint32_t size(ShaderDataType type)
		{
			//TODO: change these to a static array, then just have to access to element in the array.
			//Will be MUCH FASTER.
			switch (type)
			{
			case ShaderDataType::Float: return 4;			//size of a float is 4 bytes.
			case ShaderDataType::Float2: return 4 * 2;		//2 floats is 8 bytes.
			case ShaderDataType::Float3: return 4 * 3;		//3 floats is 12 bytes.
			case ShaderDataType::Float4: return 4 * 4;		//4 floats is 16 bytes.
			default: return 0;
			}
		}

		static uint32_t componentCount(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float: return 1;		//number of componets is 1.
			case ShaderDataType::Float2: return 2;		//componets is 2.
			case ShaderDataType::Float3: return 3;		//componets is 3.
			case ShaderDataType::Float4: return 4;		//componets is 4.
			default: return 0;
			}

		}
	}

}
