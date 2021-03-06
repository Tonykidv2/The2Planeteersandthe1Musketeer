
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "XTime.h"
#include "Defines.h"

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include "PixelShader.csh"
#include "VertexShader.csh"
#include "Geo_GS.csh"
#include "Geo_VS.csh"
#include "VertexShaderInstance.csh"
#include "DomainTriangleShader.csh"
#include "HullTriangleShader.csh"
#include "VertexTriangleShader.csh"
#include "PixelTriangleShader.csh"
#include "SkinnedVertexShader.csh"
#include "Font.h"
#include "SpriteBatch.h"
#include "SimpleMath.h"
#include "SpriteFont.h"
#include<Audio.h>

#define BACKBUFFER_WIDTH	1280.0f
#define BACKBUFFER_HEIGHT	720.0f

#define RELEASE(X) {if(X != NULL) X->Release()}

ID3D11DeviceContext*	g_pd3dDeviceContext;
IDXGISwapChain*			g_pSwapChain;
ID3D11RenderTargetView* g_pRenderTargetView;
ID3D11Device*			g_pd3dDevice;
ID3D11DepthStencilView* g_StencilView;
ID3D11Texture2D*		g_TexBuffer;
D3D11_VIEWPORT			g_DirectView;
D3D11_VIEWPORT          g_secondDirectView;
D3D11_VIEWPORT          g_thirdDirectView;

bool g_ScreenChanged;
bool g_Minimized;
XMMATRIX g_newProjection;
LPARAM g_lParam;





//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	XTime							TimeWizard;

	//Model Data
	ID3D11Buffer*					VertexBufferStar = nullptr;
	ID3D11Buffer*					IndexBufferStar = nullptr;
	ID3D11Buffer*					VertexBufferPlane = nullptr;
	ID3D11Buffer*					IndexBufferPlane = nullptr;
	ID3D11Buffer*					VertexBufferSkyBox = nullptr;
	ID3D11Buffer*					IndexBufferSkyBox = nullptr;
	ID3D11Buffer*					VertexBufferSword = nullptr;
	ID3D11Buffer*					IndexBufferSword = nullptr;
	ID3D11Buffer*					VertexBufferDeadpool = nullptr;
	ID3D11Buffer*					IndexBufferDeadpool = nullptr;
	ID3D11Buffer*					VertexBufferSource = nullptr;
	ID3D11Buffer*					IndexBufferSource = nullptr;


	//Instances
	ID3D11Buffer*					InstanceBuffer;
	ID3D11Buffer*					DeadpoolInstanceVertexBuffer = nullptr;
	ID3D11Buffer*					DeadpoolInstanceIndexBuffer = nullptr;
	unsigned int					InstanceIndexCount;
	Instance						list[4];


	//Shaders
	ID3D11VertexShader*				DirectVertShader[3];
	ID3D11PixelShader*				DirectPixShader[2];
	ID3D11InputLayout*				DirectInputLay[2];

	//Constant Buffers
	ID3D11Buffer*					constantBuffer[2];
	ID3D11Buffer*					constantPixelBuffer;
	ID3D11Buffer*					CostantBufferLights;
	ID3D11Buffer*					InstanceCostantBuffer;

	//Textures & Samples
	ID3D11ShaderResourceView*       SkyBoxShaderView;
	ID3D11ShaderResourceView*		FloorShaderView;
	ID3D11ShaderResourceView*		FloorNORMShaderView;
	ID3D11ShaderResourceView*		SwordShaderView;
	ID3D11ShaderResourceView*		SwordNORMShaderView;
	ID3D11ShaderResourceView*		DeadpoolShaderView;
	ID3D11ShaderResourceView*		DeadpoolNORMShaderView;
	ID3D11SamplerState*				sampleTexture;


	//RasterStates
	ID3D11RasterizerState*			DefaultRasterState;
	ID3D11RasterizerState*			SkyBoxRasterState;

	//Blend States
	ID3D11BlendState*				BlendState;

	////Geometry Shader
	//ID3D11Buffer*					VertexBufferGeo;
	//ID3D11VertexShader*			VertexShaderGeo;
	//ID3D11GeometryShader*			GeometryShaderGeo;
	//ID3D11PixelShader*			PixelShaderGeo;
	//XMMATRIX						GeoMatrix;
	//ID3D11ShaderResourceView*		ShaderResourceViewGeo;

	//tesselation
	ID3D11Buffer*					vertexBufferTriangle;
	ID3D11VertexShader*				vertexShaderTriangle;
	ID3D11PixelShader*				pixelShaderTriangle;
	ID3D11HullShader*				hullShaderTriangle;
	ID3D11DomainShader*				domainShaderTriangle;
	Instance						TriangleMatrix;
	Scaling							TesselScale;
	ID3D11RasterizerState*			RasterStateWireFrameTriangle;
	ID3D11RasterizerState*			RasterStateSoildTriangle;
	ID3D11Buffer*					CostantBufferTessScale;


	unsigned int SkyBoxIndexCount;
	unsigned int PlaneIndexCount;
	unsigned int StarIndexCount;
	unsigned int SwordIndexCount;
	unsigned int DeadpoolIndexCount;
	unsigned int SourceIndexCount;
	unsigned int CrashIndexCount;

	void init3D(HWND hWnd);
	void Clean3d();
	void DrawComplexModel(ID3D11Buffer* VertexBuffer, ID3D11Buffer* IndexBuffer, ID3D11ShaderResourceView* Texture, ID3D11VertexShader* vertexShader,
		ID3D11PixelShader* pixelShader, unsigned int IndexCount, ID3D11DeviceContext* context);
	void GetInputTextBox(wstring& text);
	void ClearInput();
	void InitInput();
	SEND_TO_VRAM_WORLD WorldShader;
	SEND_TO_VRAM_PIXEL VRAMPixelShader;
	TRANSLATOR translating;

	unique_ptr<AudioEngine> audio;
	unique_ptr<SpriteBatch>spritebatch;
	unique_ptr<SpriteFont> m_textFont;
	unique_ptr<SoundEffect> sound;
	ID3D11ShaderResourceView* m_text;
	ID3D11ShaderResourceView* m_text2;
	ID3D11ShaderResourceView* m_textbox;
	bool lightsToggle = false;
	bool textureSwitch = true;
	bool textControls = false;
	bool bSplitScreen;
	vector<Words> lines;
	vector<wstring> words;
	XMFLOAT2 textWordpos = XMFLOAT2(5, 400);
	int indexLineY = 400;
	int currentLine = 0;
	bool animationdone = false;
#if USINGOLDLIGHTCODE
	LightSources Lights;
#endif

#if !USINGOLDLIGHTCODE
	LightSources Lights[4];
#endif

	XMMATRIX m_viewMatrix;
	POINT prevPoint;
	POINT newPoint;
	bool Checked;
	bool ToggleBumpMap;
	void CreateVertexIndexBufferModel(ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer, ID3D11Device* device, const char* Path, unsigned int* IndexCount);
	void CreateVertexIndexBufferModel1(ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer, ID3D11Device* device, const char* Path, unsigned int* IndexCount);
	void CreateSkinnedVertexIndexBufferModel(ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer, ID3D11Device* device, const char* Path, const char* oherAni,
		unsigned int* IndexCount, AnimationController* skeleton);
	ID3D11Buffer* BindPoseVertex = nullptr;
	ID3D11Buffer* BindPoseIndex = nullptr;
	ID3D11ShaderResourceView* BindPoseTexture = nullptr;
	ID3D11ShaderResourceView* BindPoseNormTexture = nullptr;
	unsigned BindPoseIndexCount;
	DEMO_APP() {}

	ID3D11Buffer* MagePoseVertex = nullptr;
	ID3D11Buffer* MagePoseIndex = nullptr;
	ID3D11ShaderResourceView* MagePoseTexture = nullptr;
	ID3D11ShaderResourceView* MagePoseNormTexture = nullptr;
	unsigned MagePoseIndexCount;
	cBufferSkeleton MageSkeleton;
	ID3D11VertexShader* SkinningShader = nullptr;
	ID3D11InputLayout* SkinningVertexLayout = nullptr;
	ID3D11Buffer* MageSkeleonBuffer = nullptr;
	AnimationController MageAnimation;


public:

	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	float calcdist(XMVECTOR v1, XMVECTOR v2);
	static DEMO_APP *GetInstance();
	bool Run();
	bool ShutDown();
	void ResizingOfWindows();
	void secondViewPort();
	void thirdViewPort();
	
};

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{

	application = hinst;
	appWndProc = proc;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, (LONG)BACKBUFFER_WIDTH, (LONG)BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"DirectXApplication", L"GRAPHIC 2 PROJECT", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);

	vector<XMFLOAT4> verts;
	vector<XMFLOAT3> norms;
	vector<XMFLOAT3> uvs;
	vector<unsigned int> vert_indices, norm_indices, uvs_indices;

#pragma region Initialization
	//Initalizing ID3D11Device & IDXGISwapChain & ViewPorts objects
	init3D(window);
	WorldShader.worldMatrix = XMMatrixIdentity();
	WorldShader.viewMatrix = XMMatrixIdentity();
	WorldShader.projectView = XMMatrixIdentity();
	g_newProjection = XMMatrixIdentity();
	m_viewMatrix = XMMatrixIdentity();
	m_viewMatrix = XMMatrixTranslation(0.0f, 2.0f, -5.0f);
	WorldShader.projectView = XMMatrixPerspectiveFovLH(XMConvertToRadians(FIELDOFVIEW), ASPECTRATIO, ZNEAR, ZFAR);

	translating.Translate = XMMatrixIdentity();
	translating.Rotation = XMMatrixIdentity();
	translating.Scale = 1;
	g_ScreenChanged = false;
	g_Minimized = false;
	ToggleBumpMap = true;
	ZeroMemory(&TesselScale, sizeof(TesselScale));
	TesselScale.scale.x = 1.0f;

#pragma endregion

#pragma region Creating Star Shape

	bool toggle = true;
	SIMPLE_VERTEX Star[12];

	Star[10].XYZW = XMFLOAT4(0, 1, -0.25f, 1);
	Star[10].RGBA = XMFLOAT4(0, 1, 0, 1);

	Star[11].XYZW = XMFLOAT4(0, 1, 0.25f, 1);
	Star[11].RGBA = XMFLOAT4(0, 0, 1, 1);

	for (unsigned int i = 0; i < 10; i++)
	{
		if (toggle)
		{
			Star[i].XYZW = XMFLOAT4(sin(((i * 36) * 3.14159f) / 180),
				cos(((i * 36) * 3.14159f) / 180) + 1, 0.0f, 1);
			Star[i].RGBA = XMFLOAT4(1, 0, 0, 1);

		}
		else
		{
			Star[i].XYZW = XMFLOAT4(sin(((i * 36) * 3.14159f) / 180) * .50f,
				cos(((i * 36) * 3.14159f) / 180) * .50f + 1, 0.0f, 1);
			Star[i].RGBA = XMFLOAT4(1, 0, 0, 1);
		}

		Star[i].Norm = XMFLOAT3(1.0f, 1.0f, 1.0f);

		toggle = !toggle;
	}

	unsigned int indice[60] =
	{
		0, 1, 10, 1, 2, 10, 2, 3, 10, 3, 4, 10, 4, 5, 10, 5, 6, 10, 6, 7, 10, 7, 8, 10, 8, 9, 10, 9, 0, 10,
		0, 9, 11, 9, 8, 11, 8, 7, 11, 7, 6, 11, 6, 5, 11, 5, 4, 11, 4, 3, 11, 3, 2, 11, 2, 1, 11, 1, 0, 11
	};

	StarIndexCount = 60;
#pragma endregion

#pragma region Creating Plane

	LoadModel::LoadObj("PlaneSuper.obj", verts, uvs, norms,
		vert_indices, uvs_indices, norm_indices);

	VERTEX* plane = new VERTEX[vert_indices.size()];
	unsigned int *planeindices = new unsigned int[vert_indices.size()];

	for (unsigned int i = 0; i < vert_indices.size(); i++)
	{
		plane[i].XYZW = verts[vert_indices[i]];
		plane[i].UV = uvs[uvs_indices[i]];
		plane[i].normals = norms[norm_indices[i]];
		planeindices[i] = i;
	}
	for (unsigned int i = 0; i < vert_indices.size(); i += 3)
	{
		XMFLOAT4 v0 = plane[i + 0].XYZW;
		XMFLOAT4 v1 = plane[i + 1].XYZW;
		XMFLOAT4 v2 = plane[i + 2].XYZW;

		XMFLOAT3 uv0 = plane[i + 0].UV;
		XMFLOAT3 uv1 = plane[i + 1].UV;
		XMFLOAT3 uv2 = plane[i + 2].UV;

		XMFLOAT4 deltaPos1 = XMFLOAT4(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z, v1.w - v0.w);
		XMFLOAT4 deltaPos2 = XMFLOAT4(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z, v2.w - v0.w);

		XMFLOAT3 deltaUV1 = XMFLOAT3(uv1.x - uv0.x, uv1.y - uv0.y, uv1.z - uv0.z);
		XMFLOAT3 deltaUV2 = XMFLOAT3(uv2.x - uv0.x, uv2.y - uv0.y, uv2.z - uv0.z);

		float ratio = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		XMFLOAT4 deltaPos1uv2 = XMFLOAT4(deltaPos1.x * deltaUV2.y, deltaPos1.y * deltaUV2.y, deltaPos1.z * deltaUV2.y, deltaPos1.w * deltaUV2.y);
		XMFLOAT4 deltaPos2uv1 = XMFLOAT4(deltaPos2.x * deltaUV1.y, deltaPos2.y * deltaUV1.y, deltaPos2.z * deltaUV1.y, deltaPos2.w * deltaUV1.y);

		XMFLOAT3 tangent = XMFLOAT3((deltaPos1uv2.x - deltaPos2uv1.x) * ratio, (deltaPos1uv2.y - deltaPos2uv1.y) * ratio,
			(deltaPos1uv2.z - deltaPos2uv1.z) * ratio);

		XMFLOAT4 deltaPos2uv1b = XMFLOAT4(deltaPos2.x * deltaUV1.x, deltaPos2.y * deltaUV1.x, deltaPos2.z * deltaUV1.x, deltaPos2.x * deltaUV1.x);
		XMFLOAT4 deltaPos1uv2b = XMFLOAT4(deltaPos1.x * deltaUV2.x, deltaPos1.y * deltaUV2.x, deltaPos1.z * deltaUV2.x, deltaPos1.w * deltaUV2.x);

		XMFLOAT3 bitangent = XMFLOAT3((deltaPos2uv1b.x - deltaPos1uv2b.x) * ratio, (deltaPos2uv1b.y - deltaPos1uv2b.y) * ratio,
			(deltaPos2uv1b.z - deltaPos1uv2b.z) * ratio);


		plane[i + 0].Tangent = tangent;
		plane[i + 1].Tangent = tangent;
		plane[i + 2].Tangent = tangent;

		plane[i + 0].BiTangent = bitangent;
		plane[i + 1].BiTangent = bitangent;
		plane[i + 2].BiTangent = bitangent;
	}

