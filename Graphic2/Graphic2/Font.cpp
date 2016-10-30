#include "Font.h"



Font::Font()
{
	//m_font = NULL;
	m_texture = NULL;
}
Font::Font(const Font&)
{

}

Font::~Font()
{
}


bool Font::Initialize(ID3D11Device* _device, char* _x, WCHAR* _y)
{
	/*HRESULT result;
	result = LoadFont(_x);
	if (!result)
		return false;
	result = LoadTexture(_device, _y);
	if (!result)
		return false;

	return true;*/
	return 0;
}
void Font::shutdown()
{
	ReleaseFontData();
	ReleaseTexture();
}
//ID3D11ShaderResourceView* Font::GetTexture()
//{
//	return m_shaderview;
//}
void Font::BuildVertexArray(void* j, char* _file, float x, float y)
{
	
	
}

bool Font::LoadFont(char* _file)
{
	ifstream fin;
	Letter item;
	char temp;
	fin.open(_file);
	if (fin.fail())
		return false;

	if(fin.is_open())
	{
		fin.get(temp);
		item.character = temp;
		
		fin >> item.left;
		fin >> item.right;
		fin >> item.top;
		fin >> item.bottom;
		allLetters.push_back(item);
	}
	fin.close();
	return true;
}
void Font::ReleaseFontData()
{
	
	
}
bool Font::LoadTexture(ID3D11Device* _device, WCHAR* _y)
{
	//CreateDDSTextureFromFile(_device, _y, nullptr, &m_shaderview);
	
	return true;
}
void Font::ReleaseTexture()
{
	//m_texture->Release();
}
void Font::Update(ID3D11DeviceContext* context)
{
	

	//context->CopySubresourceRegion(m_texture,)
}