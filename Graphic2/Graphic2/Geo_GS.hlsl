#pragma pack_matrix(row_major)


struct GS_IN
{
	float4 pos : POSITION;
	float3 uv : UV;
	float3 normal : NORMALS;
};

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float3 uv : UV;
	float3 normal : NORMAL;
};


cbuffer OBJECT : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float ScreenHeight;
	float3 pad;

}


[maxvertexcount(4)]
void main(
	point GS_IN input[1], 
	inout TriangleStream< GS_OUT > output
)
{
	static float half_size = 2.0f;

	GS_OUT verts[4] = { (GS_OUT)0, (GS_OUT)0, (GS_OUT)0, (GS_OUT)0 };

	verts[0].pos = float4(input[0].pos.x-half_size, input[0].pos.y+half_size, input[0].pos.z, 1);
	verts[1].pos = float4(input[0].pos.x+half_size, input[0].pos.y+half_size, input[0].pos.z, 1);
	verts[2].pos = float4(input[0].pos.x-half_size, input[0].pos.y-half_size, input[0].pos.z, 1);
	verts[3].pos = float4(input[0].pos.x+half_size, input[0].pos.y-half_size, input[0].pos.z, 1);

	verts[0].uv = float3(0, 0, 0);
	verts[1].uv = float3(1, 0, 0);
	verts[2].uv = float3(0, 1, 0);
	verts[3].uv = float3(1, 1, 0);

	float3 normals = float3(cross(float3(verts[1].pos.xyz - verts[0].pos.xyz), float3(verts[0].pos.xyz - verts[2].pos.xyz)));
	verts[0].normal = normals;
	verts[1].normal = normals;
	verts[2].normal = normals;
	verts[3].normal = normals;


	for (uint i = 0; i < 4; i++)
	{
		//GSOutput element;
		//element.pos = input[i];
		//output.Append(element);

		verts[i].pos = mul(verts[i].pos, worldMatrix);
		verts[i].pos = mul(verts[i].pos, viewMatrix);
		verts[i].pos = mul(verts[i].pos, projectionMatrix);

		verts[i].normal = mul(verts[i].pos, (float3x3)worldMatrix);

		output.Append(verts[i]);
	}
}