#pragma region VertexBuffer for Floor
	D3D11_BUFFER_DESC PlanebufferDesc;
	ZeroMemory(&PlanebufferDesc, sizeof(PlanebufferDesc));
	PlanebufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	PlanebufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	PlanebufferDesc.ByteWidth = (UINT)(sizeof(VERTEX) * vert_indices.size());

	D3D11_SUBRESOURCE_DATA sub_data_plane;
	ZeroMemory(&sub_data_plane, sizeof(sub_data_plane));
	sub_data_plane.pSysMem = plane;
	g_pd3dDevice->CreateBuffer(&PlanebufferDesc, &sub_data_plane, &VertexBufferPlane);
#pragma endregion

#pragma region IndexBuffer for Floor
	D3D11_BUFFER_DESC indexBuffDesc_Plane;
	ZeroMemory(&indexBuffDesc_Plane, sizeof(indexBuffDesc_Plane));
	indexBuffDesc_Plane.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc_Plane.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc_Plane.ByteWidth = (UINT)(sizeof(unsigned int) * vert_indices.size());

	D3D11_SUBRESOURCE_DATA indexData_Plane;
	ZeroMemory(&indexData_Plane, sizeof(indexData_Plane));
	indexData_Plane.pSysMem = planeindices;
	g_pd3dDevice->CreateBuffer(&indexBuffDesc_Plane, &indexData_Plane, &IndexBufferPlane);

#pragma endregion

	PlaneIndexCount = (unsigned int)vert_indices.size();
	delete[] planeindices;
	delete[] plane;

#pragma endregion

	verts.clear();
	uvs.clear();
	norms.clear();
	vert_indices.clear();
	uvs_indices.clear();
	norm_indices.clear();

#pragma region Creating Skybox
	LoadModel::LoadObj("SkyBox.obj", verts, uvs, norms,
		vert_indices, uvs_indices, norm_indices);

	VERTEX* sky_Box = new VERTEX[vert_indices.size()];
	unsigned int* SkyBoxIndices = new unsigned int[vert_indices.size()];
	for (unsigned int i = 0; i < vert_indices.size(); i++)
	{
		sky_Box[i].XYZW = verts[vert_indices[i]];
		sky_Box[i].UV = uvs[uvs_indices[i]];
		sky_Box[i].normals = norms[norm_indices[i]];
		SkyBoxIndices[i] = i;

	}

	for (unsigned int i = 0; i < vert_indices.size(); i += 3)
	{
		XMFLOAT4 v0 = sky_Box[i + 0].XYZW;
		XMFLOAT4 v1 = sky_Box[i + 1].XYZW;
		XMFLOAT4 v2 = sky_Box[i + 2].XYZW;

		XMFLOAT3 uv0 = sky_Box[i + 0].UV;
		XMFLOAT3 uv1 = sky_Box[i + 1].UV;
		XMFLOAT3 uv2 = sky_Box[i + 2].UV;

		XMFLOAT4 deltaPos1 = XMFLOAT4(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z, v1.w - v0.w);
		XMFLOAT4 deltaPos2 = XMFLOAT4(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z, v2.w - v0.w);

		XMFLOAT3 deltaUV1 = XMFLOAT3(uv1.x - uv0.x, uv1.y - uv0.y, uv1.z - uv0.z);
		XMFLOAT3 deltaUV2 = XMFLOAT3(uv2.x - uv0.x, uv2.y - uv0.y, uv2.z - uv0.z);

		float ratio = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		XMFLOAT4 deltaPos1uv2 = XMFLOAT4(deltaPos1.x * deltaUV2.y, deltaPos1.y * deltaUV2.y, deltaPos1.z * deltaUV2.y, deltaPos1.w * deltaUV2.y);
		XMFLOAT4 deltaPos2uv1 = XMFLOAT4(deltaPos2.x * deltaUV1.y, deltaPos2.y * deltaUV1.y, deltaPos2.z * deltaUV1.y, deltaPos2.w * deltaUV1.y);

		XMFLOAT3 tangent = XMFLOAT3((deltaPos1uv2.x - deltaPos2uv1.x) * ratio, (deltaPos1uv2.y - deltaPos2uv1.y) * ratio,
			(deltaPos1uv2.z - deltaPos2uv1.z) * ratio);

		XMFLOAT4 deltaPos2uv1b = XMFLOAT4(deltaPos2.x * deltaUV1.x, deltaPos2.y * deltaUV1.x, deltaPos2.z * deltaUV1.x, deltaPos2.x * deltaUV1.x);
		XMFLOAT4 deltaPos1uv2b = XMFLOAT4(deltaPos1.x * deltaUV2.x, deltaPos1.y * deltaUV2.x, deltaPos1.z * deltaUV2.x, deltaPos1.w * deltaUV2.x);

		XMFLOAT3 bitangent = XMFLOAT3((deltaPos2uv1b.x - deltaPos1uv2b.x) * ratio, (deltaPos2uv1b.y - deltaPos1uv2b.y) * ratio,
			(deltaPos2uv1b.z - deltaPos1uv2b.z) * ratio);


		sky_Box[i + 0].Tangent = tangent;
		sky_Box[i + 1].Tangent = tangent;
		sky_Box[i + 2].Tangent = tangent;

		sky_Box[i + 0].BiTangent = tangent;
		sky_Box[i + 1].BiTangent = tangent;
		sky_Box[i + 2].BiTangent = tangent;
	}
#pragma region VertexBuffer for Skybox
	D3D11_BUFFER_DESC Sky_BoxbufferDesc;
	ZeroMemory(&Sky_BoxbufferDesc, sizeof(Sky_BoxbufferDesc));
	Sky_BoxbufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	Sky_BoxbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Sky_BoxbufferDesc.ByteWidth = (UINT)(sizeof(VERTEX) * vert_indices.size());

	D3D11_SUBRESOURCE_DATA sub_data_Sky_Box;
	ZeroMemory(&sub_data_Sky_Box, sizeof(sub_data_Sky_Box));
	sub_data_Sky_Box.pSysMem = sky_Box;
	g_pd3dDevice->CreateBuffer(&Sky_BoxbufferDesc, &sub_data_Sky_Box, &VertexBufferSkyBox);
#pragma endregion

#pragma region IndexBuffer for SkyBox
	D3D11_BUFFER_DESC indexBuffDesc_SkyBox;
	ZeroMemory(&indexBuffDesc_SkyBox, sizeof(indexBuffDesc_SkyBox));
	indexBuffDesc_SkyBox.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc_SkyBox.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc_SkyBox.ByteWidth = (UINT)(sizeof(unsigned int) * vert_indices.size());

	D3D11_SUBRESOURCE_DATA indexData_SkyBox;
	ZeroMemory(&indexData_SkyBox, sizeof(indexData_SkyBox));
	indexData_SkyBox.pSysMem = SkyBoxIndices;
	g_pd3dDevice->CreateBuffer(&indexBuffDesc_SkyBox, &indexData_SkyBox, &IndexBufferSkyBox);
#pragma endregion

	SkyBoxIndexCount = (unsigned int)vert_indices.size();
	delete[] SkyBoxIndices;
	delete[] sky_Box;

#pragma endregion

	verts.clear();
	uvs.clear();
	norms.clear();
	vert_indices.clear();
	uvs_indices.clear();
	norm_indices.clear();

	/*StoreFBXDLLinBin("Box_BindPose.fbx", "Box_BindPose.bin");
	StoreFBXDLLinBin("SwordFBX.fbx", "SwordFBX.bin");
	StoreFBXDLLinBin("Teddy_Attack1.fbx", "Teddy_Attack1.bin");*/

#pragma region Creating DeadpoolSword
	//CreateVertexIndexBufferModel(&VertexBufferSword, &IndexBufferSword, g_pd3dDevice, "deadpool sword 1.obj", &SwordIndexCount);
	//thread thread1(&DEMO_APP::CreateVertexIndexBufferModel1, this, &VertexBufferSword, &IndexBufferSword, g_pd3dDevice, "SwordFBX.bin", &SwordIndexCount);
	//thread thread1(&DEMO_APP::CreateVertexIndexBufferModel1, this, &VertexBufferSword, &IndexBufferSword, g_pd3dDevice, "SwordFBX.fbx", &SwordIndexCount);
#pragma endregion

#pragma region Creating Deadpool
	//CreateVertexIndexBufferModel(&VertexBufferDeadpool, &IndexBufferDeadpool, g_pd3dDevice, "deadpool.obj", &DeadpoolIndexCount);
	thread thread2(&DEMO_APP::CreateVertexIndexBufferModel, this, &VertexBufferDeadpool, &IndexBufferDeadpool, g_pd3dDevice, "deadpool.obj", &DeadpoolIndexCount);
#pragma endregion

#pragma region creating LightSource
	LoadModel::LoadObj("LightSource.obj", verts, uvs, norms,
		vert_indices, uvs_indices, norm_indices);

	SIMPLE_VERTEX* lightSource = new SIMPLE_VERTEX[vert_indices.size()];
	unsigned int* lightSourceIndices = new unsigned int[vert_indices.size()];

	for (unsigned int i = 0; i < vert_indices.size(); i++)
	{
		lightSource[i].XYZW = verts[vert_indices[i]];
		lightSource[i].RGBA = XMFLOAT4(0.001f, 0.75f, 0.40f, 1.0f);
		lightSourceIndices[i] = i;
	}

#pragma region VertexBuffer for LightSource
	D3D11_BUFFER_DESC LightSourcebufferDesc;
	ZeroMemory(&LightSourcebufferDesc, sizeof(LightSourcebufferDesc));
	LightSourcebufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	LightSourcebufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	LightSourcebufferDesc.ByteWidth = (UINT)(sizeof(SIMPLE_VERTEX) * vert_indices.size());

	D3D11_SUBRESOURCE_DATA sub_data_LightSource;
	ZeroMemory(&sub_data_LightSource, sizeof(sub_data_LightSource));
	sub_data_LightSource.pSysMem = lightSource;
	g_pd3dDevice->CreateBuffer(&LightSourcebufferDesc, &sub_data_LightSource, &VertexBufferSource);
#pragma endregion

#pragma region IndexBuffer for LightSource
	D3D11_BUFFER_DESC indexBuffDesc_LightSource;
	ZeroMemory(&indexBuffDesc_LightSource, sizeof(indexBuffDesc_LightSource));
	indexBuffDesc_LightSource.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc_LightSource.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc_LightSource.ByteWidth = (UINT)(sizeof(unsigned int) * vert_indices.size());

	D3D11_SUBRESOURCE_DATA indexData_LightSource;
	ZeroMemory(&indexData_LightSource, sizeof(indexData_LightSource));
	indexData_LightSource.pSysMem = lightSourceIndices;
	g_pd3dDevice->CreateBuffer(&indexBuffDesc_LightSource, &indexData_LightSource, &IndexBufferSource);

#pragma endregion

	SourceIndexCount = (unsigned int)vert_indices.size();
	delete[] lightSource;
	delete[] lightSourceIndices;

#pragma endregion

	verts.clear();
	uvs.clear();
	norms.clear();
	vert_indices.clear();
	uvs_indices.clear();
	norm_indices.clear();

#pragma region VertexBuffer for Star
	D3D11_BUFFER_DESC StarbufferDesc;
	ZeroMemory(&StarbufferDesc, sizeof(StarbufferDesc));
	StarbufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	StarbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	StarbufferDesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 12;

	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(sub_data));
	sub_data.pSysMem = Star;
	g_pd3dDevice->CreateBuffer(&StarbufferDesc, &sub_data, &VertexBufferStar);
#pragma endregion

#pragma region IndexBuffer for Star
	D3D11_BUFFER_DESC indexBuffDesc;
	ZeroMemory(&indexBuffDesc, sizeof(indexBuffDesc));
	indexBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc.ByteWidth = sizeof(indice);

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = &indice;
	g_pd3dDevice->CreateBuffer(&indexBuffDesc, &indexData, &IndexBufferStar);
#pragma endregion

