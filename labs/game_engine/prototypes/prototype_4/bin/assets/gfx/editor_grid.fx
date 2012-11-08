
float4x4 world:WORLD_MATRIX;
float4x4 view:VIEW_MATRIX;
float4x4 proj:PROJ_MATRIX;


struct vs_in
{
	float3 pos:POSITION;
};
struct vs_out
{
	float4 pos:SV_POSITION;
};

vs_out vs_main(vs_in vsin)
{
	float4x4 mat = mul(world, view);
	mat = mul(mat, proj);

	vs_out vsout;
	vsout.pos = mul(float4(vsin.pos, 1), mat);
	return vsout;
}

struct ps_out
{
	float4 color:SV_TARGET;
};

ps_out ps_main(vs_out psin)
{
	ps_out psout;
	psout.color = float4(1,1,1,1);
	return psout;
}

RasterizerState rs
{
	CULLMODE = None;
};
technique11 test
{
  pass p1
  {
	SetRasterizerState(rs);
	SetVertexShader( CompileShader( vs_4_0, vs_main() ) );
	SetPixelShader( CompileShader( ps_4_0, ps_main(  ) ) );
  }
}