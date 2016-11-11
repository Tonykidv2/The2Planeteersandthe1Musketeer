#pragma once
#include <d3d11.h>

class Model
{
private:
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	ID3D11ShaderResourceView* Texture = nullptr;
	ID3D11ShaderResourceView* NormTexture = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	unsigned int IndexCount;
public:
	Model(const char* ModelPath, const char* TexturePath, ID3D11Device& Device);
	~Model();

	bool DrawToScreen(ID3D11DeviceContext& Context, ID3D11Buffer* ConstantBuffer);


};