#pragma region ShaderData
	//Regular Shaders
	g_pd3dDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &DirectVertShader[0]);
	g_pd3dDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &DirectPixShader[0]);
	g_pd3dDevice->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &DirectVertShader[1]);
	g_pd3dDevice->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &DirectPixShader[1]);

	////Geo Shaders
	//g_pd3dDevice->CreateVertexShader(Geo_VS, sizeof(Geo_VS), NULL, &VertexShaderGeo);
	//g_pd3dDevice->CreateGeometryShader(Geo_GS, sizeof(Geo_GS), NULL, &GeometryShaderGeo);
	//g_pd3dDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &DirectVertShader[0]);

	g_pd3dDevice->CreateVertexShader(VertexTriangleShader, sizeof(VertexTriangleShader), NULL, &vertexShaderTriangle);
	g_pd3dDevice->CreatePixelShader(PixelTriangleShader, sizeof(PixelTriangleShader), NULL, &pixelShaderTriangle);
	g_pd3dDevice->CreateHullShader(HullTriangleShader, sizeof(HullTriangleShader), NULL, &hullShaderTriangle);
	g_pd3dDevice->CreateDomainShader(DomainTriangleShader, sizeof(DomainTriangleShader), NULL, &domainShaderTriangle);


	//Instance Shader
	g_pd3dDevice->CreateVertexShader(VertexShaderInstance, sizeof(VertexShaderInstance), NULL, &DirectVertShader[2]);
	//Skinning Shader
	g_pd3dDevice->CreateVertexShader(SkinnedVertexShader, sizeof(SkinnedVertexShader), NULL, &SkinningShader);

	D3D11_INPUT_ELEMENT_DESC LayoutComplex[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC LayoutSimple[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	D3D11_INPUT_ELEMENT_DESC Layout_Skinned[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	g_pd3dDevice->CreateInputLayout(LayoutComplex, 5, Trivial_VS, sizeof(Trivial_VS), &DirectInputLay[0]);
	g_pd3dDevice->CreateInputLayout(LayoutSimple, 3, VertexShader, sizeof(VertexShader), &DirectInputLay[1]);
	g_pd3dDevice->CreateInputLayout(Layout_Skinned, 6, SkinnedVertexShader, sizeof(SkinnedVertexShader), &SkinningVertexLayout);
#pragma endregion

#pragma region Costant Buffers

	D3D11_BUFFER_DESC BufferDesc2;
	ZeroMemory(&BufferDesc2, sizeof(BufferDesc2));
	BufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc2.ByteWidth = sizeof(SEND_TO_VRAM_WORLD);
	BufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pd3dDevice->CreateBuffer(&BufferDesc2, NULL, &constantBuffer[0]);

	D3D11_BUFFER_DESC BufferDesc5;
	ZeroMemory(&BufferDesc5, sizeof(BufferDesc5));
	BufferDesc5.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc5.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc5.ByteWidth = sizeof(TRANSLATOR);
	BufferDesc5.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pd3dDevice->CreateBuffer(&BufferDesc5, NULL, &constantBuffer[1]);

	D3D11_BUFFER_DESC BufferDesc3;
	ZeroMemory(&BufferDesc3, sizeof(BufferDesc3));
	BufferDesc3.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc3.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc3.ByteWidth = sizeof(SEND_TO_VRAM_PIXEL);
	BufferDesc3.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pd3dDevice->CreateBuffer(&BufferDesc3, NULL, &constantPixelBuffer);

	D3D11_BUFFER_DESC LightDesc;
	ZeroMemory(&LightDesc, sizeof(LightDesc));
	LightDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightDesc.ByteWidth = sizeof(LightSources) * 4;
	LightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pd3dDevice->CreateBuffer(&LightDesc, NULL, &CostantBufferLights);

	D3D11_BUFFER_DESC InstanceDesc;
	ZeroMemory(&InstanceDesc, sizeof(InstanceDesc));
	InstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	InstanceDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	InstanceDesc.ByteWidth = sizeof(Instance) * 4;
	InstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pd3dDevice->CreateBuffer(&InstanceDesc, NULL, &InstanceCostantBuffer);

	D3D11_BUFFER_DESC SkinningDesc;
	ZeroMemory(&SkinningDesc, sizeof(SkinningDesc));
	SkinningDesc.Usage = D3D11_USAGE_DYNAMIC;
	SkinningDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	SkinningDesc.ByteWidth = sizeof(cBufferSkeleton);
	SkinningDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pd3dDevice->CreateBuffer(&SkinningDesc, NULL, &MageSkeleonBuffer);

	D3D11_BUFFER_DESC TesselDesc;
	ZeroMemory(&TesselDesc, sizeof(TesselDesc));
	TesselDesc.Usage = D3D11_USAGE_DYNAMIC;
	TesselDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	TesselDesc.ByteWidth = sizeof(Scaling);
	TesselDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pd3dDevice->CreateBuffer(&TesselDesc, NULL, &CostantBufferTessScale);

#pragma endregion

#pragma region Depth Buffer
	//Creating DepthStencil initlization
	//DXGI_FORMAT_R32G32B32A32_FLOAT
	//DXGI_FORMAT_D32_FLOAT
	D3D11_TEXTURE2D_DESC texture2D;
	ZeroMemory(&texture2D, sizeof(texture2D));
	texture2D.Width = (UINT)BACKBUFFER_WIDTH;
	texture2D.Height = (UINT)BACKBUFFER_HEIGHT;
	texture2D.Usage = D3D11_USAGE_DEFAULT;
	texture2D.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2D.Format = DXGI_FORMAT_D32_FLOAT;
	texture2D.MipLevels = 1;
	texture2D.ArraySize = 1;
	texture2D.SampleDesc.Count = 4;
	g_pd3dDevice->CreateTexture2D(&texture2D, NULL, &g_TexBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC stencil;
	ZeroMemory(&stencil, sizeof(stencil));
	stencil.Format = DXGI_FORMAT_D32_FLOAT;
	stencil.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;		//use this enum to incorporate sampleDecs.count/.Quality values otherwise use D3D11_DSV_DIMENSION_TEXTURE2D
	stencil.Texture2D.MipSlice = 0;
	g_pd3dDevice->CreateDepthStencilView(g_TexBuffer, &stencil, &g_StencilView);
#pragma endregion

#pragma region Default RasterState
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.SlopeScaledDepthBias = 0;
	rasterDesc.DepthBiasClamp = 0;
	rasterDesc.ScissorEnable = false;
	rasterDesc.MultisampleEnable = true;

	g_pd3dDevice->CreateRasterizerState(&rasterDesc, &DefaultRasterState);
#pragma endregion

#pragma region SkyBox RasterState
	rasterDesc.CullMode = D3D11_CULL_NONE;
	g_pd3dDevice->CreateRasterizerState(&rasterDesc, &SkyBoxRasterState);
#pragma endregion

#pragma region BlendState
	D3D11_BLEND_DESC BlendDecs;
	BlendDecs.AlphaToCoverageEnable = true;
	BlendDecs.IndependentBlendEnable = false;
	BlendDecs.RenderTarget[0].BlendEnable = true;
	BlendDecs.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDecs.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDecs.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDecs.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDecs.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDecs.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDecs.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	g_pd3dDevice->CreateBlendState(&BlendDecs, &BlendState);

#pragma endregion

#pragma region Textures
	CreateDDSTextureFromFile(g_pd3dDevice, L"SkyBox.dds", NULL, &SkyBoxShaderView);
	CreateDDSTextureFromFile(g_pd3dDevice, L"brickwall.dds", NULL, &FloorShaderView);
	CreateDDSTextureFromFile(g_pd3dDevice, L"DeadPoolSword.dds", NULL, &SwordShaderView);
	CreateDDSTextureFromFile(g_pd3dDevice, L"DeadPoolTex.dds", NULL, &DeadpoolShaderView);
	CreateDDSTextureFromFile(g_pd3dDevice, L"DeadPool_NORMAL.dds", NULL, &DeadpoolNORMShaderView);
	CreateDDSTextureFromFile(g_pd3dDevice, L"BrickWall_NORMAL.dds", NULL, &FloorNORMShaderView);
	CreateDDSTextureFromFile(g_pd3dDevice, L"DeadpoolSword_Normal.dds", NULL, &SwordNORMShaderView);
	CreateDDSTextureFromFile(g_pd3dDevice, L"TestCube.dds", NULL, &BindPoseTexture);
	CreateDDSTextureFromFile(g_pd3dDevice, L"NormBindPoseTexture.dds", NULL, &BindPoseNormTexture);
	CreateDDSTextureFromFile(g_pd3dDevice, L"PPG.dds", NULL, &MagePoseTexture);
	CreateDDSTextureFromFile(g_pd3dDevice, L"PPG_N.dds", NULL, &MagePoseNormTexture);
#pragma endregion

#pragma region Sampler

	D3D11_SAMPLER_DESC Sample = {};
	Sample.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	Sample.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	Sample.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	g_pd3dDevice->CreateSamplerState(&Sample, &sampleTexture);

#pragma endregion

#pragma region Lights INIT

#if USINGOLDLIGHTCODE

	//Directional Light				//somewhat blue color
	Lights.m_DirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f + 0.3f, 1.0f);
	Lights.m_DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f + 0.3f, 1.0f);
	Lights.m_DirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f + 0.3f, 1.0f);
	Lights.m_DirLight.Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//Point Light will be postioned on the left		//somewhat green
	Lights.m_pointLight.Ambient = XMFLOAT4(0.5f, 0.7f, 0.5f, 1.0f);
	Lights.m_pointLight.Diffuse = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f);
	Lights.m_pointLight.Specular = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f);
	Lights.m_pointLight.Att = XMFLOAT3(0.0f, .5f, 0.0f);

	Lights.m_pointLight.Range = 100.0f;
	Lights.m_pointLight.Position = XMFLOAT3(1.0f, .0f, 0.0f);

	//Spot Light will be positioned on the right	//somewhat red
	Lights.m_SpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	Lights.m_SpotLight.Diffuse = XMFLOAT4(0.8f, 0.2f, 0.2f, 1.0f);
	Lights.m_SpotLight.Specular = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	Lights.m_SpotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Lights.m_SpotLight.Position = XMFLOAT3(3.0f, 3.0, 0.0f);
	Lights.m_SpotLight.Spot = 30.0f;
	Lights.m_SpotLight.Range = 20.0f;

	//Materials
	Lights.m_Material.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	Lights.m_Material.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	Lights.m_Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

#endif

#if !USINGOLDLIGHTCODE

	//Directional Light
	Lights[0].Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1);
	Lights[0].Direction = XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f);
	Lights[0].Color = XMFLOAT4(0.8f, 0.8f, .5f, 1.0f);
	Lights[0].Radius = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	//Point Light
	Lights[1].Position = XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f);
	Lights[1].Direction = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	Lights[1].Color = XMFLOAT4(0.001f, .75f, .40f, 1.0f);
	Lights[1].Radius = XMFLOAT4(1.0f, 0.0f, 0.0f, 20.0f);
	//Spot Light
	Lights[2].Position = XMFLOAT4(3.0f, 1.0f, 0.0, 1.0f);
	Lights[2].Direction = XMFLOAT4(0.0, -1.0f, .7f, 0.0f);
	Lights[2].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Lights[2].Radius = XMFLOAT4(1.0f, 0.95f, 0.9f, .9f); //w = radius y = inner z = outer
														 //Ambient Light
	Lights[3].Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	Lights[3].Direction = XMFLOAT4(0.0, -1.0f, 1.0f, 0.0f);
	Lights[3].Color = XMFLOAT4(0.001f, 0.001f, 1.0f, 1.0f);
	Lights[3].Radius = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
#endif

#pragma endregion

#pragma region geometry Shader
	////Geometry Shader

	//VERTEX Geo_Vert[1];
	//Geo_Vert[0].XYZW = XMFLOAT4(0, 0, 0, 1);
	//Geo_Vert[0].UV = XMFLOAT3(0, 0, 0);
	//Geo_Vert[0].normals = XMFLOAT3(0, 0, 0);

	//D3D11_BUFFER_DESC GeobufferDesc;
	//ZeroMemory(&GeobufferDesc, sizeof(GeobufferDesc));
	//GeobufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//GeobufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//GeobufferDesc.ByteWidth = sizeof(VERTEX);

	//D3D11_SUBRESOURCE_DATA Geo_data;
	//ZeroMemory(&Geo_data, sizeof(Geo_data));
	//Geo_data.pSysMem = &Geo_Vert;
	//g_pd3dDevice->CreateBuffer(&GeobufferDesc, &Geo_data, &VertexBufferGeo);



#pragma endregion	

#pragma region instancingInit
	for (int i = 0; i < 4; i++)
	{
		list[i].WorldLocation = XMMatrixIdentity();
	}

	list[0].WorldLocation = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	list[1].WorldLocation = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	list[2].WorldLocation = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
	list[3].WorldLocation = XMMatrixTranslation(0.0f, 0.0f, -2.0f);


	D3D11_BUFFER_DESC instanceBufferDesc;
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(Instance) * 4;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA instanceData;
	ZeroMemory(&instanceData, sizeof(instanceData));
	instanceData.pSysMem = list;

	g_pd3dDevice->CreateBuffer(&instanceBufferDesc, &instanceData, &InstanceBuffer);

	//CreateVertexIndexBufferModel(&DeadpoolInstanceVertexBuffer, &DeadpoolInstanceIndexBuffer, g_pd3dDevice, "DeadpoolInstance.obj", &InstanceIndexCount);
	thread thread3(&DEMO_APP::CreateVertexIndexBufferModel, this, &DeadpoolInstanceVertexBuffer, &DeadpoolInstanceIndexBuffer, g_pd3dDevice, "DeadpoolInstance.obj", &InstanceIndexCount);

#pragma endregion

#pragma region Tesselation

	VERTEX Triangle_Vert[3];
	Triangle_Vert[0].XYZW = XMFLOAT4(2.0f, 3.0f, 0.0f, 1.0f);
	Triangle_Vert[0].UV = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Triangle_Vert[0].normals = XMFLOAT3(0.0f, 0.0f, -1.0f);

	Triangle_Vert[1].XYZW = XMFLOAT4(3.0f, 1.0f, 0.0f, 1.0f);
	Triangle_Vert[1].UV = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Triangle_Vert[1].normals = XMFLOAT3(0.0f, 0.0f, -1.0f);

	Triangle_Vert[2].XYZW = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	Triangle_Vert[2].UV = XMFLOAT3(1.0f, 0.5f, 0.0f);
	Triangle_Vert[2].normals = XMFLOAT3(0.0f, 0.0f, -1.0f);

	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Usage = D3D11_USAGE_IMMUTABLE;
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.ByteWidth = sizeof(VERTEX) * 3;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = Triangle_Vert;
	g_pd3dDevice->CreateBuffer(&Desc, &data, &vertexBufferTriangle);

	TriangleMatrix.WorldLocation = XMMatrixTranslation(0.0f, 0.0f, -2.0f);

	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.SlopeScaledDepthBias = 0;
	rasterDesc.ScissorEnable = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.DepthClipEnable = true;

	g_pd3dDevice->CreateRasterizerState(&rasterDesc, &RasterStateWireFrameTriangle);

	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_SOLID;

	g_pd3dDevice->CreateRasterizerState(&rasterDesc, &RasterStateSoildTriangle);

#pragma endregion

	//thread1.detach();
	thread2.detach();
	thread3.detach();

#pragma region TEXT 
	spritebatch.reset(new SpriteBatch(g_pd3dDeviceContext));
	m_textFont.reset(new SpriteFont(g_pd3dDevice, L"Arial.spritefont"));
	CreateDDSTextureFromFile(g_pd3dDevice, L"Green_Button.dds", NULL, &m_text);
	CreateDDSTextureFromFile(g_pd3dDevice, L"Red_Button.dds", NULL, &m_text2);
	CreateDDSTextureFromFile(g_pd3dDevice, L"textbox.dds", NULL, &m_textbox);
	audio.reset(new AudioEngine(AudioEngine_Default));
	sound.reset(new SoundEffect(audio.get(), L"LightSound.wav"));
	InitInput();
#pragma endregion


	//thread thread4(&DEMO_APP::CreateVertexIndexBufferModel1, this, &BindPoseVertex, &BindPoseIndex, g_pd3dDevice, "Box_BindPose.bin", &BindPoseIndexCount);
	//thread4.join();
	//thread thread4(&DEMO_APP::CreateVertexIndexBufferModel1, this, &BindPoseVertex, &BindPoseIndex, g_pd3dDevice, "Box_BindPose.fbx", &BindPoseIndexCount);
	//thread4.join();
	//thread thread5(&DEMO_APP::CreateVertexIndexBufferModel1, this, &TeddyPoseVertex, &TeddyPoseIndex, g_pd3dDevice, "Teddy_Attack1.bin", &TeddyPoseIndexCount);
	//thread5.detach();

	thread thread5(&DEMO_APP::CreateSkinnedVertexIndexBufferModel, this, &MagePoseVertex, &MagePoseIndex, g_pd3dDevice, "Walk.fbx", "Run.fbx", &MagePoseIndexCount, &MageAnimation);
	thread5.detach();
	TimeWizard.Restart();

	bSplitScreen = false;
}

void DEMO_APP::init3D(HWND hWnd)
{

#pragma region Initail stuff
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.BufferDesc.Width = (UINT)BACKBUFFER_WIDTH;
	scd.BufferDesc.Height = (UINT)BACKBUFFER_HEIGHT;
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
															//scd.SampleDesc.Quality = 1;								
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = 0; //DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// Special Flags
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	unsigned int flags = 0;
#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&g_pSwapChain,
		&g_pd3dDevice,
		NULL,
		&g_pd3dDeviceContext);
#pragma endregion

#pragma region BackBuffer Stuff
	//Getting Temp BackBuffer to reference
	ID3D11Texture2D* tempBuffer;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tempBuffer);

	//using the tempbuffer address to created ID3D11RenderTargetView object
	g_pd3dDevice->CreateRenderTargetView(tempBuffer, NULL, &g_pRenderTargetView);
	//I don't need this anymore so I release it to the wild
	tempBuffer->Release();

	//Setting Rendertarget as back buffer
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
#pragma endregion

