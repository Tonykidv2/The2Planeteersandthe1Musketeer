#include "LightHelper.hlsli"


struct INPUT_PIXEL
{
	float4 pos : SV_POSITION;
	float3 uv : UV;
	float3 normal : NORMALS;
	float3 posW : POSITIONW;
	float3 Tangent : TANGENT;
	float3 BiTangent : BITANGENT;
};

TextureCube TEXTURE : register(t0);
Texture2D Texture1 : register(t1);
Texture2D TextureNorm : register(t2);
SamplerState FILTER : register(s0);

cbuffer texturing : register(b0)
{
	float WhichTexture;
	float3 padding;
}

cbuffer CB_Lights : register(b1)
{
#if USINGOLDLIGHTCODE
	DirectionalLight m_DirLight;
	PointLight m_pointLight;
	SpotLight m_SpotLight;
	Material m_Material;
	float3 m_EyePosw;
	float pad;
#endif

#if !USINGOLDLIGHTCODE
	Lights list[4];
#endif

}


float4 main( INPUT_PIXEL colorFromRasterizer ) : SV_TARGET
{
	
	float4 color = float4(0, 0, 0, 1);
	float3 ColorNorm = float3(0, 0, 0);

	if (WhichTexture == 0)
	{
		color = TEXTURE.Sample(FILTER, colorFromRasterizer.normal).rgba;
		return color;
	}

	if (WhichTexture == 1)
	{
		color = Texture1.Sample(FILTER, colorFromRasterizer.uv).rgba;
		ColorNorm = TextureNorm.Sample(FILTER, colorFromRasterizer.uv).rgb;
	}

	float3 newNormal = NormalCalcFunc(ColorNorm, colorFromRasterizer.Tangent , colorFromRasterizer.normal);

	if (WhichTexture == 2)
		color = Texture1.Sample(FILTER, colorFromRasterizer.uv).rgba;

	//float weight0, weight1, weight2, weight3, weight4;
	//float normalization;
	//
	//weight0 = 1.0f;
	//weight1 = 0.9f;
	//weight2 = 0.55f;
	//weight3 = 0.18f;
	//weight4 = 0.1f;
	//
	//normalization = (weight0 + 2.0f *(weight1 + weight2 + weight3 + weight4));
	//
	//weight0 /= normalization;
	//weight1	/= normalization;
	//weight2	/= normalization;
	//weight3	/= normalization;
	//weight4	/= normalization;
	//
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord1) * weight4;
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord2) * weight3;
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord3) * weight2;
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord4) * weight1;
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord5) * weight0;
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord6) * weight1;
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord7) * weight2;
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord8) * weight3;
	//color += Texture1.Sample(FILTER, colorFromRasterizer.TexCoord9) * weight4;

	float4 finalColor = float4(0, 0, 0, 1);

#if !USINGOLDLIGHTCODE
	if (WhichTexture == 2)
		newNormal = colorFromRasterizer.normal;

	if(list[0].radius.x == 1)
		finalColor.xyz += DirectionalLightCalc(list[0], newNormal);
	if (list[1].radius.x == 1)
		finalColor.xyz += PNTLightCalc(list[1], newNormal, colorFromRasterizer.posW);
	if (list[2].radius.x == 1)
		finalColor.xyz += SPOTLightCalc(list[2], newNormal, colorFromRasterizer.posW);

	finalColor.xyz *= color.xyz;
	finalColor.w = color.w;


#endif	

#if USINGOLDLIGHTCODE
	float3 toEyee = normalize(m_EyePosw - colorFromRasterizer.pos);
	
	float4 ambient	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float4 A, D, S;
	
	ComputeDirectionalLight(m_DirLight, color, toEyee, A, D, S);
	
	ambient		+= A;
	diffuse		+= D;
	spec		+= S;
	
	//ComputePointLight(m_pointLight, colorFromRasterizer.pos, (float3)color, toEyee, A, D, S);
	//
	//ambient	+= A;
	//diffuse	+= D;
	//spec		+= S;
	
	//ComputeSpotLight(m_SpotLight, toEyee, (float3)color, toEyee, A, D, S);
	//
	//ambient	+= A;
	//diffuse	+= D;
	//spec		+= S;
	
	float4 litColor = ambient + diffuse + spec; // + color;
	
	litColor.a = m_Material.Diffuse.a; // +color.a;
	litColor = saturate(litColor);

	return litColor;
#endif

	return finalColor;

}