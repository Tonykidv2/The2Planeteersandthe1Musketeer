// FBXImporter.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FBXImporter.h"
#include "fbxsdk.h"


// This is an example of an exported variable
//FBXIMPORTER_API int nFBXImporter=0;

// This is an example of an exported function.
//FBXIMPORTER_API int fnFBXImporter(void)
//{
//    return 42;
//}

// This is the constructor of a class that has been exported.
// see FBXImporter.h for the class definition
//CFBXImporter::CFBXImporter()
//{
//    return;
////}


extern "C" __declspec(dllexport) bool StoreFBXDLLinBin(const char * path)
{

	int check=0;
	FbxManager* g_pFbxManager = nullptr;

	if (g_pFbxManager == nullptr)
	{
		g_pFbxManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxManager, IOSROOT);
		g_pFbxManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxManager, "");

	bool Successs = pImporter->Initialize(path, -1, g_pFbxManager->GetIOSettings());

	if (!Successs)
		return false;

	Successs = pImporter->Import(pFbxScene);
	if (!Successs)
		return false;



	FbxNode* rootNode = pFbxScene->GetRootNode();
	
	if (rootNode)
	{
		for (int i = 0; i < rootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = rootNode->GetChild(i);
			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			fbxsdk::FbxMesh* pMesh = (fbxsdk::FbxMesh*)pFbxChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();
			FbxVector4 pTangent;
			FbxVector4 pNormals;
			FbxVector2 pUVs;

			const fbxsdk::FbxGeometryElementNormal * lNormalElement = pMesh->GetElementNormal(0);
			const fbxsdk::FbxGeometryElementUV * lUVElement = pMesh->GetElementUV(0);
			const fbxsdk::FbxGeometryElementBinormal* TangentElement = pMesh->GetElementBinormal(0);

			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{

				int iNumVertices = pMesh->GetPolygonSize(j);
				assert(iNumVertices == 3);

				for (int k = 0; k < iNumVertices; k++)
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					DirectX::XMFLOAT4 vertex;
					vertex.x = -(float)pVertices[iControlPointIndex].mData[0];
					vertex.y = (float)pVertices[iControlPointIndex].mData[1];
					vertex.z = (float)pVertices[iControlPointIndex].mData[2];
					vertex.w = 1;
					
					
					pMesh->GetPolygonVertexNormal(j, k, pNormals);
					DirectX::XMFLOAT3 Normal;

					Normal.x = -(float)pNormals.mData[0];
					Normal.y = (float)pNormals.mData[1];
					Normal.z = (float)pNormals.mData[2];
					
					

					int UvIndex = pMesh->GetTextureUVIndex(j, k);
					pUVs = lUVElement->GetDirectArray().GetAt(UvIndex);
					DirectX::XMFLOAT3 UV;
					UV.x = (float)pUVs.mData[0];
					UV.y = 1.0f - (float)pUVs.mData[1];
					
					
					WriteFBXDLLtoBinary(path, vertex, UV, Normal);
					//pTangent = TangentElement->GetDirectArray().GetAt(UvIndex);
					//DirectX::XMFLOAT3 tangent;
					//tangent.x = (float)pTangent.mData[0];
					//tangent.y = (float)pTangent.mData[1];
					//tangent.z = (float)pTangent.mData[2];
					//out_Tangets.push_back(tangent);
				}


			}
		}
			
	}
	
	rootNode->Destroy();
	pImporter->Destroy();
	pFbxScene->Destroy();
	g_pFbxManager->Destroy();
	return true;
}
extern "C" __declspec(dllexport) void WriteFBXDLLtoBinary(const char * path, DirectX::XMFLOAT4 verts, DirectX::XMFLOAT3 uv, DirectX::XMFLOAT3 norm)
{
	
	//char end[4] = "end";
	std::fstream file(path, std::ios::out | std::ios::binary | std::ios::app);
	
		//verts
		//file.write(id_vert, 5);
		file.write((char*)&verts.x, sizeof(float));
		
		file.write((char*)&verts.y, sizeof(float));

		file.write((char*)&verts.z, sizeof(float));
	
		file.write((char*)&verts.w, sizeof(float));
	
		//uvs
		//file.write(id_uv, 3);
		file.write((char*)&uv.x, sizeof(float));
		
		file.write((char*)&uv.y, sizeof(float));
	
		file.write((char*)&uv.z, sizeof(float));
		

		//normals
		//file.write(id_norm, 5);
		file.write((char*)&norm.x, sizeof(float));
	
		file.write((char*)&norm.y, sizeof(float));
	
		file.write((char*)&norm.z, sizeof(float));
		

		file.close();
	
	
}
extern "C" __declspec(dllexport) void ReadFBXDLLfromBinary(const char * path, std::vector<DirectX::XMFLOAT4> &pOutVertexVector,
	std::vector<DirectX::XMFLOAT3>& out_UVs, std::vector<DirectX::XMFLOAT3>& out_Normals,
	std::vector<DirectX::XMFLOAT3>& out_Tangets)
{
	DirectX::XMFLOAT4 vert;
	DirectX::XMFLOAT3 uv;
	DirectX::XMFLOAT3 norm;
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		while (!file.eof())
		{
			file.read((char*)&vert.x, sizeof(float));
			file.read((char*)&vert.y, sizeof(float));
			file.read((char*)&vert.z, sizeof(float));
			file.read((char*)&vert.w, sizeof(float));
			pOutVertexVector.push_back(vert);
			file.read((char*)&uv.x, sizeof(float));
			file.read((char*)&uv.y, sizeof(float));
			file.read((char*)&uv.z, sizeof(float));
			out_UVs.push_back(uv);
			file.read((char*)&norm.x, sizeof(float));
			file.read((char*)&norm.y, sizeof(float));
			file.read((char*)&norm.z, sizeof(float));
			out_Normals.push_back(norm);

		}
	}
	file.close();

}