#pragma region Viewport
	//Setting VeiwPort
	ZeroMemory(&g_DirectView, sizeof(D3D11_VIEWPORT));

	g_DirectView.TopLeftX = 0;
	g_DirectView.TopLeftY = 0;
	g_DirectView.Width = BACKBUFFER_WIDTH;
	g_DirectView.Height = BACKBUFFER_HEIGHT;
	g_DirectView.MinDepth = 0;
	g_DirectView.MaxDepth = 1.0f;

#pragma endregion

}

void DEMO_APP::CreateVertexIndexBufferModel(ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer, ID3D11Device* device, const char* Path,
	unsigned int* IndexCount)
{

	vector<XMFLOAT4> verts;
	vector<XMFLOAT3> norms;
	vector<XMFLOAT3> uvs;
	vector<unsigned int> vert_indices, norm_indices, uvs_indices;

	LoadModel::LoadObj(Path, verts, uvs, norms,
		vert_indices, uvs_indices, norm_indices);

	VERTEX* Model = new VERTEX[vert_indices.size()];
	unsigned int* ModelIndices = new unsigned int[vert_indices.size()];

	for (unsigned int i = 0; i < vert_indices.size(); i++)
	{
		Model[i].XYZW = verts[vert_indices[i]];
		Model[i].UV = uvs[uvs_indices[i]];
		Model[i].normals = norms[norm_indices[i]];
		ModelIndices[i] = i;
	}

	for (unsigned int i = 0; i < vert_indices.size(); i += 3)
	{
		XMFLOAT4 v0 = Model[i + 0].XYZW;
		XMFLOAT4 v1 = Model[i + 1].XYZW;
		XMFLOAT4 v2 = Model[i + 2].XYZW;

		XMFLOAT3 uv0 = Model[i + 0].UV;
		XMFLOAT3 uv1 = Model[i + 1].UV;
		XMFLOAT3 uv2 = Model[i + 2].UV;

		XMFLOAT4 deltaPos1 = XMFLOAT4(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z, v1.w - v0.w);
		XMFLOAT4 deltaPos2 = XMFLOAT4(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z, v2.w - v0.w);

		XMFLOAT3 deltaUV1 = XMFLOAT3(uv1.x - uv0.x, uv1.y - uv0.y, uv1.z - uv0.z);
		XMFLOAT3 deltaUV2 = XMFLOAT3(uv2.x - uv0.x, uv2.y - uv0.y, uv2.z - uv0.z);

		float ratio = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		XMFLOAT4 deltaPos1uv2 = XMFLOAT4(deltaPos1.x * deltaUV2.y, deltaPos1.y * deltaUV2.y, deltaPos1.z * deltaUV2.y, deltaPos1.w * deltaUV2.y);
		XMFLOAT4 deltaPos2uv1 = XMFLOAT4(deltaPos2.x * deltaUV1.y, deltaPos2.y * deltaUV1.y, deltaPos2.z * deltaUV1.y, deltaPos2.w * deltaUV1.y);

		XMFLOAT3 tangent = XMFLOAT3((deltaPos1uv2.x - deltaPos2uv1.x) * ratio, (deltaPos1uv2.y - deltaPos2uv1.y) * ratio,
			(deltaPos1uv2.z - deltaPos2uv1.z) * ratio);

		XMFLOAT4 deltaPos2uv1b = XMFLOAT4(deltaPos2.x * deltaUV1.x, deltaPos2.y * deltaUV1.x, deltaPos2.z * deltaUV1.x, deltaPos2.x * deltaUV1.x);
		XMFLOAT4 deltaPos1uv2b = XMFLOAT4(deltaPos1.x * deltaUV2.x, deltaPos1.y * deltaUV2.x, deltaPos1.z * deltaUV2.x, deltaPos1.w * deltaUV2.x);

		XMFLOAT3 bitangent = XMFLOAT3((deltaPos2uv1b.x - deltaPos1uv2b.x) * ratio, (deltaPos2uv1b.y - deltaPos1uv2b.y) * ratio,
			(deltaPos2uv1b.z - deltaPos1uv2b.z) * ratio);


		Model[i + 0].Tangent = tangent;
		Model[i + 1].Tangent = tangent;
		Model[i + 2].Tangent = tangent;

		Model[i + 0].BiTangent = tangent;
		Model[i + 1].BiTangent = tangent;
		Model[i + 2].BiTangent = tangent;
	}

#pragma region VertexBuffer Model

	D3D11_BUFFER_DESC ModelbufferDesc;
	ZeroMemory(&ModelbufferDesc, sizeof(ModelbufferDesc));
	ModelbufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ModelbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ModelbufferDesc.ByteWidth = (UINT)(sizeof(VERTEX) * vert_indices.size());

	D3D11_SUBRESOURCE_DATA sub_data_Model;
	ZeroMemory(&sub_data_Model, sizeof(sub_data_Model));
	sub_data_Model.pSysMem = Model;
	device->CreateBuffer(&ModelbufferDesc, &sub_data_Model, VertexBuffer);

#pragma endregion

#pragma region IndexBuffer Model

	D3D11_BUFFER_DESC indexBuffDesc_Model;
	ZeroMemory(&indexBuffDesc_Model, sizeof(indexBuffDesc_Model));
	indexBuffDesc_Model.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc_Model.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc_Model.ByteWidth = (UINT)(sizeof(unsigned int) * vert_indices.size());

	D3D11_SUBRESOURCE_DATA indexData_Model;
	ZeroMemory(&indexData_Model, sizeof(indexData_Model));
	indexData_Model.pSysMem = ModelIndices;
	device->CreateBuffer(&indexBuffDesc_Model, &indexData_Model, IndexBuffer);

#pragma endregion

	*IndexCount = (unsigned int)vert_indices.size();
	delete[] Model;
	delete[] ModelIndices;

	verts.clear();
	uvs.clear();
	norms.clear();
	vert_indices.clear();
	uvs_indices.clear();
	norm_indices.clear();
}

void DEMO_APP::CreateVertexIndexBufferModel1(ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer, ID3D11Device* device, const char* Path,
	unsigned int* IndexCount)
{

	vector<XMFLOAT4> verts;
	vector<XMFLOAT3> norms;
	vector<XMFLOAT3> uvs;
	vector<XMFLOAT3> tangent;
	vector<unsigned int> vert_indices, norm_indices, uvs_indices;

	//LoadModel::LoadObj(Path, verts, uvs, norms,
	//	vert_indices, uvs_indices, norm_indices);
	//verts.clear();
	//norms.clear();
	//uvs.clear();
	LoadModel::LoadFBXinBin(Path, verts, uvs, norms, tangent);
	//LoadModel::LoadFBX(Path, verts, uvs, norms, tangent);

	VERTEX* Model = new VERTEX[verts.size()];
	unsigned int* ModelIndices = new unsigned int[verts.size()];

	for (unsigned int i = 0; i < verts.size(); i++)
	{
		Model[i].XYZW = verts[i];//vert[vert_indices[i]];
		Model[i].UV = uvs[i];//uvs[uvs_indices[i]];
		Model[i].normals = norms[i];
		ModelIndices[i] = i;
		//Model[i].Tangent = tangent[i];
	}


	for (unsigned int i = 0; i < verts.size() - 2; i += 3)
	{
		XMFLOAT4 v0 = Model[i + 0].XYZW;
		XMFLOAT4 v1 = Model[i + 1].XYZW;
		XMFLOAT4 v2 = Model[i + 2].XYZW;

		XMFLOAT3 uv0 = Model[i + 0].UV;
		XMFLOAT3 uv1 = Model[i + 1].UV;
		XMFLOAT3 uv2 = Model[i + 2].UV;

		XMFLOAT4 deltaPos1 = XMFLOAT4(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z, v1.w - v0.w);
		XMFLOAT4 deltaPos2 = XMFLOAT4(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z, v2.w - v0.w);

		XMFLOAT3 deltaUV1 = XMFLOAT3(uv1.x - uv0.x, uv1.y - uv0.y, uv1.z - uv0.z);
		XMFLOAT3 deltaUV2 = XMFLOAT3(uv2.x - uv0.x, uv2.y - uv0.y, uv2.z - uv0.z);

		float ratio = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		XMFLOAT4 deltaPos1uv2 = XMFLOAT4(deltaPos1.x * deltaUV2.y, deltaPos1.y * deltaUV2.y, deltaPos1.z * deltaUV2.y, deltaPos1.w * deltaUV2.y);
		XMFLOAT4 deltaPos2uv1 = XMFLOAT4(deltaPos2.x * deltaUV1.y, deltaPos2.y * deltaUV1.y, deltaPos2.z * deltaUV1.y, deltaPos2.w * deltaUV1.y);

		XMFLOAT3 tangent = XMFLOAT3((deltaPos1uv2.x - deltaPos2uv1.x) * ratio, (deltaPos1uv2.y - deltaPos2uv1.y) * ratio,
			(deltaPos1uv2.z - deltaPos2uv1.z) * ratio);

		XMFLOAT4 deltaPos2uv1b = XMFLOAT4(deltaPos2.x * deltaUV1.x, deltaPos2.y * deltaUV1.x, deltaPos2.z * deltaUV1.x, deltaPos2.x * deltaUV1.x);
		XMFLOAT4 deltaPos1uv2b = XMFLOAT4(deltaPos1.x * deltaUV2.x, deltaPos1.y * deltaUV2.x, deltaPos1.z * deltaUV2.x, deltaPos1.w * deltaUV2.x);

		XMFLOAT3 bitangent = XMFLOAT3((deltaPos2uv1b.x - deltaPos1uv2b.x) * ratio, (deltaPos2uv1b.y - deltaPos1uv2b.y) * ratio,
			(deltaPos2uv1b.z - deltaPos1uv2b.z) * ratio);


		Model[i + 0].Tangent = tangent;
		Model[i + 1].Tangent = tangent;
		Model[i + 2].Tangent = tangent;

		//Model[i + 0].BiTangent = bitangent;
		//Model[i + 1].BiTangent = bitangent;
		//Model[i + 2].BiTangent = bitangent;
	}
#pragma region VertexBuffer Model

	D3D11_BUFFER_DESC ModelbufferDesc;
	ZeroMemory(&ModelbufferDesc, sizeof(ModelbufferDesc));
	ModelbufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ModelbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ModelbufferDesc.ByteWidth = (UINT)(sizeof(VERTEX) * verts.size());

	D3D11_SUBRESOURCE_DATA sub_data_Model;
	ZeroMemory(&sub_data_Model, sizeof(sub_data_Model));
	sub_data_Model.pSysMem = Model;
	device->CreateBuffer(&ModelbufferDesc, &sub_data_Model, VertexBuffer);

#pragma endregion

#pragma region IndexBuffer Model

	D3D11_BUFFER_DESC indexBuffDesc_Model;
	ZeroMemory(&indexBuffDesc_Model, sizeof(indexBuffDesc_Model));
	indexBuffDesc_Model.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc_Model.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc_Model.ByteWidth = (UINT)(sizeof(unsigned int) * verts.size());

	D3D11_SUBRESOURCE_DATA indexData_Model;
	ZeroMemory(&indexData_Model, sizeof(indexData_Model));
	indexData_Model.pSysMem = ModelIndices;
	device->CreateBuffer(&indexBuffDesc_Model, &indexData_Model, IndexBuffer);

#pragma endregion

	*IndexCount = (unsigned int)verts.size();
	delete[] Model;
	delete[] ModelIndices;

	verts.clear();
	uvs.clear();
	norms.clear();
	vert_indices.clear();
	uvs_indices.clear();
	norm_indices.clear();
}

void DEMO_APP::CreateSkinnedVertexIndexBufferModel(ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer, ID3D11Device* device, const char* Path, const char* otherAni,
	unsigned int* IndexCount, AnimationController* skeleton)
{

	std::vector<SkinnedVertex> SkinVert;
	Skeleton cSkeleton;
	Skeleton dSkeleton;
	LoadFBXDLLNEW(Path, SkinVert, cSkeleton);
	LoadFBXDLLNEWANIM(otherAni, dSkeleton);
	unsigned int* ModelIndices = new unsigned int[SkinVert.size()];

	for (size_t i = 0; i < SkinVert.size(); i++)
	{
		ModelIndices[i] =(unsigned int) i;
	}


	for (size_t i = 0; i < SkinVert.size() - 2; i += 3)
	{
		XMFLOAT3 v0 = SkinVert[i + 0].m_Positon;
		XMFLOAT3 v1 = SkinVert[i + 1].m_Positon;
		XMFLOAT3 v2 = SkinVert[i + 2].m_Positon;

		XMFLOAT2 uv0 = SkinVert[i + 0].m_UV;
		XMFLOAT2 uv1 = SkinVert[i + 1].m_UV;
		XMFLOAT2 uv2 = SkinVert[i + 2].m_UV;

		XMFLOAT4 deltaPos1 = XMFLOAT4(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z, 0 - 0);
		XMFLOAT4 deltaPos2 = XMFLOAT4(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z, 0 - 0);

		XMFLOAT3 deltaUV1 = XMFLOAT3(uv1.x - uv0.x, uv1.y - uv0.y, 0 - 0);
		XMFLOAT3 deltaUV2 = XMFLOAT3(uv2.x - uv0.x, uv2.y - uv0.y, 0 - 0);

		float ratio = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		XMFLOAT4 deltaPos1uv2 = XMFLOAT4(deltaPos1.x * deltaUV2.y, deltaPos1.y * deltaUV2.y, deltaPos1.z * deltaUV2.y, deltaPos1.w * deltaUV2.y);
		XMFLOAT4 deltaPos2uv1 = XMFLOAT4(deltaPos2.x * deltaUV1.y, deltaPos2.y * deltaUV1.y, deltaPos2.z * deltaUV1.y, deltaPos2.w * deltaUV1.y);

		XMFLOAT3 tangent = XMFLOAT3((deltaPos1uv2.x - deltaPos2uv1.x) * ratio, (deltaPos1uv2.y - deltaPos2uv1.y) * ratio,
			(deltaPos1uv2.z - deltaPos2uv1.z) * ratio);

		/*XMFLOAT4 deltaPos2uv1b = XMFLOAT4(deltaPos2.x * deltaUV1.x, deltaPos2.y * deltaUV1.x, deltaPos2.z * deltaUV1.x, deltaPos2.x * deltaUV1.x);
		XMFLOAT4 deltaPos1uv2b = XMFLOAT4(deltaPos1.x * deltaUV2.x, deltaPos1.y * deltaUV2.x, deltaPos1.z * deltaUV2.x, deltaPos1.w * deltaUV2.x);

		XMFLOAT3 bitangent = XMFLOAT3((deltaPos2uv1b.x - deltaPos1uv2b.x) * ratio, (deltaPos2uv1b.y - deltaPos1uv2b.y) * ratio,
		(deltaPos2uv1b.z - deltaPos1uv2b.z) * ratio);*/

		SkinVert[i + 0].m_Tangent = tangent;
		SkinVert[i + 1].m_Tangent = tangent;
		SkinVert[i + 2].m_Tangent = tangent;

	}
#pragma region VertexBuffer Model

	D3D11_BUFFER_DESC ModelbufferDesc;
	ZeroMemory(&ModelbufferDesc, sizeof(ModelbufferDesc));
	ModelbufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ModelbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ModelbufferDesc.ByteWidth = (UINT)(sizeof(SkinnedVertex) * SkinVert.size());

	D3D11_SUBRESOURCE_DATA sub_data_Model;
	ZeroMemory(&sub_data_Model, sizeof(sub_data_Model));
	sub_data_Model.pSysMem = SkinVert.data();
	device->CreateBuffer(&ModelbufferDesc, &sub_data_Model, VertexBuffer);

#pragma endregion

#pragma region IndexBuffer Model

	D3D11_BUFFER_DESC indexBuffDesc_Model;
	ZeroMemory(&indexBuffDesc_Model, sizeof(indexBuffDesc_Model));
	indexBuffDesc_Model.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc_Model.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc_Model.ByteWidth = (UINT)(sizeof(unsigned int) * SkinVert.size());

	D3D11_SUBRESOURCE_DATA indexData_Model;
	ZeroMemory(&indexData_Model, sizeof(indexData_Model));
	indexData_Model.pSysMem = ModelIndices;
	device->CreateBuffer(&indexBuffDesc_Model, &indexData_Model, IndexBuffer);

#pragma endregion

	AnimationClip Clip;
	AnimationClip Clip2;
	size_t BoneCount = cSkeleton.m_Joints.size();
	size_t BoneCount2 = dSkeleton.m_Joints.size();
	std::vector<BoneAnimation> Ani; // = new BoneAnimation[BoneCount];
	std::vector<BoneAnimation> Ani2; // = new BoneAnimation[BoneCount2];


	for (int i = 0; i < BoneCount; i++)
	{
		KeyFrame* walker;
		BoneAnimation temp;
		walker = cSkeleton.m_Joints[i].m_Animation;
		while (walker)
		{
			temp.Keyframes.push_back(walker);
			walker = walker->m_Next;
		}
		Ani.push_back(temp);
	}
	for (int i = 0; i < BoneCount2; i++)
	{
		KeyFrame* walker;
		BoneAnimation temp;
		walker = dSkeleton.m_Joints[i].m_Animation;
		while (walker)
		{
			temp.Keyframes.push_back(walker);
			walker = walker->m_Next;
		}
		Ani2.push_back(temp);
	}
	for (size_t i = 0; i < BoneCount; i++)
	{
		Clip.BoneAnimations.push_back(Ani[i]);
	}
	for (size_t i = 0; i < BoneCount2; i++)
	{
		Clip2.BoneAnimations.push_back(Ani2[i]);
	}
	skeleton->Anim.push_back(Clip);
	skeleton->Anim.push_back(Clip2);

	*IndexCount = (unsigned int)SkinVert.size();
	animationdone = true;
	delete[] ModelIndices;

}

