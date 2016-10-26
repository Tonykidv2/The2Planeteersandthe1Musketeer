#pragma pack_matrix(row_major)

struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
	float3 wPosition : WorldPOS;
	float3 norm : NORMAL;
	float3 uv : UV;
	float3 Tangent : TANGENT;
	float3 BiTangent : BITANGENT;
	// TODO: change/add other stuff
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float3 pos : POSITON;
	float3 norm : NORMAL;
	float3 uv : UV;
	float3 Tangent : TANGENT;
	float3 BiTangent : BITANGENT;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

cbuffer OBJECT : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float ScreenHeight;
	float3 pad;

}

cbuffer TRANSLATOR : register(b1)
{
	float4x4 Rotation;
	float4x4 Translate;
	float Scale;
	float3 padding;
}

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	//rasterized position
	Output.vPosition = float4(
		patch[0].pos * domain.x + patch[1].pos * domain.y + patch[2].pos * domain.z , 1);


	Output.norm = float3(
		patch[0].norm * domain.x + 
		patch[1].norm * domain.y + 
		patch[2].norm * domain.z);

	Output.uv = float3(
		patch[0].uv * domain.x + 
		patch[1].uv * domain.y + 
		patch[2].uv * domain.z);

	Output.Tangent = float3(
		patch[0].Tangent * domain.x +
		patch[1].Tangent * domain.y +
		patch[2].Tangent * domain.z);

	float4x4 scales = float4x4(Scale, 0.0f, 0.0f, 0.0f,
								0.0f, Scale, 0.0f, 0.0f,
								0.0f, 0.0f, Scale, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f);

	Output.vPosition = mul(Output.vPosition, scales);
	Output.vPosition = mul(Output.vPosition, Rotation);
	Output.vPosition = mul(Output.vPosition, Translate);

	Output.vPosition = mul(Output.vPosition, worldMatrix);
	Output.wPosition = Output.vPosition;
	Output.vPosition = mul(Output.vPosition, viewMatrix);
	Output.vPosition = mul(Output.vPosition, projectionMatrix);

	Output.norm = mul(normalize(float4(Output.norm, 0)), worldMatrix);
	Output.Tangent = mul(normalize(float4(Output.Tangent, 0)), worldMatrix);

	return Output;

}
