#pragma once

#include "Viper/Renderer/Texture.h"

namespace Viper {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		// Note that this destructor is automatically virtual 
		// because the base class has a virtual destructor so
		// this is just here for clarity
		virtual ~OpenGLTexture2D();
		const std::string& GetPath() const { return m_Path; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual void SetData(void* data, uint32_t size) override ;
		virtual void Bind(uint32_t slot = 0) const override;

	private:
		bool m_IsLoaded = false;
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
		uint32_t m_InternalFormat;
		uint32_t m_DataFormat;
	};

}