void DEMO_APP::DrawComplexModel(ID3D11Buffer* VertexBuffer, ID3D11Buffer* IndexBuffer, ID3D11ShaderResourceView* Texture, ID3D11VertexShader* vertexShader
	, ID3D11PixelShader* pixelShader, unsigned int IndexCount, ID3D11DeviceContext* context)
{

	unsigned int Stride = sizeof(VERTEX);
	unsigned int Offset = 0;


}

float DEMO_APP::calcdist(XMVECTOR v1, XMVECTOR v2)
{
	XMVECTOR v;

	v.m128_f32[0] = v1.m128_f32[0] - v2.m128_f32[0];
	v.m128_f32[1] = v1.m128_f32[1] - v2.m128_f32[1];
	v.m128_f32[2] = v1.m128_f32[2] - v2.m128_f32[2];

	return sqrt(v.m128_f32[0] * v.m128_f32[0] + v.m128_f32[1] * v.m128_f32[1] + v.m128_f32[2] * v.m128_f32[2]);
}

void DEMO_APP::ResizingOfWindows()
{
	if (g_pSwapChain == nullptr)
	{
		return;
	}

	g_pRenderTargetView->Release();
	g_StencilView->Release();
	g_TexBuffer->Release();
	//pointer to thr back buffer

	ID3D11Texture2D *tempBuffer;
	g_pd3dDeviceContext->ClearState();
	g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	HRESULT hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tempBuffer);

	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
	g_pSwapChain->GetDesc(&m_swapChainDesc);

	g_DirectView.Width = (FLOAT)m_swapChainDesc.BufferDesc.Width;
	g_DirectView.Height = (FLOAT)m_swapChainDesc.BufferDesc.Height;
	g_DirectView.MinDepth = 0.0f;
	g_DirectView.MaxDepth = 1.0f;
	g_DirectView.TopLeftX = 0;
	g_DirectView.TopLeftY = 0;
	g_pd3dDeviceContext->RSSetViewports(1, &g_DirectView);

	g_pd3dDevice->CreateRenderTargetView(tempBuffer, NULL, &g_pRenderTargetView);

	D3D11_TEXTURE2D_DESC m_texture2d = {};

	D3D11_DEPTH_STENCIL_VIEW_DESC d_stencil = {};

	m_texture2d.Width = (UINT)g_DirectView.Width;
	m_texture2d.Height = (UINT)g_DirectView.Height;
	m_texture2d.Usage = D3D11_USAGE_DEFAULT;
	m_texture2d.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_texture2d.Format = DXGI_FORMAT_D32_FLOAT;
	m_texture2d.MipLevels = 1;
	m_texture2d.ArraySize = 1;
	m_texture2d.SampleDesc.Count = 4;
	g_pd3dDevice->CreateTexture2D(&m_texture2d, NULL, &g_TexBuffer);

	d_stencil.Format = DXGI_FORMAT_D32_FLOAT;
	d_stencil.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d_stencil.Texture2D.MipSlice = 0;
	g_pd3dDevice->CreateDepthStencilView(g_TexBuffer, NULL, &g_StencilView);

	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_StencilView);

	g_newProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), g_DirectView.Width / g_DirectView.Height, ZNEAR, ZFAR);

}

DEMO_APP * DEMO_APP::GetInstance()
{
	static DEMO_APP instance;
	return &instance;
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	TimeWizard.Signal();

	float timer = (float)TimeWizard.TotalTime();

	if (g_ScreenChanged)
	{
		WorldShader.projectView = g_newProjection;
		g_ScreenChanged = false;
	}

	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_StencilView);
	g_pd3dDeviceContext->RSSetViewports(1, &g_DirectView);
	WorldShader.ScreenHeight = g_DirectView.Height;
	float temp[4] = { 0.0f, 0.1f, 0.3f, 1.0f };
	g_pd3dDeviceContext->ClearRenderTargetView(g_pRenderTargetView, temp);
	g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_pd3dDeviceContext->RSSetState(DefaultRasterState);

	float BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_pd3dDeviceContext->OMSetBlendState(BlendState, NULL, 0xFFFFFFFF);

	g_pd3dDeviceContext->PSSetShaderResources(0, 1, &SkyBoxShaderView);
	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &FloorShaderView);
	VRAMPixelShader.whichTexture = 0;

#pragma region control Light
#if USINGOLDLIGHTCODE
	if (GetAsyncKeyState('Z'))
	{
		Lights.m_DirLight.Direction.x += TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('X'))
	{
		Lights.m_DirLight.Direction.x -= TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('C'))
	{
		Lights.m_DirLight.Direction.y += TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('V'))
	{
		Lights.m_DirLight.Direction.y -= TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('B'))
	{
		Lights.m_DirLight.Direction.z += TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('N'))
	{
		Lights.m_DirLight.Direction.z -= TimeWizard.SmoothDelta();
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		Lights.m_DirLight.Direction.x = 0;
		Lights.m_DirLight.Direction.y = 0;
		Lights.m_DirLight.Direction.z = 0;
	}
#endif
#if !USINGOLDLIGHTCODE

	if (GetAsyncKeyState('X'))
	{
		Lights[1].Position.x += (float)TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('Z'))
	{
		Lights[1].Position.x -= (float)TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('C'))
	{
		Lights[1].Position.y += (float)TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('V'))
	{
		Lights[1].Position.y -= (float)TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('B'))
	{
		Lights[1].Position.z += (float)TimeWizard.SmoothDelta();
	}
	if (GetAsyncKeyState('N'))
	{
		Lights[1].Position.z -= (float)TimeWizard.SmoothDelta();
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		Lights[1].Position.x = -1.0f;
		Lights[1].Position.y = 1.0f;
		Lights[1].Position.z = 0;
	}

	if (GetAsyncKeyState(VK_NUMPAD8))
	{
		Lights[0].Direction.z -= (float)TimeWizard.SmoothDelta();
		if (Lights[0].Direction.z <= -1.0f)
			Lights[0].Direction.z = -1.0f;
	}
	if (GetAsyncKeyState(VK_NUMPAD2))
	{
		Lights[0].Direction.z += (float)TimeWizard.SmoothDelta();
		if (Lights[0].Direction.z >= 1.0f)
			Lights[0].Direction.z = 1.0f;
	}
	if (GetAsyncKeyState(VK_NUMPAD6))
	{
		Lights[0].Direction.x -= (float)TimeWizard.SmoothDelta();
		if (Lights[0].Direction.x <= -1.0f)
			Lights[0].Direction.x = -1.0f;
	}
	if (GetAsyncKeyState(VK_NUMPAD4))
	{
		Lights[0].Direction.x += (float)TimeWizard.SmoothDelta();
		if (Lights[0].Direction.x >= 1.0f)
			Lights[0].Direction.x = 1.0f;
	}


	if (lightsToggle)
	{
		if (Lights[0].Radius.x == 0)
			Lights[0].Radius.x = 1;
		else
			Lights[0].Radius.x = 0;
		if (Lights[1].Radius.x == 0)
			Lights[1].Radius.x = 1;
		else
			Lights[1].Radius.x = 0;
		if (Lights[2].Radius.x == 0)
			Lights[2].Radius.x = 1;
		else
			Lights[2].Radius.x = 0;
		lightsToggle = false;
	}

#endif
#pragma endregion 

#pragma region Toggle Normal Map
	if (!textControls)
	{
		if (GetAsyncKeyState('M') & 0x1)
		{
			ToggleBumpMap = !ToggleBumpMap;
		}
	}
#pragma endregion

#pragma region ControlCamera

	XMMATRIX T;
	if (!textControls)
	{
		if (GetAsyncKeyState('W'))
		{

			T = XMMatrixTranslation(0, 0, (float)TimeWizard.Delta());
			m_viewMatrix = XMMatrixMultiply(T, m_viewMatrix);
		}
		if (GetAsyncKeyState('S'))
		{
			T = XMMatrixTranslation(0, 0, (float)-TimeWizard.Delta());
			m_viewMatrix = XMMatrixMultiply(T, m_viewMatrix);
		}

		XMMATRIX L;
		if (GetAsyncKeyState('D'))
		{

			L = XMMatrixTranslation((float)TimeWizard.Delta(), 0, 0);
			m_viewMatrix = XMMatrixMultiply(L, m_viewMatrix);
		}
		if (GetAsyncKeyState('A'))
		{

			L = XMMatrixTranslation((float)-TimeWizard.Delta(), 0, 0);
			m_viewMatrix = XMMatrixMultiply(L, m_viewMatrix);
		}
	}
	XMMATRIX C;
	if (GetAsyncKeyState(VK_UP))
	{

		C = XMMatrixTranslation(0, (float)TimeWizard.Delta(), 0);
		m_viewMatrix = XMMatrixMultiply(m_viewMatrix, C);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		C = XMMatrixTranslation(0, (float)-TimeWizard.Delta(), 0);
		m_viewMatrix = XMMatrixMultiply(m_viewMatrix, C);
	}

	if (GetAsyncKeyState(VK_NUMPAD7) & 0x1)
	{
		bSplitScreen = !bSplitScreen;
	}

	XMVECTOR TempXYZW = m_viewMatrix.r[3];


	if (GetAsyncKeyState(VK_LBUTTON))
	{
		m_viewMatrix.r[3].m128_f32[0] = 0.0f;
		m_viewMatrix.r[3].m128_f32[1] = 0.0f;
		m_viewMatrix.r[3].m128_f32[2] = 0.0f;
		m_viewMatrix.r[3].m128_f32[3] = 1.0f;

		GetCursorPos(&newPoint);

		if (!Checked)
		{
			prevPoint = newPoint;
		}

		if (prevPoint.x != newPoint.x)
		{
			m_viewMatrix = XMMatrixMultiply(m_viewMatrix, XMMatrixRotationY((prevPoint.x - newPoint.x) * -.01f)); //Global
		}
		if (prevPoint.y != newPoint.y)
		{
			m_viewMatrix = XMMatrixMultiply(XMMatrixRotationX((prevPoint.y - newPoint.y) * -.01f), m_viewMatrix); //LOCAL
		}

		prevPoint = newPoint;
		Checked = true;
	}
	else
	{
		Checked = false;
	}
	m_viewMatrix.r[3] = TempXYZW;
	Lights[2].Position.x = TempXYZW.m128_f32[0];
	Lights[2].Position.y = TempXYZW.m128_f32[1];
	Lights[2].Position.z = TempXYZW.m128_f32[2];
	Lights[2].Position.w = TempXYZW.m128_f32[3];

	WorldShader.viewMatrix = XMMatrixInverse(nullptr, m_viewMatrix);

	Lights[2].Direction.x = WorldShader.viewMatrix.r[0].m128_f32[2];
	Lights[2].Direction.y = WorldShader.viewMatrix.r[1].m128_f32[2];
	Lights[2].Direction.z = WorldShader.viewMatrix.r[2].m128_f32[2];

#pragma endregion

	if (bSplitScreen == false && !GetAsyncKeyState(VK_NUMPAD7))
	{
#pragma region Updating Video Buffers
		//Sending NEW worldMARIX, viewMatrix, projectionMATRIX to videocard
		D3D11_MAPPED_SUBRESOURCE m_mapSource;
		g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
		memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
		g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

		//Sending NEW rotation, scale, translation to videocard
		D3D11_MAPPED_SUBRESOURCE m_mapSource2;
		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

		//Sending NEW Texture details to videocard
		D3D11_MAPPED_SUBRESOURCE m_mapSource1;
		g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
		memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
		g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

		//Sending NEW Light Info to videoCard
		D3D11_MAPPED_SUBRESOURCE LightSauce;
		g_pd3dDeviceContext->Map(CostantBufferLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &LightSauce);
		memcpy_s(LightSauce.pData, sizeof(Lights), &Lights, sizeof(Lights));
		g_pd3dDeviceContext->Unmap(CostantBufferLights, 0);

		//Sending instance Data to the videoCard
		D3D11_MAPPED_SUBRESOURCE InstanceSource;
		g_pd3dDeviceContext->Map(InstanceCostantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &InstanceSource);
		memcpy_s(InstanceSource.pData, sizeof(Instance) * 4, &list, sizeof(Instance) * 4);
		g_pd3dDeviceContext->Unmap(InstanceCostantBuffer, 0);

		//Sending new Scaling Data for Tessel Triangle
		D3D11_MAPPED_SUBRESOURCE SizingTesselSource;
		g_pd3dDeviceContext->Map(CostantBufferTessScale, 0, D3D11_MAP_WRITE_DISCARD, 0, &SizingTesselSource);
		memcpy_s(SizingTesselSource.pData, sizeof(Scaling), &TesselScale, sizeof(Scaling));
		g_pd3dDeviceContext->Unmap(CostantBufferTessScale, 0);


		g_pd3dDeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer[0]);
		g_pd3dDeviceContext->VSSetConstantBuffers(1, 1, &constantBuffer[1]);
		g_pd3dDeviceContext->VSSetConstantBuffers(2, 1, &InstanceCostantBuffer);
		g_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &constantPixelBuffer);
		g_pd3dDeviceContext->PSSetConstantBuffers(1, 1, &CostantBufferLights);
		g_pd3dDeviceContext->DSSetConstantBuffers(0, 1, &constantBuffer[0]);
		g_pd3dDeviceContext->DSSetConstantBuffers(1, 1, &constantBuffer[1]);
		g_pd3dDeviceContext->HSSetConstantBuffers(0, 1, &CostantBufferTessScale);
