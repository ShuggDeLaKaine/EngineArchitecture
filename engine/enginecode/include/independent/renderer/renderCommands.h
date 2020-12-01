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
		enum class Commands { clearDepthBuffer, clearColourBuffer, clearColourAndDepthBuffer, setClearColour };		//!< enum class to take types of commands.

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
		static RenderCommands* createCommand(RenderCommands::Commands command)
		{
			RenderCommands* result = new RenderCommands;

			switch (command)
			{
			case RenderCommands::Commands::clearColourAndDepthBuffer :
				result->m_action = getClearColourAndDepthBufferCommand();
				return result;
			}
		}			//!< create commands, with switch statements for command type and the action required for each.
	private:
		static std::function<void(void)> getClearColourAndDepthBufferCommand();			//!< command to clear colour and depth buffers.

	};
}