struct INPUT_VS
{
	float4 pos : POSITION;
	float3 uv : UV;
	float3 normal : NORMALS;
	float3 Tangent : TANGENT;
	float3 BiTangent : BITANGENT;
};

struct OUTPUT_VS
{
	float3 pos : POSITON;
	float3 norm : NORMAL;
	float3 uv : UV;
	float3 Tangent : TANGENT;
	float3 BiTangent : BITANGENT;
};

OUTPUT_VS main( INPUT_VS input )
{

	OUTPUT_VS  output = (OUTPUT_VS)0;

	output.pos = input.pos.xyz;
	output.norm = input.normal;
	output.uv.xy = input.uv.xy;
	output.Tangent = input.Tangent;

	return output;
}