#pragma endregion

		unsigned int stride = 0;
		unsigned int offsets = 0;

#pragma region Drawing Skybox

		translating.Translate.r[3] = m_viewMatrix.r[3];
		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);


		stride = sizeof(VERTEX);
		g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferSkyBox, &stride, &offsets);

		VRAMPixelShader.whichTexture = 0;

		g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
		memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
		g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

		g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
		g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
		g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);

		g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferSkyBox, DXGI_FORMAT_R32_UINT, 0);
		g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_pd3dDeviceContext->PSSetSamplers(0, 1, &sampleTexture);

		g_pd3dDeviceContext->RSSetState(SkyBoxRasterState);

		g_pd3dDeviceContext->DrawIndexed(SkyBoxIndexCount, 0, 0);

#pragma endregion

		g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

#pragma region Drawing light Source

		if (Lights[1].Radius.x == 1)
		{
			stride = sizeof(SIMPLE_VERTEX);

			translating.Translate = XMMatrixTranslation(Lights[1].Position.x, Lights[1].Position.y, Lights[1].Position.z);
			translating.Scale = .1f;
			g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
			memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
			g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);


			g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[1]);
			g_pd3dDeviceContext->VSSetShader(DirectVertShader[1], NULL, NULL);
			g_pd3dDeviceContext->PSSetShader(DirectPixShader[1], NULL, NULL);

			g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferSource, &stride, &offsets);
			g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferSource, DXGI_FORMAT_R32_UINT, 0);

			g_pd3dDeviceContext->DrawIndexed(SourceIndexCount, 0, 0);

			translating.Translate = XMMatrixTranslation(0, 0, 0);

			g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
			memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
			g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);
		}

#pragma endregion

#pragma region Drawing Deadpool

		translating.Translate = XMMatrixTranslation(0, 0, 0);
		translating.Scale = .3f;
		WorldShader.worldMatrix = XMMatrixRotationY(timer * 0.5f);
		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

		g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
		memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
		g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

		stride = sizeof(VERTEX);

		if (ToggleBumpMap)
			VRAMPixelShader.whichTexture = 1;
		else if (!ToggleBumpMap)
			VRAMPixelShader.whichTexture = 2;

		g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
		memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
		g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

		g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
		g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
		g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);
		g_pd3dDeviceContext->PSSetShaderResources(1, 1, &DeadpoolShaderView);
		g_pd3dDeviceContext->PSSetShaderResources(2, 1, &DeadpoolNORMShaderView);
		g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferDeadpool, &stride, &offsets);
		g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferDeadpool, DXGI_FORMAT_R32_UINT, 0);
		if (IndexBufferDeadpool)
			g_pd3dDeviceContext->DrawIndexed(DeadpoolIndexCount, 0, 0);

		translating.Translate = XMMatrixTranslation(0, 0, 0);
		translating.Rotation = XMMatrixIdentity();
		translating.Scale = 1.0f;

		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

		WorldShader.worldMatrix = XMMatrixIdentity();
		g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
		memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
		g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

#pragma endregion

#pragma region Drawing FBX Mage Walk
		translating.Translate = XMMatrixTranslation(2, 0, 0);
		translating.Scale = .5f;
		//translating.Rotation = XMMatrixRotationY(timer * 2.0f);
		WorldShader.worldMatrix = XMMatrixRotationY(-timer * 0.5f);
		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

		g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
		memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
		g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);



		stride = sizeof(SkinnedVertex);

		if (ToggleBumpMap)
			VRAMPixelShader.whichTexture = 1;
		else if (!ToggleBumpMap)
			VRAMPixelShader.whichTexture = 2;

		g_pd3dDeviceContext->IASetInputLayout(SkinningVertexLayout);
		g_pd3dDeviceContext->VSSetShader(SkinningShader, NULL, NULL);
		g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);
		g_pd3dDeviceContext->VSSetConstantBuffers(2, 1, &MageSkeleonBuffer);
		g_pd3dDeviceContext->VSSetConstantBuffers(3, 1, &InstanceCostantBuffer);
		g_pd3dDeviceContext->PSSetShaderResources(1, 1, &MagePoseTexture);
		g_pd3dDeviceContext->PSSetShaderResources(2, 1, &MagePoseNormTexture);
		g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &MagePoseVertex, &stride, &offsets);
		g_pd3dDeviceContext->IASetIndexBuffer(MagePoseIndex, DXGI_FORMAT_R32_UINT, 0);
		if (MagePoseIndex && animationdone)
		{
			if (animationdone)
				MageAnimation.Update((float)TimeWizard.Delta() * 10, &MageSkeleton.JointPostion);
			D3D11_MAPPED_SUBRESOURCE MageSkeleton_Map;
			g_pd3dDeviceContext->Map(MageSkeleonBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MageSkeleton_Map);
			memcpy_s(MageSkeleton_Map.pData, sizeof(cBufferSkeleton), MageSkeleton.JointPostion, sizeof(cBufferSkeleton));
			g_pd3dDeviceContext->Unmap(MageSkeleonBuffer, 0);
			//g_pd3dDeviceContext->DrawIndexed(MagePoseIndexCount, 0, 0);
			g_pd3dDeviceContext->DrawInstanced(MagePoseIndexCount, 4, 0, 0);
		}
		translating.Translate = XMMatrixTranslation(0, 0, 0);
		translating.Rotation = XMMatrixIdentity();
		translating.Scale = 1.0f;
		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

		WorldShader.worldMatrix = XMMatrixIdentity();
		g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
		memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
		g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);
#pragma endregion

#pragma region Drawing Floor

		g_pd3dDeviceContext->PSSetShaderResources(1, 1, &FloorShaderView);
		g_pd3dDeviceContext->PSSetShaderResources(2, 1, &FloorNORMShaderView);

		stride = sizeof(VERTEX);

		if (ToggleBumpMap)
			VRAMPixelShader.whichTexture = 1;
		else if (!ToggleBumpMap)
			VRAMPixelShader.whichTexture = 2;

		g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
		memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
		g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

		g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
		g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
		g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);

		g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferPlane, &stride, &offsets);
		g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferPlane, DXGI_FORMAT_R32_UINT, 0);

		g_pd3dDeviceContext->RSSetState(SkyBoxRasterState);
		g_pd3dDeviceContext->DrawIndexed(PlaneIndexCount, 0, 0);
#pragma endregion

#pragma region Drawing Text

		POINT CUR;
		GetCursorPos(&CUR);
		ScreenToClient(window, &CUR);
		//RECT* dest;  //Rectangle(1, 2, 336, 127);
		ID3D11Texture2D* x;
		m_text->GetResource((ID3D11Resource**)&x);
		D3D11_TEXTURE2D_DESC desc;
		x->GetDesc(&desc);

		SimpleMath::Rectangle dest = SimpleMath::Rectangle(100, 100, desc.Width / 2, desc.Height / 2);

		spritebatch->Begin();
		spritebatch->SetViewport(g_DirectView);
		if (GetAsyncKeyState(VK_RBUTTON) & 0x1)
		{
		if (dest.Contains(CUR.x, CUR.y))
		{


			sound->Play();
			textureSwitch = !textureSwitch;
			lightsToggle = true;
		}
		}
		if (!textureSwitch)
		{

			spritebatch->Draw(m_text2, XMFLOAT2(100, 100), NULL, DirectX::Colors::Red, 0.0f, XMFLOAT2(0, 0), 0.5f, SpriteEffects::SpriteEffects_None, 0.0f);
			m_textFont->DrawString(spritebatch.get(), L"OFF", DirectX::XMFLOAT2(155, 118), DirectX::Colors::Black);

		}
		else if (textureSwitch)
		{
			spritebatch->Draw(m_text, XMFLOAT2(100, 100), NULL, DirectX::Colors::Green, 0.0f, XMFLOAT2(0, 0), 0.5f, SpriteEffects::SpriteEffects_None, 0.0f);
			m_textFont->DrawString(spritebatch.get(), L"ON", DirectX::XMFLOAT2(155, 118), DirectX::Colors::Black);

		}

		m_textFont->DrawString(spritebatch.get(), L"I did it, Hello WORLD!!", DirectX::XMFLOAT2(1, 1), DirectX::Colors::DeepPink);
		//g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		//spritebatch.reset(new SpriteBatch(g_pd3dDeviceContext));
#pragma region TextBox
		spritebatch->Draw(m_textbox, XMFLOAT2(5, 400), NULL, DirectX::Colors::White);
		if (GetAsyncKeyState(VK_LSHIFT) & 0x1)
		{
			textControls = !textControls;
		}

		if (textControls)
		{
			if (GetAsyncKeyState(VK_DELETE) & 0x1)
			{
				if (GetAsyncKeyState(VK_LSHIFT) & 0X1)
				{
					textControls = false;
					ClearInput();
				}
				else
					ClearInput();
			}

			if (currentLine == 0 && (wcscmp(lines[currentLine].text, L"") == 0))
				lines[currentLine].text = L"Press Left Shift";
			if (GetAsyncKeyState(VK_BACK) & 0x1)
			{
				if (!words[currentLine].empty())
					words[currentLine].pop_back();
				else if (words[currentLine].empty())
					words[currentLine] = L"Press Left Shift";
				if (currentLine != 0 && (wcscmp(words[currentLine].c_str(), L"") == 0 || currentLine >= 13))
				{
					currentLine--;
					words[currentLine].pop_back();
					lines[currentLine].check = false;
				}
			}
			if (words[currentLine].size() > 18 && currentLine<13)
			{


				GetInputTextBox(words[currentLine]);
				//lines[currentLine].text = words[currentLine].c_str();
				lines[currentLine].check = true;
				currentLine++;
			}
			else
			{
				if (currentLine < 13)
				{
					if (words[currentLine].size() <= 18)
					{
						GetInputTextBox(words[currentLine]);
						lines[currentLine].text = words[currentLine].c_str();
					}
				}
			}

		}
		for (size_t i = 0; i < lines.size(); i++)
		{
			if (currentLine > 13)
				currentLine = 13;
			if (lines[i].check)
			{
				m_textFont->DrawString(spritebatch.get(), lines[i].text, lines[i].pos, DirectX::Colors::Black);

			}
			else
			{
				if (wcscmp(lines[i].text, L"Press Left Shift") == 0)
					break;
				m_textFont->DrawString(spritebatch.get(), lines[currentLine].text, lines[i].pos, DirectX::Colors::Black);
				break;
			}
		}

#pragma endregion

		spritebatch->End();
		g_pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
#pragma endregion
	}

	else
	{
		secondViewPort();
		thirdViewPort();
	}

	g_pSwapChain->Present(0, 0);

	return true;
}

void DEMO_APP::secondViewPort()
{
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
	g_pSwapChain->GetDesc(&m_swapChainDesc);

	TimeWizard.Signal();

	float timer = (float)TimeWizard.TotalTime();

	if (g_ScreenChanged)
	{
		WorldShader.projectView = g_newProjection;
		g_ScreenChanged = false;
	}

	g_secondDirectView.Width = m_swapChainDesc.BufferDesc.Width * 0.5f;
	g_secondDirectView.Height = m_swapChainDesc.BufferDesc.Height * 0.5f;
	g_secondDirectView.MinDepth = 0.0f;
	g_secondDirectView.MaxDepth = 1.0f;
	g_secondDirectView.TopLeftX = 0;
	g_secondDirectView.TopLeftY = 0;

	//orientation of model
	WorldShader.projectView = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), g_DirectView.Width / g_DirectView.Height, ZNEAR, ZFAR);
	// desc of camera
	WorldShader.viewMatrix = XMMatrixInverse(NULL, m_viewMatrix);

	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_StencilView);
	g_pd3dDeviceContext->RSSetViewports(1, &g_secondDirectView);
	WorldShader.ScreenHeight = g_DirectView.Height;
	float temp[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_pd3dDeviceContext->ClearRenderTargetView(g_pRenderTargetView, temp);
	g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_pd3dDeviceContext->RSSetState(DefaultRasterState);

	float BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_pd3dDeviceContext->OMSetBlendState(BlendState, NULL, 0xFFFFFFFF);

	g_pd3dDeviceContext->PSSetShaderResources(0, 1, &SkyBoxShaderView);
	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &FloorShaderView);
	VRAMPixelShader.whichTexture = 0;

#pragma region Updating Video Buffers
	//Sending NEW worldMARIX, viewMatrix, projectionMATRIX to videocard
	D3D11_MAPPED_SUBRESOURCE m_mapSource;
	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

	//Sending NEW rotation, scale, translation to videocard
	D3D11_MAPPED_SUBRESOURCE m_mapSource2;
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	//Sending NEW Texture details to videocard
	D3D11_MAPPED_SUBRESOURCE m_mapSource1;
	g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
	memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
	g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

	//Sending NEW Light Info to videoCard
	D3D11_MAPPED_SUBRESOURCE LightSauce;
	g_pd3dDeviceContext->Map(CostantBufferLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &LightSauce);
	memcpy_s(LightSauce.pData, sizeof(Lights), &Lights, sizeof(Lights));
	g_pd3dDeviceContext->Unmap(CostantBufferLights, 0);

	//Sending instance Data to the videoCard
	D3D11_MAPPED_SUBRESOURCE InstanceSource;
	g_pd3dDeviceContext->Map(InstanceCostantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &InstanceSource);
	memcpy_s(InstanceSource.pData, sizeof(Instance) * 4, &list, sizeof(Instance) * 4);
	g_pd3dDeviceContext->Unmap(InstanceCostantBuffer, 0);

	//Sending new Scaling Data for Tessel Triangle
	D3D11_MAPPED_SUBRESOURCE SizingTesselSource;
	g_pd3dDeviceContext->Map(CostantBufferTessScale, 0, D3D11_MAP_WRITE_DISCARD, 0, &SizingTesselSource);
	memcpy_s(SizingTesselSource.pData, sizeof(Scaling), &TesselScale, sizeof(Scaling));
	g_pd3dDeviceContext->Unmap(CostantBufferTessScale, 0);


	g_pd3dDeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer[0]);
	g_pd3dDeviceContext->VSSetConstantBuffers(1, 1, &constantBuffer[1]);
	g_pd3dDeviceContext->VSSetConstantBuffers(2, 1, &InstanceCostantBuffer);
	g_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &constantPixelBuffer);
	g_pd3dDeviceContext->PSSetConstantBuffers(1, 1, &CostantBufferLights);
	g_pd3dDeviceContext->DSSetConstantBuffers(0, 1, &constantBuffer[0]);
	g_pd3dDeviceContext->DSSetConstantBuffers(1, 1, &constantBuffer[1]);
	g_pd3dDeviceContext->HSSetConstantBuffers(0, 1, &CostantBufferTessScale);
