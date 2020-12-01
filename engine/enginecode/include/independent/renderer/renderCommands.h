/** \file renderCommands.h */
#pragma once
#include <functional>

namespace Engine
{
	class RendererCommons;

	/** \class RenderCommands
	*	\brief 
	*/
	class RenderCommands
	{
	public:
		enum class Commands { clearDepthBuffer, clearColourBuffer, clearColourAndDepthBuffer, setClearColour  };		//!< enum class to take types of commands.

	private:
		std::function<void(void)> m_action;		//!< action of the render command.
		friend class RenderCommandsFactory;		//!< coupled with friend RenderCommandFactory, as both won't work without the other.
		friend class RendererCommons;			//!< coupled with friend RendererCommons, as both will action commands from here.

	};

	/** \class RenderCommandsFactory
	*	\brief Class to store functions api agnostic commands for clearing things like buffers.
	*/
	class RenderCommandsFactory
	{
	public:
		template<typename ... Args> static RenderCommands* createCommand(RenderCommands::Commands command, Args&& ...args)
		{
			RenderCommands* result = new RenderCommands;

			switch (command)
			{
			case RenderCommands::Commands::clearColourAndDepthBuffer :
				result->m_action = getClearColourAndDepthBufferCommand();
				return result;

			case RenderCommands::Commands::setClearColour :
				float r, g, b, a;

				auto argTuple = std::make_tuple(args...);

				getValue<float, 0>(r, argTuple);
				getValue<float, 1>(g, argTuple);
				getValue<float, 2>(b, argTuple);
				getValue<float, 3>(a, argTuple);

				result->m_action = setClearColourCommand(r, g, b, a);
				return result;

			}
		}			//!<template with typename arguements; creates commands, with switch statements for command type and the action required for each.
	private:
		static std::function<void(void)> getClearColourAndDepthBufferCommand();							//!< command to clear colour and depth buffers.
		static std::function<void(void)> setClearColourCommand(float r, float g, float b, float a);		//!< command to clear colour, with float RBGA params.


		/*
		static std::function<void(void)>
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);		&&	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
		*/


		//following code based on https://www.geeksforgeeks.org/how-to-iterate-over-the-elements-of-an-stdtuple-in-c/		
		//check value I - if MORE OR EQUALSs number of values in tuple (size of parameter pack) then do nothing for these.
		template <typename G, size_t I, typename... Ts>
		typename std::enable_if<I >= sizeof...(Ts), void>::type			
		static getValue(G& result, std::tuple<Ts...> tup)
		{
			//if I is greater or equal to the size of the tuple; then don't do anything.
		}

		//check value I - if LESS than number of values in tuple (size of parameter pack) and get that thing.
		template <typename G, size_t I, typename... Ts>
		typename std::enable_if<(I < sizeof...(Ts)), void>::type
		static getValue(G& result, std::tuple<Ts...> tup)
		{
			//get the I-th thing within the tuple.
			result = std::get<I>(tup);
		}
	};
}