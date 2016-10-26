// Input control point
struct VS_CONTROL_POINT_OUTPUT
{
	float3 pos : POSITON;
	float3 norm : NORMAL;
	float3 uv : UV;
	float3 Tangent : TANGENT;
	float3 BiTangent : BITANGENT;
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

#define NUM_CONTROL_POINTS 3


cbuffer SIZING :register (b0)
{
	float4 size;
}

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;


	const float d0 = 1.0f;
	const float d1 = 5.0f;

	float tess = 4.5f * saturate((d1 - size.x) / (d1 - d0));

	if (tess <= 1.0f)
		tess = 1.0f;

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] = 
	Output.EdgeTessFactor[1] = 
	Output.EdgeTessFactor[2] = 
	Output.InsideTessFactor = tess; // e.g. could calculate dynamic tessellation factors instead

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.pos = ip[i].pos;
	Output.norm = ip[i].norm;
	Output.uv = ip[i].uv;
	Output.Tangent = ip[i].Tangent;

	return Output;
}