#pragma endregion

	unsigned int stride = 0;
	unsigned int offsets = 0;

#pragma region Drawing Skybox

	translating.Translate.r[3] = m_viewMatrix.r[3];
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);


	stride = sizeof(VERTEX);
	g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferSkyBox, &stride, &offsets);

	VRAMPixelShader.whichTexture = 0;

	g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
	memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
	g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

	g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
	g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
	g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);

	g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferSkyBox, DXGI_FORMAT_R32_UINT, 0);
	g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pd3dDeviceContext->PSSetSamplers(0, 1, &sampleTexture);

	g_pd3dDeviceContext->RSSetState(SkyBoxRasterState);

	g_pd3dDeviceContext->DrawIndexed(SkyBoxIndexCount, 0, 0);

#pragma endregion

	g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

#pragma region Drawing light Source

	if (Lights[1].Radius.x == 1)
	{
		stride = sizeof(SIMPLE_VERTEX);

		translating.Translate = XMMatrixTranslation(Lights[1].Position.x, Lights[1].Position.y, Lights[1].Position.z);
		translating.Scale = .1f;
		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);


		g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[1]);
		g_pd3dDeviceContext->VSSetShader(DirectVertShader[1], NULL, NULL);
		g_pd3dDeviceContext->PSSetShader(DirectPixShader[1], NULL, NULL);

		g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferSource, &stride, &offsets);
		g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferSource, DXGI_FORMAT_R32_UINT, 0);

		g_pd3dDeviceContext->DrawIndexed(SourceIndexCount, 0, 0);

		translating.Translate = XMMatrixTranslation(0, 0, 0);

		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);
	}

#pragma endregion

#pragma region Drawing Deadpool

	translating.Translate = XMMatrixTranslation(0, 0, 0);
	translating.Scale = .3f;
	WorldShader.worldMatrix = XMMatrixRotationY(timer * 0.5f);
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

	stride = sizeof(VERTEX);

	if (ToggleBumpMap)
		VRAMPixelShader.whichTexture = 1;
	else if (!ToggleBumpMap)
		VRAMPixelShader.whichTexture = 2;

	g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
	memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
	g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

	g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
	g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
	g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);
	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &DeadpoolShaderView);
	g_pd3dDeviceContext->PSSetShaderResources(2, 1, &DeadpoolNORMShaderView);
	g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferDeadpool, &stride, &offsets);
	g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferDeadpool, DXGI_FORMAT_R32_UINT, 0);
	if (IndexBufferDeadpool)
		g_pd3dDeviceContext->DrawIndexed(DeadpoolIndexCount, 0, 0);

	translating.Translate = XMMatrixTranslation(0, 0, 0);
	translating.Rotation = XMMatrixIdentity();
	translating.Scale = 1.0f;

	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	WorldShader.worldMatrix = XMMatrixIdentity();
	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

#pragma endregion

#pragma region Drawing FBX Mage Walk
	translating.Translate = XMMatrixTranslation(2, 0, 0);
	translating.Scale = .5f;
	//translating.Rotation = XMMatrixRotationY(timer * 2.0f);
	WorldShader.worldMatrix = XMMatrixRotationY(-timer * 0.5f);
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);



	stride = sizeof(SkinnedVertex);

	if (ToggleBumpMap)
		VRAMPixelShader.whichTexture = 1;
	else if (!ToggleBumpMap)
		VRAMPixelShader.whichTexture = 2;

	g_pd3dDeviceContext->IASetInputLayout(SkinningVertexLayout);
	g_pd3dDeviceContext->VSSetShader(SkinningShader, NULL, NULL);
	g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);
	g_pd3dDeviceContext->VSSetConstantBuffers(2, 1, &MageSkeleonBuffer);
	g_pd3dDeviceContext->VSSetConstantBuffers(3, 1, &InstanceCostantBuffer);
	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &MagePoseTexture);
	g_pd3dDeviceContext->PSSetShaderResources(2, 1, &MagePoseNormTexture);
	g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &MagePoseVertex, &stride, &offsets);
	g_pd3dDeviceContext->IASetIndexBuffer(MagePoseIndex, DXGI_FORMAT_R32_UINT, 0);
	if (MagePoseIndex && animationdone)
	{
		if (animationdone)
			MageAnimation.Update((float)TimeWizard.Delta() * 20, &MageSkeleton.JointPostion);
		D3D11_MAPPED_SUBRESOURCE MageSkeleton_Map;
		g_pd3dDeviceContext->Map(MageSkeleonBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MageSkeleton_Map);
		memcpy_s(MageSkeleton_Map.pData, sizeof(cBufferSkeleton), MageSkeleton.JointPostion, sizeof(cBufferSkeleton));
		g_pd3dDeviceContext->Unmap(MageSkeleonBuffer, 0);
		//g_pd3dDeviceContext->DrawIndexed(MagePoseIndexCount, 0, 0);
		g_pd3dDeviceContext->DrawInstanced(MagePoseIndexCount, 4, 0, 0);
	}
	translating.Translate = XMMatrixTranslation(0, 0, 0);
	translating.Rotation = XMMatrixIdentity();
	translating.Scale = 1.0f;
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	WorldShader.worldMatrix = XMMatrixIdentity();
	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);
#pragma endregion

#pragma region Drawing Floor

	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &FloorShaderView);
	g_pd3dDeviceContext->PSSetShaderResources(2, 1, &FloorNORMShaderView);

	stride = sizeof(VERTEX);

	if (ToggleBumpMap)
		VRAMPixelShader.whichTexture = 1;
	else if (!ToggleBumpMap)
		VRAMPixelShader.whichTexture = 2;

	g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
	memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
	g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

	g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
	g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
	g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);

	g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferPlane, &stride, &offsets);
	g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferPlane, DXGI_FORMAT_R32_UINT, 0);

	g_pd3dDeviceContext->RSSetState(SkyBoxRasterState);
	g_pd3dDeviceContext->DrawIndexed(PlaneIndexCount, 0, 0);
#pragma endregion

#pragma region Drawing Text

	POINT CUR;
	GetCursorPos(&CUR);
	ScreenToClient(window, &CUR);
	//RECT* dest;  //Rectangle(1, 2, 336, 127);
	ID3D11Texture2D* x;
	m_text->GetResource((ID3D11Resource**)&x);
	D3D11_TEXTURE2D_DESC desc;
	x->GetDesc(&desc);

	SimpleMath::Rectangle dest = SimpleMath::Rectangle(100, 100, desc.Width / 2, desc.Height / 2);

	spritebatch->Begin();
	spritebatch->SetViewport(g_secondDirectView);
	if ((GetAsyncKeyState(VK_RBUTTON) & 0x1))
	{
		if (dest.Contains(CUR.x, CUR.y))
		{
			sound->Play();
			textureSwitch = !textureSwitch;
			lightsToggle = true;
		}
	}
	if (!textureSwitch)
	{

		spritebatch->Draw(m_text2, XMFLOAT2(100, 100), NULL, DirectX::Colors::Red, 0.0f, XMFLOAT2(0, 0), 0.5f, SpriteEffects::SpriteEffects_None, 0.0f);
		m_textFont->DrawString(spritebatch.get(), L"OFF", DirectX::XMFLOAT2(155, 118), DirectX::Colors::Black);

	}
	else if (textureSwitch)
	{
		spritebatch->Draw(m_text, XMFLOAT2(100, 100), NULL, DirectX::Colors::Green, 0.0f, XMFLOAT2(0, 0), 0.5f, SpriteEffects::SpriteEffects_None, 0.0f);
		m_textFont->DrawString(spritebatch.get(), L"ON", DirectX::XMFLOAT2(155, 118), DirectX::Colors::Black);

	}

	m_textFont->DrawString(spritebatch.get(), L"I did it, Hello WORLD!!", DirectX::XMFLOAT2(1, 1), DirectX::Colors::DeepPink);
	//g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//spritebatch.reset(new SpriteBatch(g_pd3dDeviceContext));
#pragma region TextBox
	spritebatch->Draw(m_textbox, XMFLOAT2(5, 400), NULL, DirectX::Colors::White);
	if (GetAsyncKeyState(VK_LSHIFT) & 0x1)
	{
		textControls = !textControls;
	}

	if (textControls)
	{
		if (GetAsyncKeyState(VK_DELETE) & 0x1)
		{
			if (GetAsyncKeyState(VK_LSHIFT) & 0X1)
			{
				textControls = false;
				ClearInput();
			}
			else
				ClearInput();
		}

		if (currentLine == 0 && (wcscmp(lines[currentLine].text, L"") == 0))
			lines[currentLine].text = L"Press Left Shift";
		if (GetAsyncKeyState(VK_BACK) & 0x1)
		{
			if (!words[currentLine].empty())
				words[currentLine].pop_back();
			else if (words[currentLine].empty())
				words[currentLine] = L"Press Left Shift";
			if (currentLine != 0 && (wcscmp(words[currentLine].c_str(), L"") == 0 || currentLine >= 13))
			{
				currentLine--;
				words[currentLine].pop_back();
				lines[currentLine].check = false;
			}
		}
		if (words[currentLine].size() > 18 && currentLine<13)
		{


			GetInputTextBox(words[currentLine]);
			//lines[currentLine].text = words[currentLine].c_str();
			lines[currentLine].check = true;
			currentLine++;
		}
		else
		{
			if (currentLine < 13)
			{
				if (words[currentLine].size() <= 18)
				{
					GetInputTextBox(words[currentLine]);
					lines[currentLine].text = words[currentLine].c_str();
				}
			}
		}

	}
	for (size_t i = 0; i < lines.size(); i++)
	{
		if (currentLine > 13)
			currentLine = 13;
		if (lines[i].check)
		{
			m_textFont->DrawString(spritebatch.get(), lines[i].text, lines[i].pos, DirectX::Colors::Black);

		}
		else
		{
			if (wcscmp(lines[i].text, L"Press Left Shift") == 0)
				break;
			m_textFont->DrawString(spritebatch.get(), lines[currentLine].text, lines[i].pos, DirectX::Colors::Black);
			break;
		}
	}

#pragma endregion
	spritebatch->End();
	g_pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
#pragma endregion
}

void DEMO_APP::thirdViewPort()
{
	DXGI_SWAP_CHAIN_DESC _swapChainDesc;
	g_pSwapChain->GetDesc(&_swapChainDesc);

	TimeWizard.Signal();

	float timer = (float)TimeWizard.TotalTime();

	if (g_ScreenChanged)
	{
		WorldShader.projectView = g_newProjection;
		g_ScreenChanged = false;
	}

	g_thirdDirectView.Width = _swapChainDesc.BufferDesc.Width * 0.5f;
	g_thirdDirectView.Height = _swapChainDesc.BufferDesc.Height * 0.5f;
	g_thirdDirectView.MinDepth = 0.0f;
	g_thirdDirectView.MaxDepth = 1.0f;
	g_thirdDirectView.TopLeftX = g_thirdDirectView.Width;
	g_thirdDirectView.TopLeftY = g_thirdDirectView.Height;

	//orientation of model
	WorldShader.projectView = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), g_thirdDirectView.Width / g_thirdDirectView.Height, ZNEAR, ZFAR);
	//desc of camera
	WorldShader.viewMatrix = XMMatrixInverse(NULL, m_viewMatrix);

	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_StencilView);
	g_pd3dDeviceContext->RSSetViewports(1, &g_thirdDirectView);
	g_pd3dDeviceContext->RSSetState(DefaultRasterState);

	float temp[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	float BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_pd3dDeviceContext->OMSetBlendState(BlendState, NULL, 0xFFFFFFFF);

	g_pd3dDeviceContext->PSSetShaderResources(0, 1, &SkyBoxShaderView);
	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &FloorShaderView);
	VRAMPixelShader.whichTexture = 0;

#pragma region Updating Video Buffers
	//Sending NEW worldMARIX, viewMatrix, projectionMATRIX to videocard
	D3D11_MAPPED_SUBRESOURCE m_mapSource;
	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

	//Sending NEW rotation, scale, translation to videocard
	D3D11_MAPPED_SUBRESOURCE m_mapSource2;
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	//Sending NEW Texture details to videocard
	D3D11_MAPPED_SUBRESOURCE m_mapSource1;
	g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
	memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
	g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

	//Sending NEW Light Info to videoCard
	D3D11_MAPPED_SUBRESOURCE LightSauce;
	g_pd3dDeviceContext->Map(CostantBufferLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &LightSauce);
	memcpy_s(LightSauce.pData, sizeof(Lights), &Lights, sizeof(Lights));
	g_pd3dDeviceContext->Unmap(CostantBufferLights, 0);

	//Sending instance Data to the videoCard
	D3D11_MAPPED_SUBRESOURCE InstanceSource;
	g_pd3dDeviceContext->Map(InstanceCostantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &InstanceSource);
	memcpy_s(InstanceSource.pData, sizeof(Instance) * 4, &list, sizeof(Instance) * 4);
	g_pd3dDeviceContext->Unmap(InstanceCostantBuffer, 0);

	//Sending new Scaling Data for Tessel Triangle
	D3D11_MAPPED_SUBRESOURCE SizingTesselSource;
	g_pd3dDeviceContext->Map(CostantBufferTessScale, 0, D3D11_MAP_WRITE_DISCARD, 0, &SizingTesselSource);
	memcpy_s(SizingTesselSource.pData, sizeof(Scaling), &TesselScale, sizeof(Scaling));
	g_pd3dDeviceContext->Unmap(CostantBufferTessScale, 0);


	g_pd3dDeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer[0]);
	g_pd3dDeviceContext->VSSetConstantBuffers(1, 1, &constantBuffer[1]);
	g_pd3dDeviceContext->VSSetConstantBuffers(2, 1, &InstanceCostantBuffer);
	g_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &constantPixelBuffer);
	g_pd3dDeviceContext->PSSetConstantBuffers(1, 1, &CostantBufferLights);
	g_pd3dDeviceContext->DSSetConstantBuffers(0, 1, &constantBuffer[0]);
	g_pd3dDeviceContext->DSSetConstantBuffers(1, 1, &constantBuffer[1]);
	g_pd3dDeviceContext->HSSetConstantBuffers(0, 1, &CostantBufferTessScale);
#pragma endregion

	unsigned int stride = 0;
	unsigned int offsets = 0;

