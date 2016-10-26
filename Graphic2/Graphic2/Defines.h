#pragma once

#include <iostream>
#include <ctime>
#include <math.h>
#include <Windows.h>
#include <vector>
#include <DirectXMath.h>
#include <thread>
#include "Load Model.h"
#include "DDSTextureLoader.h"


using namespace DirectX;
using namespace std;
#include "LightHelper.h"

#define RASTER_WIDTH 1280.0f
#define RASTER_HEIGHT 720.0f
#define NUM_PIXEL UINT(RASTER_WIDTH * RASTER_HEIGHT)
#define M_PI 3.14159

unsigned int Raster[NUM_PIXEL];
float Zbuffer[NUM_PIXEL];

#define FIELDOFVIEW 65.0f
#define ASPECTRATIO ((RASTER_WIDTH) / (RASTER_HEIGHT))
#define ZNEAR 0.1f
#define ZFAR 100.0f

#define USINGOLDLIGHTCODE FALSE


struct SEND_TO_VRAM_WORLD
{
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectView;
	float ScreenHeight;
	XMFLOAT3 padding;
};

struct SEND_TO_VRAM_PIXEL
{
	float whichTexture;
	XMFLOAT3 padding;
};

struct TRANSLATOR
{
	XMMATRIX Rotation;
	XMMATRIX Translate;
	float Scale;
	XMFLOAT3 padding;
};

struct LightSources
{
#if USINGOLDLIGHTCODE
	DirectionalLight m_DirLight;
	PointLight m_pointLight;
	SpotLight m_SpotLight;
	Material m_Material;
	XMFLOAT3 m_EyePosw;
	float pad;
#endif

#if !USINGOLDLIGHTCODE
	XMFLOAT4 Position;
	XMFLOAT4 Direction;
	XMFLOAT4 Color;
	XMFLOAT4 Radius;
#endif

};

struct VERTEX
{
	XMFLOAT4 XYZW;
	XMFLOAT3 UV;
	XMFLOAT3 normals;
	XMFLOAT3 Tangent;
	XMFLOAT3 BiTangent;
};

struct SIMPLE_VERTEX
{
	XMFLOAT4 XYZW;
	XMFLOAT4 RGBA;
	XMFLOAT3 Norm;
};

struct Instance
{
	XMMATRIX WorldLocation;
};

struct Scaling
{
	XMFLOAT4 scale;
};