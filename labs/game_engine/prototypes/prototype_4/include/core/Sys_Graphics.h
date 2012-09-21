#pragma once

#include "math/math_all.h"

namespace engine
{
	class Sys_Graphics
	{
	public:

		virtual bool									Initialize(void* app_handle, 
																uint32 width, 
																uint32 height, 
																uint32 format)										= 0;

		virtual void									Release()													= 0;
		virtual void									Draw(RenderDataPtr pData)									= 0;
		virtual void									PushRenderData(RenderDataPtr pData)							= 0;
		virtual void									Render()													= 0;

		virtual void									SetViewTransform(const math::Matrix44& view)				= 0;
		virtual void									SetProjTransform(const math::Matrix44& proj)				= 0;

		virtual void									SetClearColor(const math::Color4& clr)						= 0;
		virtual void									SetClearDepth(float d)										= 0;
		virtual void									SetClearStencil(uint32 val)									= 0;

		virtual void									ClearRenderTarget()											= 0;
		virtual void									ClearRenderQueue()											= 0;
		//virtual void									SetRenderTarget(Texture2DPtr pTarget)						= 0;
		virtual void									Present()													= 0;

		//virtual ShaderPtr								CreateShader()												= 0;

		//virtual RenderDataPtr							CreateRenderData()											= 0;
		//virtual Texture2DPtr							CreateTexture2D()											= 0;

		//virtual TexturePtr								CreateRenderTarget()										= 0;

	protected:
		Sys_Graphics(void){}
		virtual ~Sys_Graphics(void){}
	};


}
