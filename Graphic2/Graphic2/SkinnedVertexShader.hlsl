#pragma pack_matrix(row_major)



struct INPUT_VERTEX
{
    float3 pos : POSITION;
    float3 normal : NORMALS;
    float2 uv : UV;
    float3 Tangent : TANGENT;
    float3 JointWeights : WEIGHT;
    uint4 JointIndice : BONEINDICES;
};

struct OUTPUT_VERTEX
{
    float4 projectedCoordinate : SV_POSITION;
    float3 uvOUT : UV;
    float3 normalOUT : NORMALS;
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

cbuffer Skeleton : register(b2)
{
    float4x4 bonesTrans[96];
}

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = fromVertexBuffer.JointWeights.x;
    weights[1] = fromVertexBuffer.JointWeights.y;
    weights[2] = fromVertexBuffer.JointWeights.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];
    
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);
    
    posL += weights[0] * mul(float4(fromVertexBuffer.pos, 1.0), bonesTrans[fromVertexBuffer.JointIndice[0]]).xyz;
    posL += weights[1] * mul(float4(fromVertexBuffer.pos, 1.0), bonesTrans[fromVertexBuffer.JointIndice[1]]).xyz;
    posL += weights[2] * mul(float4(fromVertexBuffer.pos, 1.0), bonesTrans[fromVertexBuffer.JointIndice[2]]).xyz;
    posL += weights[3] * mul(float4(fromVertexBuffer.pos, 1.0), bonesTrans[fromVertexBuffer.JointIndice[3]]).xyz;
    
    normalL += weights[0] * mul(float4(fromVertexBuffer.normal, 0.0), (float4x4)bonesTrans[fromVertexBuffer.JointIndice[0]]).xyz;
    normalL += weights[1] * mul(float4(fromVertexBuffer.normal, 0.0), (float4x4)bonesTrans[fromVertexBuffer.JointIndice[1]]).xyz;
    normalL += weights[2] * mul(float4(fromVertexBuffer.normal, 0.0), (float4x4)bonesTrans[fromVertexBuffer.JointIndice[2]]).xyz;
    normalL += weights[3] * mul(float4(fromVertexBuffer.normal, 0.0), (float4x4)bonesTrans[fromVertexBuffer.JointIndice[3]]).xyz;
    
    tangentL += weights[0] * mul(float4(fromVertexBuffer.Tangent.xyz, 0.0), (float4x4)bonesTrans[fromVertexBuffer.JointIndice[0]]).xyz;
    tangentL += weights[1] * mul(float4(fromVertexBuffer.Tangent.xyz, 0.0), (float4x4)bonesTrans[fromVertexBuffer.JointIndice[1]]).xyz;
    tangentL += weights[2] * mul(float4(fromVertexBuffer.Tangent.xyz, 0.0), (float4x4)bonesTrans[fromVertexBuffer.JointIndice[2]]).xyz;
    tangentL += weights[3] * mul(float4(fromVertexBuffer.Tangent.xyz, 0.0), (float4x4)bonesTrans[fromVertexBuffer.JointIndice[3]]).xyz;
    
    fromVertexBuffer.pos = posL;
    fromVertexBuffer.normal = normalL;
    fromVertexBuffer.Tangent = tangentL;

    OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX) 0;
	
    sendToRasterizer.projectedCoordinate.xyz = fromVertexBuffer.pos.xyz;
    sendToRasterizer.projectedCoordinate.w = 1;


    float4x4 scales = float4x4(Scale, 0.0f, 0.0f, 0.0f,
								0.0f, Scale, 0.0f, 0.0f,
								0.0f, 0.0f, Scale, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f);

    sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, scales);
    sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Rotation);
    sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Translate);

    sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, worldMatrix);
    sendToRasterizer.posW = sendToRasterizer.projectedCoordinate.xyz;
    //sendToRasterizer.posW = mul(worldMatrix, float4(sendToRasterizer.posW, 0.0));

    sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, viewMatrix);
    sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, projectionMatrix);
	
    sendToRasterizer.uvOUT = float3(fromVertexBuffer.uv, 0.0f);

    //sendToRasterizer.normalOUT = mul(float4(fromVertexBuffer.normal, 0.0), (float4x4) worldMatrix).xyz;
    //sendToRasterizer.outTangent = mul(float4(fromVertexBuffer.Tangent, 0.0), (float4x4) worldMatrix).xyz;
    sendToRasterizer.normalOUT = mul(normalize(fromVertexBuffer.normal), (float3x3) worldMatrix);
    sendToRasterizer.outTangent = mul(normalize(fromVertexBuffer.Tangent), (float3x3) worldMatrix);
	
    return sendToRasterizer;
}
