#pragma once


namespace engine
{
	class RenderData
	{
	public:

		RenderData(void){}

		virtual ~RenderData(void){}

		virtual void											Render(Sys_GraphicsPtr pSysGraphics)										= 0;
		virtual GFXPtr											GetGFX()																	= 0;
		virtual math::Matrix44									GetWorldMatrix()															= 0;
	};


}
