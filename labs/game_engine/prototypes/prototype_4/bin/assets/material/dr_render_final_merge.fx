#include <deferred_shading/dr_gbuffer.hlsl>

float4x4 mat:MATRIX_WVP;
float4x4 matView:MATRIX_WV;

float2 vp:VP_SIZE;
Texture2D tex_gbuffer[3]:DR_GBUFFER;
Texture2D tex_abuffer:DR_ABUFFER;


struct vs_in
{
	float3 pos:POSITION;
};
struct vs_out
{
	float4 pos:SV_POSITION;
};

vs_out vs_main(vs_in i)
{
	vs_out o;
	o.pos = float4(i.pos.xyz, 1);

	return o;
}

struct ps_out
{
	float4 color:SV_TARGET;
};

ps_out ps_main(vs_out i)
{
	ps_out o;


	float2 uv = i.pos.xy;
	uv = uv / i.pos.w;
	
	uv.x = (uv.x ) / vp.x;
	uv.y = (uv.y ) / vp.y;
	
	half3 d = dr_gbuffer_get_diffuse(tex_gbuffer, uv);
	float3 l = tex_abuffer.Sample(Sampler_GBuffer,uv).xyz;

	o.color.xyz = l * d;
	o.color.w = 1;
	return o;
}
RasterizerState rs
{
	CULLMODE				= None;
};
DepthStencilState ds
{
	DepthEnable				= FALSE;
	DepthFunc				= LESS;
	DepthWriteMask				= ZERO;
	StencilEnable				= true;
	FrontFaceStencilFail			= KEEP;
	FrontFaceStencilDepthFail		= KEEP;
	FrontFaceStencilPass			= KEEP;
	FrontFaceStencilFunc			= EQUAL;

	BackFaceStencilFail			= KEEP;
	BackFaceStencilDepthFail		= KEEP;
	BackFaceStencilPass			= KEEP;
	BackFaceStencilFunc			= NEVER;
};
BlendState bs
{
	BLENDENABLE[0]				= false;
	
};
technique11 test
{
  pass p1
  {
	SetBlendState( bs, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
	SetRasterizerState(rs);
	SetDepthStencilState(ds, 1);
	SetVertexShader( CompileShader( vs_4_0, vs_main() ) );
	SetPixelShader( CompileShader( ps_4_0, ps_main() ) );

  }
}