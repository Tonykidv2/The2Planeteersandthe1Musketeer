#pragma pack_matrix(row_major)



struct INPUT_VERTEX
{
	float4 pos : POSITION;
	float3 uv : UV;
	float3 normal : NORMALS;
	float3 Tangent : TANGENT;
	float3 BiTangent : BITANGENT;
};

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float3 uvOUT : UV;
	float3 normalOUT: NORMALS;
	float3 posW : POSITIONW;
	float3 outTangent : TANGENT;
	float3 outBiTangent : BITANGENT;
};

// TODO: PART 3 STEP 2a
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

OUTPUT_VERTEX main( INPUT_VERTEX fromVertexBuffer )
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	
	sendToRasterizer.projectedCoordinate.xyz = fromVertexBuffer.pos.xyz;
	sendToRasterizer.projectedCoordinate.w = 1;

	float4x4 scales = float4x4( Scale,0.0f, 0.0f, 0.0f,
								0.0f,Scale,0.0f,0.0f,
								0.0f,0.0f,Scale,0.0f,
								0.0f,0.0f,0.0f,1.0f);

	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, scales);
	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Rotation);
	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Translate);

	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, worldMatrix);
	sendToRasterizer.posW = sendToRasterizer.projectedCoordinate;

	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, viewMatrix);
	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, projectionMatrix);
	
	sendToRasterizer.uvOUT = fromVertexBuffer.uv;
	sendToRasterizer.uvOUT.z = 0;

	sendToRasterizer.normalOUT = mul(normalize(fromVertexBuffer.normal), (float3x3)worldMatrix);
	sendToRasterizer.outTangent = mul(normalize(fromVertexBuffer.Tangent), (float3x3)worldMatrix);
	
	return sendToRasterizer;
}