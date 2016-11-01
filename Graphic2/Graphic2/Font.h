#pragma once
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <DirectXMath.h>
#include <vector>
#include<fstream>
#include <istream>
#include<iostream>
#include <string>
#include"DDSTextureLoader.h"


class Font
{
	struct VERTEX
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 norm;
	};
	struct Letter
	{

		float left, right, bottom, top;
		char character;
	};
public:
	Font();
	Font(const Font&);
	~Font();
	bool Initialize(ID3D11Device* _device, char* _x, WCHAR* _y);
	void shutdown();
	
	void BuildVertexArray(void* j, char* _file, float x, float y);
	void Update(ID3D11DeviceContext* context);
	bool LoadFont(char* _file);
	Letter* FindLetter(char x); 
	void BLIT(ID3D11Device* _device,ID3D11DeviceContext* _context,char _letter,ID3D11Resource* _backbuffer,float x, float y);
	ID3D11Resource* letterRegion;
	/////////DIRECTX///////
	const wchar_t* TexturePath;
	const wchar_t* TexturePath2;
	std::vector<VERTEX> vertBuffer;
	std::vector<UINT> indexBuffer;
	ID3D11Buffer*		m_VertexBuffer;
	ID3D11Buffer*		m_IndexBuffer;
	ID3D11ShaderResourceView*	m_ShaderResourceView;
	ID3D11ShaderResourceView*	m_ShaderResourceView2;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11Buffer*		m_constantBuffer;
	ID3D11VertexShader*	m_vShader;
	ID3D11InputLayout*	m_inLayout;
private:
	
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* _device, WCHAR* _y);
	void ReleaseTexture();
	void SetResource(ID3D11Device* _device,ID3D11SamplerState* m_state);
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_shaderview;
	std::vector<Letter*> allLetters;
	std::vector<Letter*> lowerLetters;
	std::vector<Letter*> upperLetters;
	std::vector<Letter*> specialLetters;
	/*void BLIT(const unsigned int* _source, unsigned int source_w, RECT _tile, unsigned int draw_x, unsigned int draw_y)
	{
		unsigned int x = 0;
		unsigned int y = 0;
		for (unsigned int index_y = (unsigned int)_tile.top; index_y < (unsigned int)_tile.bottom; ++index_y, ++y)
		{
			for (unsigned int index_x = (unsigned int)_tile.left; index_x <(unsigned int)_tile.right; ++index_x, ++x)
			{
				DrawPixel(_source[Convert2D_1D(index_x, index_y, source_w)], draw_x + x, draw_y + y);

			}
			x = 0;
		}
	}*/
};

