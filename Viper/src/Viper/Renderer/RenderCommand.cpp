#include "vppch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Viper {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}