#pragma region Drawing Skybox

	translating.Translate.r[3] = m_viewMatrix.r[3];
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);


	stride = sizeof(VERTEX);
	g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferSkyBox, &stride, &offsets);

	VRAMPixelShader.whichTexture = 0;

	g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
	memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
	g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

	g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
	g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
	g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);

	g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferSkyBox, DXGI_FORMAT_R32_UINT, 0);
	g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pd3dDeviceContext->PSSetSamplers(0, 1, &sampleTexture);

	g_pd3dDeviceContext->RSSetState(SkyBoxRasterState);

	g_pd3dDeviceContext->DrawIndexed(SkyBoxIndexCount, 0, 0);

#pragma endregion

	g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

#pragma region Drawing light Source

	if (Lights[1].Radius.x == 1)
	{
		stride = sizeof(SIMPLE_VERTEX);

		translating.Translate = XMMatrixTranslation(Lights[1].Position.x, Lights[1].Position.y, Lights[1].Position.z);
		translating.Scale = .1f;
		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);


		g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[1]);
		g_pd3dDeviceContext->VSSetShader(DirectVertShader[1], NULL, NULL);
		g_pd3dDeviceContext->PSSetShader(DirectPixShader[1], NULL, NULL);

		g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferSource, &stride, &offsets);
		g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferSource, DXGI_FORMAT_R32_UINT, 0);

		g_pd3dDeviceContext->DrawIndexed(SourceIndexCount, 0, 0);

		translating.Translate = XMMatrixTranslation(0, 0, 0);

		g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
		memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
		g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);
	}

#pragma endregion

#pragma region Drawing Deadpool

	translating.Translate = XMMatrixTranslation(0, 0, 0);
	translating.Scale = .3f;
	WorldShader.worldMatrix = XMMatrixRotationY(timer * 0.5f);
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

	stride = sizeof(VERTEX);

	if (ToggleBumpMap)
		VRAMPixelShader.whichTexture = 1;
	else if (!ToggleBumpMap)
		VRAMPixelShader.whichTexture = 2;

	g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
	memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
	g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

	g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
	g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
	g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);
	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &DeadpoolShaderView);
	g_pd3dDeviceContext->PSSetShaderResources(2, 1, &DeadpoolNORMShaderView);
	g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferDeadpool, &stride, &offsets);
	g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferDeadpool, DXGI_FORMAT_R32_UINT, 0);
	if (IndexBufferDeadpool)
		g_pd3dDeviceContext->DrawIndexed(DeadpoolIndexCount, 0, 0);

	translating.Translate = XMMatrixTranslation(0, 0, 0);
	translating.Rotation = XMMatrixIdentity();
	translating.Scale = 1.0f;

	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	WorldShader.worldMatrix = XMMatrixIdentity();
	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);

#pragma endregion

#pragma region Drawing FBX Mage Walk
	translating.Translate = XMMatrixTranslation(2, 0, 0);
	translating.Scale = .5f;
	//translating.Rotation = XMMatrixRotationY(timer * 2.0f);
	WorldShader.worldMatrix = XMMatrixRotationY(-timer * 0.5f);
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);



	stride = sizeof(SkinnedVertex);

	if (ToggleBumpMap)
		VRAMPixelShader.whichTexture = 1;
	else if (!ToggleBumpMap)
		VRAMPixelShader.whichTexture = 2;

	g_pd3dDeviceContext->IASetInputLayout(SkinningVertexLayout);
	g_pd3dDeviceContext->VSSetShader(SkinningShader, NULL, NULL);
	g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);
	g_pd3dDeviceContext->VSSetConstantBuffers(2, 1, &MageSkeleonBuffer);
	g_pd3dDeviceContext->VSSetConstantBuffers(3, 1, &InstanceCostantBuffer);
	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &MagePoseTexture);
	g_pd3dDeviceContext->PSSetShaderResources(2, 1, &MagePoseNormTexture);
	g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &MagePoseVertex, &stride, &offsets);
	g_pd3dDeviceContext->IASetIndexBuffer(MagePoseIndex, DXGI_FORMAT_R32_UINT, 0);
	if (MagePoseIndex && animationdone)
	{
		if (animationdone)
			MageAnimation.Update((float)TimeWizard.Delta() * 20, &MageSkeleton.JointPostion);
		D3D11_MAPPED_SUBRESOURCE MageSkeleton_Map;
		g_pd3dDeviceContext->Map(MageSkeleonBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MageSkeleton_Map);
		memcpy_s(MageSkeleton_Map.pData, sizeof(cBufferSkeleton), MageSkeleton.JointPostion, sizeof(cBufferSkeleton));
		g_pd3dDeviceContext->Unmap(MageSkeleonBuffer, 0);
		//g_pd3dDeviceContext->DrawIndexed(MagePoseIndexCount, 0, 0);
		g_pd3dDeviceContext->DrawInstanced(MagePoseIndexCount, 4, 0, 0);
	}
	translating.Translate = XMMatrixTranslation(0, 0, 0);
	translating.Rotation = XMMatrixIdentity();
	translating.Scale = 1.0f;
	g_pd3dDeviceContext->Map(constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource2);
	memcpy_s(m_mapSource2.pData, sizeof(TRANSLATOR), &translating, sizeof(TRANSLATOR));
	g_pd3dDeviceContext->Unmap(constantBuffer[1], 0);

	WorldShader.worldMatrix = XMMatrixIdentity();
	g_pd3dDeviceContext->Map(constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource);
	memcpy_s(m_mapSource.pData, sizeof(SEND_TO_VRAM_WORLD), &WorldShader, sizeof(SEND_TO_VRAM_WORLD));
	g_pd3dDeviceContext->Unmap(constantBuffer[0], 0);
#pragma endregion

#pragma region Drawing Floor

	g_pd3dDeviceContext->PSSetShaderResources(1, 1, &FloorShaderView);
	g_pd3dDeviceContext->PSSetShaderResources(2, 1, &FloorNORMShaderView);

	stride = sizeof(VERTEX);

	if (ToggleBumpMap)
		VRAMPixelShader.whichTexture = 1;
	else if (!ToggleBumpMap)
		VRAMPixelShader.whichTexture = 2;

	g_pd3dDeviceContext->Map(constantPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapSource1);
	memcpy_s(m_mapSource1.pData, sizeof(SEND_TO_VRAM_PIXEL), &VRAMPixelShader, sizeof(SEND_TO_VRAM_PIXEL));
	g_pd3dDeviceContext->Unmap(constantPixelBuffer, 0);

	g_pd3dDeviceContext->IASetInputLayout(DirectInputLay[0]);
	g_pd3dDeviceContext->VSSetShader(DirectVertShader[0], NULL, NULL);
	g_pd3dDeviceContext->PSSetShader(DirectPixShader[0], NULL, NULL);

	g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &VertexBufferPlane, &stride, &offsets);
	g_pd3dDeviceContext->IASetIndexBuffer(IndexBufferPlane, DXGI_FORMAT_R32_UINT, 0);

	g_pd3dDeviceContext->RSSetState(SkyBoxRasterState);
	g_pd3dDeviceContext->DrawIndexed(PlaneIndexCount, 0, 0);
#pragma endregion

#pragma region Drawing Text

	//POINT CUR;
	//GetCursorPos(&CUR);
	//ScreenToClient(window, &CUR);
	////RECT* dest;  //Rectangle(1, 2, 336, 127);
	//ID3D11Texture2D* x;
	//m_text->GetResource((ID3D11Resource**)&x);
	//D3D11_TEXTURE2D_DESC desc;
	//x->GetDesc(&desc);

	//SimpleMath::Rectangle dest = SimpleMath::Rectangle(100, 100, desc.Width / 2, desc.Height / 2);

	spritebatch->Begin();
	spritebatch->SetViewport(g_thirdDirectView);
	/*if ( (GetAsyncKeyState(VK_RBUTTON) & 0x1))
	{
		if (dest.Contains(CUR.x / 4, CUR.y / 4))
		{
			sound->Play();
			textureSwitch = !textureSwitch;
			lightsToggle = true;
		}
	}
	if (!textureSwitch)
	{

		spritebatch->Draw(m_text2, XMFLOAT2(100, 100), NULL, DirectX::Colors::Red, 0.0f, XMFLOAT2(0, 0), 0.5f, SpriteEffects::SpriteEffects_None, 0.0f);
		m_textFont->DrawString(spritebatch.get(), L"OFF", DirectX::XMFLOAT2(155, 118), DirectX::Colors::Black);

	}
	else if (textureSwitch)
	{
		spritebatch->Draw(m_text, XMFLOAT2(100, 100), NULL, DirectX::Colors::Green, 0.0f, XMFLOAT2(0, 0), 0.5f, SpriteEffects::SpriteEffects_None, 0.0f);
		m_textFont->DrawString(spritebatch.get(), L"ON", DirectX::XMFLOAT2(155, 118), DirectX::Colors::Black);

	}*/

	m_textFont->DrawString(spritebatch.get(), L"I did it, Hello WORLD!!", DirectX::XMFLOAT2(1, 1), DirectX::Colors::DeepPink);
	//g_pd3dDeviceContext->ClearDepthStencilView(g_StencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//spritebatch.reset(new SpriteBatch(g_pd3dDeviceContext));

	spritebatch->End();
	g_pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
#pragma endregion

}

void DEMO_APP::InitInput()
{
	for (size_t i = 0; i < 14; i++)
	{

		Words enter;

		enter.text = L"Press Left Shift";
		wstring text;
		if (i == 0)
			text = L"Press Left Shift";
		words.push_back(text);
		if (i == 0)
			enter.check = true;
		else
			enter.check = false;
		enter.pos = XMFLOAT2(5, (float)indexLineY);
		indexLineY += 22;
		/*if(i==0)
		enter.check = true;
		else
		enter.check = false;*/
		lines.push_back(enter);
	}
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{
	Clean3d();

	for (size_t i = 0; i < MageAnimation.Anim.size(); i++)
	{
		for (size_t k = 0; k < MageAnimation.Anim[i].BoneAnimations.size(); k++)
		{
			for (size_t j = 0; j < MageAnimation.Anim[i].BoneAnimations[k].Keyframes.size(); j++)
			{
				delete MageAnimation.Anim[i].BoneAnimations[k].Keyframes[j];
			}
		}
	}
	UnregisterClass(L"DirectXApplication", application);
	return true;
}

void DEMO_APP::Clean3d()
{
	g_pSwapChain->SetFullscreenState(FALSE, NULL);

	g_pd3dDeviceContext->Release();
	g_pSwapChain->Release();
	g_pRenderTargetView->Release();
	g_pd3dDevice->Release();
	g_StencilView->Release();
	g_TexBuffer->Release();

	VertexBufferStar->Release();
	IndexBufferStar->Release();
	VertexBufferPlane->Release();
	IndexBufferPlane->Release();
	VertexBufferSkyBox->Release();
	IndexBufferSkyBox->Release();
	if (VertexBufferSword)
		VertexBufferSword->Release();
	if (IndexBufferSword)
		IndexBufferSword->Release();
	if (VertexBufferDeadpool)
		VertexBufferDeadpool->Release();
	if (IndexBufferDeadpool)
		IndexBufferDeadpool->Release();
	if (VertexBufferSource)
		VertexBufferSource->Release();
	if (IndexBufferSource)
		IndexBufferSource->Release();


	DirectVertShader[0]->Release();
	DirectPixShader[0]->Release();
	DirectInputLay[0]->Release();
	constantBuffer[0]->Release();
	DirectVertShader[1]->Release();
	DirectPixShader[1]->Release();
	DirectInputLay[1]->Release();
	constantBuffer[1]->Release();
	DirectVertShader[2]->Release();
	constantPixelBuffer->Release();
	CostantBufferLights->Release();


	DefaultRasterState->Release();
	SkyBoxRasterState->Release();

	sampleTexture->Release();
	SkyBoxShaderView->Release();
	FloorShaderView->Release();
	FloorNORMShaderView->Release();
	SwordShaderView->Release();
	SwordNORMShaderView->Release();
	DeadpoolShaderView->Release();
	DeadpoolNORMShaderView->Release();

	BlendState->Release();

	/*VertexBufferGeo->Release();
	VertexShaderGeo->Release();
	GeometryShaderGeo->Release();*/

	InstanceBuffer->Release();
	InstanceCostantBuffer->Release();
	if (DeadpoolInstanceIndexBuffer)
		DeadpoolInstanceIndexBuffer->Release();
	if (DeadpoolInstanceVertexBuffer)
		DeadpoolInstanceVertexBuffer->Release();



	vertexBufferTriangle->Release();
	vertexShaderTriangle->Release();
	pixelShaderTriangle->Release();
	hullShaderTriangle->Release();
	domainShaderTriangle->Release();
	TriangleMatrix;
	RasterStateWireFrameTriangle->Release();
	RasterStateSoildTriangle->Release();
	CostantBufferTessScale->Release();

	if (BindPoseVertex)
		BindPoseVertex->Release();
	if (BindPoseIndex)
		BindPoseIndex->Release();
	if (BindPoseTexture)
		BindPoseTexture->Release();
	if (BindPoseNormTexture)
		BindPoseNormTexture->Release();


	if (MagePoseVertex)
		MagePoseVertex->Release();
	if (MagePoseIndex)
		MagePoseIndex->Release();
	if (MagePoseTexture)
		MagePoseTexture->Release();
	if (MagePoseNormTexture)
		MagePoseNormTexture->Release();
	if (SkinningShader)
		SkinningShader->Release();
	if (SkinningVertexLayout)
		SkinningVertexLayout->Release();
	if (MageSkeleonBuffer)
		MageSkeleonBuffer->Release();
	//////AAron////////
	m_text->Release();
	m_text2->Release();
	m_textFont.release();
	spritebatch.release();
	m_textbox->Release();
	audio.release();
	sound.release();
	lines.clear();
	words.clear();
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

// ****************** BEGIN WARNING ***********************// 
// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY!

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1L);

	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance, (WNDPROC)WndProc);
	MSG msg; ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && myApp.Run())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	myApp.ShutDown();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;

	switch (message)
	{

		break;
	case (WM_DESTROY): { PostQuitMessage(0); }
					   break;
	case (WM_CREATE):
		g_lParam = lParam;
		break;
	case (WM_SIZE):
	{
		if (g_pSwapChain)
			DEMO_APP::GetInstance()->ResizingOfWindows();
	};
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
void DEMO_APP::GetInputTextBox(wstring& text)
{
	bool done = false;

	if (text.compare(L"Press Left Shift") == 0)
	{
		while (!text.empty())
			text.pop_back();
	}



	for (char i = 32; i < 127; ++i)
	{
		if (GetAsyncKeyState(i) & 0x1)
		{
			text.push_back(i);
		}

	}


	/*	if (text.empty()&&currentLine==0)
	text = L"Enter";*/


}
void DEMO_APP::ClearInput()
{
	words.clear();
	lines.clear();
	currentLine = 0;
	indexLineY = 400;
	InitInput();
}

//********************* END WARNING ************************//