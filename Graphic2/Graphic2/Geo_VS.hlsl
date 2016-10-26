struct VS_IN
{
	float4 pos : POSITION;
	float3 uv : UV;
	float3 normal : NORMALS;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 uv : UV;
	float3 normal : NORMALS;
};


VS_OUT main(VS_IN input)
{

	VS_OUT output = (VS_OUT)0;

	output.pos.xyzw = input.pos.xyzw;
	output.uv = input.uv;
	output.normal = input.normal;

	return output;
}