#pragma once
#include <fstream>
//#include <fbxsdk.h>
#include "..\FBXImporter\FBXImporter.h"

namespace LoadModel
{
	bool LoadObj(const char* path, std::vector<DirectX::XMFLOAT4>& out_vertex, 
		std::vector<DirectX::XMFLOAT3>& out_UVs, std::vector<DirectX::XMFLOAT3>& out_Normals, 
		std::vector<unsigned int>& vertex_indices, std::vector<unsigned int>& uv_indices,
		std::vector<unsigned int>& normal_indices)
	{
		
		char input, input2;
		bool checked = false;
		

		std::fstream fin;

		fin.open(path, std::ios_base::in);

		if (fin.fail())
			return false;

		if (fin.is_open())
		{

			while (!fin.eof())
			{
				fin.get(input);
				if (input == '2')
				{
					input = 'd';
				}
				if (input == 'v')
				{
					fin.get(input);
					if (input == ' ')
					{
						DirectX::XMFLOAT4 temp;
						fin >> temp.x >> temp.y >> temp.z;
						temp.x = -temp.x;
						out_vertex.push_back(temp);
					}
					else if (input == 't')
					{
						DirectX::XMFLOAT3 temp;
						fin >> temp.x >> temp.y;
						temp.y = 1.0f - temp.y;
						temp.z = 0;
						out_UVs.push_back(temp);
					}
					else if (input == 'n')
					{
						DirectX::XMFLOAT3 temp;
						fin >> temp.x >> temp.y >> temp.z;
						temp.x = -temp.x;
						out_Normals.push_back(temp);
					}

					while (input != '\n')
					{
						fin.get(input);
					}
				}
				else if (input == 'f')
				{
					DirectX::XMINT3 vIndex, uvIndex, norIndex;

					fin.get(input);
					fin >> vIndex.x >> input2 >> uvIndex.x >> input2 >> norIndex.x
						>> vIndex.y >> input2 >> uvIndex.y >> input2 >> norIndex.y
						>> vIndex.z >> input2 >> uvIndex.z >> input2 >> norIndex.z;

					vertex_indices.push_back(vIndex.x - 1);
					vertex_indices.push_back(vIndex.y - 1);
					vertex_indices.push_back(vIndex.z - 1);

					uv_indices.push_back(uvIndex.x - 1);
					uv_indices.push_back(uvIndex.y - 1);
					uv_indices.push_back(uvIndex.z - 1);

					normal_indices.push_back(norIndex.x - 1);
					normal_indices.push_back(norIndex.y - 1);
					normal_indices.push_back(norIndex.z - 1);
				}
			}
			fin.close();
			return true;
		}
		else
			return false;

		return false;
	}


	bool LoadFBX(const char * path, std::vector<DirectX::XMFLOAT4> &pOutVertexVector,
		std::vector<DirectX::XMFLOAT3>& out_UVs, std::vector<DirectX::XMFLOAT3>& out_Normals,
		std::vector<DirectX::XMFLOAT3>& out_Tangets)
	{
		//FbxManager* g_pFbxManager = nullptr;
		//
		//if (g_pFbxManager == nullptr)
		//{
		//	g_pFbxManager = FbxManager::Create();
		//
		//	FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxManager, IOSROOT);
		//	g_pFbxManager->SetIOSettings(pIOsettings);
		//}
		//
		//FbxImporter* pImporter = FbxImporter::Create(g_pFbxManager, "");
		//FbxScene* pFbxScene = FbxScene::Create(g_pFbxManager, "");
		//
		//bool Successs = pImporter->Initialize(path, -1, g_pFbxManager->GetIOSettings());
		//
		//if (!Successs)
		//	return false;
		//
		//Successs = pImporter->Import(pFbxScene);
		//if (!Successs)
		//	return false;
		//
		//pImporter->Destroy();
		//
		//FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
		//
		//if (pFbxRootNode)
		//{
		//	for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		//	{
		//		FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
		//		if (pFbxChildNode->GetNodeAttribute() == NULL)
		//			continue;
		//
		//		FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();
		//
		//		if (AttributeType != FbxNodeAttribute::eMesh)
		//			continue;
		//		
		//		fbxsdk::FbxMesh* pMesh = (fbxsdk::FbxMesh*)pFbxChildNode->GetNodeAttribute();
		//
		//		FbxVector4* pVertices = pMesh->GetControlPoints();
		//		FbxVector4 pTangent;
		//		FbxVector4 pNormals;
		//		FbxVector2 pUVs;
		//
		//		const fbxsdk::FbxGeometryElementNormal * lNormalElement = pMesh->GetElementNormal(0);
		//		const fbxsdk::FbxGeometryElementUV * lUVElement = pMesh->GetElementUV(0);
		//		const fbxsdk::FbxGeometryElementBinormal* TangentElement = pMesh->GetElementBinormal(0);
		//
		//		for (int j = 0; j < pMesh->GetPolygonCount(); j++)
		//		{
		//			
		//			int iNumVertices = pMesh->GetPolygonSize(j);
		//			assert(iNumVertices == 3);
		//
		//			for (int k = 0; k < iNumVertices; k++)
		//			{
		//				int iControlPointIndex = pMesh->GetPolygonVertex(j, k);
		//
		//				DirectX::XMFLOAT4 vertex;
		//				vertex.x = -(float)pVertices[iControlPointIndex].mData[0];
		//				vertex.y = (float)pVertices[iControlPointIndex].mData[1];
		//				vertex.z = (float)pVertices[iControlPointIndex].mData[2];
		//				vertex.w = 1;
		//				pOutVertexVector.push_back(vertex);
		//
		//				pMesh->GetPolygonVertexNormal(j, k, pNormals);
		//				DirectX::XMFLOAT3 Normal;
		//
		//				Normal.x = -(float)pNormals.mData[0];
		//				Normal.y = (float)pNormals.mData[1];
		//				Normal.z = (float)pNormals.mData[2];
		//				out_Normals.push_back(Normal);
		//
		//				
		//				int UvIndex = pMesh->GetTextureUVIndex(j, k);
		//				pUVs = lUVElement->GetDirectArray().GetAt(UvIndex);
		//				DirectX::XMFLOAT3 UV;
		//				UV.x = (float)pUVs.mData[0];
		//				UV.y = 1.0f - (float)pUVs.mData[1];
		//				out_UVs.push_back(UV);
		//
		//				//pTangent = TangentElement->GetDirectArray().GetAt(UvIndex);
		//				//DirectX::XMFLOAT3 tangent;
		//				//tangent.x = (float)pTangent.mData[0];
		//				//tangent.y = (float)pTangent.mData[1];
		//				//tangent.z = (float)pTangent.mData[2];
		//				//out_Tangets.push_back(tangent);
		//			}
		//
		//
		//		}
		//	}
		//}
		//return true;
		return LoadFBXDLL(path, pOutVertexVector, out_UVs, out_Normals, out_Tangets);
	}

}