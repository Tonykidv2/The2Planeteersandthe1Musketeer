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
	std::ifstream fin;
	Letter* item;
	
	
	fin.open(_file);
	if (fin.fail())
		return false;

	if(fin.is_open())
	{
		while (!fin.eof())
		{
			
			item = new Letter();
			
			fin >> item->character;

			fin >> item->left;
			fin >> item->right;
			fin >> item->top;
			fin >> item->bottom;
			
			allLetters.push_back(item);
			
		}
	}
	

	
	fin.close();
	return true;
}
void Font::SetResource(ID3D11Device* _device, ID3D11SamplerState* m_state)
{
//	_device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &m_vShader);
	static const D3D11_INPUT_ELEMENT_DESC MeshDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
//	_device->CreateInputLayout(MeshDesc, ARRAYSIZE(MeshDesc), Trivial_VS, sizeof(Trivial_VS), &m_inLayout);
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(DirectX::XMFLOAT4X4) * 3, D3D11_BIND_CONSTANT_BUFFER);

	//_device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &m_pixelShader);
	_device->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBuffer);
	//CREATE VERTEX BUFFER
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = vertBuffer.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VERTEX)*(UINT)vertBuffer.size(), D3D11_BIND_VERTEX_BUFFER);
	
		_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_VertexBuffer);

	//CREATE INDEX BUFFER
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indexBuffer.data();
	indexBufferData.SysMemPitch = 0;

	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(UINT)*(UINT)indexBuffer.size(), D3D11_BIND_INDEX_BUFFER);

	_device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_IndexBuffer);

	D3D11_SAMPLER_DESC state;
	ZeroMemory(&state, sizeof(D3D11_SAMPLER_DESC));
	state.Filter = D3D11_FILTER_ANISOTROPIC;
	state.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	state.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	state.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	state.MaxAnisotropy = 1;

		_device->CreateSamplerState(&state, &m_state);
}
void Font::BLIT(ID3D11Device* _device, ID3D11DeviceContext* _context, char _letter, ID3D11Resource* _backbuffer, float x, float y)
{
	CreateDDSTextureFromFile(_device,
		L"fontDDS.dds",
		&letterRegion,
		&m_shaderview);
	Letter* temp;
		temp=FindLetter(_letter);
		D3D11_BOX let;
		let.left = temp->left;
		let.right = temp->right;
		let.top = temp->top;
		let.bottom = temp->bottom;
		_context->CopySubresourceRegion(_backbuffer, 0, x, y, 0, letterRegion, 0, &let);
}
Font::Letter* Font::FindLetter(char x)
{
	for (size_t i = 0; i < allLetters.size(); i++)
	{
		if (allLetters[i]->character == x)
			return allLetters[i];
	}
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