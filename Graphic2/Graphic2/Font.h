#pragma once
#include"Defines.h"
class Font
{

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
private:
	
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* _device, WCHAR* _y);
	void ReleaseTexture();
	
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_shaderview;
	std::vector<Letter> allLetters;
	std::vector<Letter> lowerLetters;
	std::vector<Letter> upperLetters;
	std::vector<Letter> specialLetters;
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

