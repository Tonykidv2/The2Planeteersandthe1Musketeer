// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FBXIMPORTER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FBXIMPORTER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//#ifdef FBXIMPORTER_EXPORTS
//#define FBXIMPORTER_API __declspec(dllexport)
//#else
//#define FBXIMPORTER_API __declspec(dllimport)
//#endif
//
//// This class is exported from the FBXImporter.dll
//class FBXIMPORTER_API CFBXImporter {
//public:
//	CFBXImporter(void);
//	// TODO: add your methods here.
//};
//
//extern FBXIMPORTER_API int nFBXImporter;
//
//FBXIMPORTER_API int fnFBXImporter(void);
#include "stdafx.h"
#include<fstream>
#include<string>



extern "C" __declspec(dllexport) bool StoreFBXDLLinBin(const char * path);
extern "C" __declspec(dllexport) void WriteFBXDLLtoBinary(const char * path, DirectX::XMFLOAT4 verts,DirectX::XMFLOAT3 uv,DirectX::XMFLOAT3 norm);
extern "C" __declspec(dllexport) void ReadFBXDLLfromBinary(const char * path, std::vector<DirectX::XMFLOAT4> &pOutVertexVector,
	std::vector<DirectX::XMFLOAT3>& out_UVs, std::vector<DirectX::XMFLOAT3>& out_Normals,
	std::vector<DirectX::XMFLOAT3>& out_Tangets);

