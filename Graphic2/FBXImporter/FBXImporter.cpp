// FBXImporter.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FBXImporter.h"
#include "fbxsdk.h"
#include "Utilities.h"
#include "Vertex.h"
#include <unordered_map>

class FBXExporter
{
private:
	FbxManager* m_FbxManager;
	FbxScene* m_Scene;
	bool m_HasAnimation;
	std::unordered_map<unsigned int, CtrlPoint*>m_ControlPoints;
	unsigned int m_TriangleCount;
	std::vector<Triangle>m_Triangles;
	std::vector<PNTIWVertex>m_Vertices;
	Skeleton m_Skeleton;
	FbxLongLong m_AnimationLength;
	std::string m_AnimationName;
	LARGE_INTEGER m_CPUFreq;

	void ProcessSkeletonHeirarchy(FbxNode* m_rootNode);
	void ProcessSkeletonHeirarchyRecursively(FbxNode* m_Node, int m_Depth, int myIndex, int m_ParentIndex);
	void ProcessControlPoints(FbxNode* m_Node);
	void ProcessJointsAndAnimations(FbxNode* m_Node);
	unsigned int FindJointIndexUsingName(const std::string& m_JointName);
	void ProcessMesh(FbxNode* m_Node);
	void ReadUV(FbxMesh* m_Mesh, int m_CtrlPointIndex, int m_TextureUVIndex, int m_UVLayer, DirectX::XMFLOAT2& outUV);
	void ReadNormal(FbxMesh* m_Mesh, int m_CtrlPointIndex, int m_vertexCounter, DirectX::XMFLOAT3& m_outNormal);
	void ReadTangent(FbxMesh* m_Mesh, int m_CtrlPointIndex, int m_VertexCounter, DirectX::XMFLOAT3& m_outTangent);
	void ProcessGeometry(FbxNode *m_Node);
};




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
//}

#include <fbxsdk.h>


extern "C" __declspec(dllexport) bool LoadFBXDLL(const char * path, std::vector<DirectX::XMFLOAT4>& pOutVertexVector,
	std::vector<DirectX::XMFLOAT3>& out_UVs, std::vector<DirectX::XMFLOAT3>& out_Normals,
	std::vector<DirectX::XMFLOAT3>& out_Tangets)
{


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

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		int ChildCount = pFbxRootNode->GetChildCount();

		for (int i = 0; i < ChildCount; i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
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
					pOutVertexVector.push_back(vertex);

					pMesh->GetPolygonVertexNormal(j, k, pNormals);
					DirectX::XMFLOAT3 Normal;

					Normal.x = -(float)pNormals.mData[0];
					Normal.y = (float)pNormals.mData[1];
					Normal.z = (float)pNormals.mData[2];
					out_Normals.push_back(Normal);


					int UvIndex = pMesh->GetTextureUVIndex(j, k);
					pUVs = lUVElement->GetDirectArray().GetAt(UvIndex);
					DirectX::XMFLOAT3 UV;
					UV.x = (float)pUVs.mData[0];
					UV.y = 1.0 - (float)pUVs.mData[1];
					out_UVs.push_back(UV);

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

	pFbxRootNode->Destroy();
	pImporter->Destroy();
	pFbxScene->Destroy();
	g_pFbxManager->Destroy();
	return true;
}
/*
FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex)
{
	FbxAMatrix localposeMatrix;
	FbxMatrix localMatrix = pPose->GetMatrix(pNodeIndex);
	memcpy((double*)localposeMatrix, (double*)localMatrix, sizeof(localMatrix.mData));
	return localposeMatrix;
}

FbxAMatrix GetGeometry(FbxNode* pNode)
{
	const FbxVector4 localTransform = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 localRotation = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 localScaling = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(localTransform, localRotation, localScaling);
}

FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose = NULL, FbxAMatrix* parentGlobalPosition = NULL)
{
	FbxAMatrix globalPosition;
	bool positionFound = false;

	if (pPose)
	{
		int nodeIndex = pPose->Find(pNode);
		if (nodeIndex > -1)
		{
			if (pPose->IsBindPose() || !pPose->IsLocalMatrix(nodeIndex))
			{
				globalPosition = GetPoseMatrix(pPose, nodeIndex);
			}
			else
			{
				FbxAMatrix m_parentGlobalPosition;
				if (parentGlobalPosition)
				{
					m_parentGlobalPosition = *parentGlobalPosition;
				}
				else
				{
					if (pNode->GetParent())
					{
						m_parentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
					}
				}
				FbxAMatrix localPosition = GetPoseMatrix(pPose, nodeIndex);
				globalPosition = m_parentGlobalPosition * localPosition;
			}
			positionFound = true;
		}
	}

	if (!positionFound)
	{
		globalPosition = pNode->EvaluateGlobalTransform(pTime);
	}
	return globalPosition;
}


		
void ComputeClusterDeformation(FbxAMatrix& globalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose)
{
	//lClusterRelativeInitPosition -> inverse bind pose
	//lClusterRelativeCurrentPositionInverse -> bone offset
	//Relative -> to mesh,  cluster-> bone,

	FbxCluster::ELinkMode clusterMode = pCluster->GetLinkMode();

	FbxAMatrix lRefGlobalInitPosition;
	FbxAMatrix lRefGlobalCurrentPosition;
	
	FbxAMatrix lClusterGlobalInitPosition;
	FbxAMatrix lClusterGlobalCurrentPosition;

	FbxAMatrix lRefGeometry;
	FbxAMatrix lClusterGeometry;

	FbxAMatrix lClusterRelativeInitPosition;
	FbxAMatrix lClusterRelativeCurrentPositionInverse;

	if (!clusterMode == FbxCluster::eAdditive && !pCluster->GetAssociateModel())
	{
		pCluster->GetTransformMatrix(lRefGlobalInitPosition);
		lRefGlobalCurrentPosition = globalPosition;
		lRefGeometry = GetGeometry(pMesh->GetNode());
		lRefGlobalInitPosition *= lRefGeometry;

		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

		lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lRefGlobalInitPosition;

		lClusterRelativeCurrentPositionInverse = lRefGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

		pVertexTransformMatrix =  lClusterRelativeInitPosition * lClusterRelativeCurrentPositionInverse;
	}
}
*/


void FBXExporter::ProcessSkeletonHeirarchy(FbxNode* m_rootNode)
{
	for (int childIndex = 0; childIndex < m_rootNode->GetChildCount(); ++childIndex)
	{
		FbxNode* currNode = m_rootNode->GetChild(childIndex);
		
	}
}

void FBXExporter::ProcessSkeletonHeirarchyRecursively(FbxNode* m_Node, int m_Depth, int myIndex, int m_ParentIndex)
{
	if (m_Node->GetNodeAttribute() && m_Node->GetNodeAttribute()->GetAttributeType() && m_Node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint currentJoint;
		currentJoint.m_ParentIndex = m_ParentIndex;
		currentJoint.m_Name = m_Node->GetName();
		m_Skeleton.m_Joints.push_back(currentJoint);
	}
	for (int i = 0; i < m_Node->GetChildCount(); i++)
	{
		ProcessSkeletonHeirarchyRecursively(m_Node->GetChild(i), m_Depth + 1, m_Skeleton.m_Joints.size(), myIndex);
	}
}

void FBXExporter::ProcessControlPoints(FbxNode* m_Node)
{
	FbxMesh* currentMesh = m_Node->GetMesh();
	unsigned int ctrlPointCount = currentMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; i++)
	{
		CtrlPoint* currentCtrlPoint = new CtrlPoint();
		DirectX::XMFLOAT3 currentPosition;
		currentPosition.x = static_cast<float>(currentMesh->GetControlPointAt(i).mData[0]);
		currentPosition.y = static_cast<float>(currentMesh->GetControlPointAt(i).mData[1]);
		currentPosition.z = static_cast<float>(currentMesh->GetControlPointAt(i).mData[2]);
		currentCtrlPoint->m_Position = currentPosition;
		m_ControlPoints[i] = currentCtrlPoint;
	}

}

void FBXExporter::ProcessJointsAndAnimations(FbxNode* m_Node)
{
	FbxMesh* currentMesh = m_Node->GetMesh();
	unsigned int numOfDeformers = currentMesh->GetDeformerCount();
	//This geometry transform is mostly like identity matrix
	FbxAMatrix geometryTransform = Utilities::GetGeometryTransformation(m_Node);

	//A deformer contains some clusters
	//A cluster contains a link, which is basically a joint
	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; deformerIndex++)
	{
		//We're going to use deformer in form of skin
		FbxSkin* currentSkin = reinterpret_cast<FbxSkin*>(currentMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currentSkin)
		{
			continue;
		}

		unsigned int numOfClusters = currentSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; clusterIndex++)
		{
			FbxCluster* currentCluster = currentSkin->GetCluster(clusterIndex);
			std::string currentJointName = currentCluster->GetLink()->GetName();
			unsigned int currentJointIndex = FindJointIndexUsingName(currentJointName);
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			//Transformation of the mesh at binding time
			currentCluster->GetTransformMatrix(transformMatrix);
			//Transformation of the cluster(joint) at binding time from joint space to world space
			currentCluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			//Update the info in mSkeleton
			m_Skeleton.m_Joints[currentJointIndex].m_GlobalBindPoseInverse = globalBindposeInverseMatrix;
			m_Skeleton.m_Joints[currentJointIndex].m_Node = currentCluster->GetLink();

			//Associate each joint with the control points it affects
			unsigned int numOfIndices = currentCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; i++)
			{
				BlendingIndexWeightPair currentBlendingIndexWeightPair;
				currentBlendingIndexWeightPair.m_BlendingIndex = currentJointIndex;
				currentBlendingIndexWeightPair.m_BlendingWeight = currentCluster->GetControlPointWeights()[i];
				m_ControlPoints[currentCluster->GetControlPointIndices()[i]]->m_BlendingInfo.push_back(currentBlendingIndexWeightPair);
			}

			// Get animation info(now only supports one take)
			FbxAnimStack* currentAnimStack = m_Scene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = currentAnimStack->GetName();
			m_AnimationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = m_Scene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			m_AnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
			KeyFrame** currentAnimation = &m_Skeleton.m_Joints[currentJointIndex].m_Animation;

			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); i++)
			{
				FbxTime currentTime;
				currentTime.SetFrame(i, FbxTime::eFrames24);
				*currentAnimation = new KeyFrame();
				(*currentAnimation)->m_FrameNum = i;
				FbxAMatrix currentTransformOffset = m_Node->EvaluateGlobalTransform(currentTime) * geometryTransform;
				(*currentAnimation)->m_GlobalTransform = currentTransformOffset.Inverse() * currentCluster->GetLink()->EvaluateGlobalTransform(currentTime);
				currentAnimation = &((*currentAnimation)->m_Next);
			}
		}
	}

	//Some of the joints hav less than 4 joints affecting them
	//For normal renderer, adding more dummy joints if there isn't enough
	BlendingIndexWeightPair currentBlendingIndexWeightpair;
	currentBlendingIndexWeightpair.m_BlendingIndex = 0;
	currentBlendingIndexWeightpair.m_BlendingWeight = 0;
	for (auto itr = m_ControlPoints.begin(); itr != m_ControlPoints.end(); itr++)
	{
		for (unsigned int i = itr->second->m_BlendingInfo.size(); i <= 4; i++)
		{
			itr->second->m_BlendingInfo.push_back(currentBlendingIndexWeightpair);
		}
	}
}

unsigned int FBXExporter::FindJointIndexUsingName(const std::string& m_JointName)
{
	for (unsigned int i = 0; i < m_Skeleton.m_Joints.size(); i++)
	{
		if (m_Skeleton.m_Joints[i].m_Name == m_JointName)
		{
			return i;
		}
	}
	throw std::exception("Skeleton information in FBX file is corrupted.");
}


void FBXExporter::ReadUV(FbxMesh* m_Mesh, int m_CtrlPointIndex, int m_TextureUVIndex, int m_UVLayer, DirectX::XMFLOAT2& outUV)
{
	if (m_UVLayer >= 2 || m_Mesh->GetElementUVCount() <= m_UVLayer)
	{
		throw std::exception("INvalid UV Layer Number");
	}

	FbxGeometryElementUV* vertexUV = m_Mesh->GetElementUV(m_UVLayer);

	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement:: eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(m_CtrlPointIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(m_CtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(m_CtrlPointIndex);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(m_CtrlPointIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(m_CtrlPointIndex).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(m_CtrlPointIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(m_CtrlPointIndex).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

void FBXExporter::ReadNormal(FbxMesh* m_Mesh, int m_CtrlPointIndex, int m_vertexCounter, DirectX::XMFLOAT3& m_outNormal)
{
	if (m_Mesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementNormal* vertexNormal = m_Mesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
		case FbxGeometryElementNormal::eByControlPoint:
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				m_outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(m_CtrlPointIndex).mData[0]);
				m_outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(m_CtrlPointIndex).mData[1]);
				m_outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(m_CtrlPointIndex).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(m_CtrlPointIndex);
				m_outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				m_outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				m_outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;

			default:
				throw std::exception("Invalid Reference");
			}
		break;

		case FbxGeometryElement::eByPolygonVertex:
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				m_outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(m_vertexCounter).mData[0]);
				m_outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(m_vertexCounter).mData[1]);
				m_outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(m_vertexCounter).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(m_vertexCounter);
				m_outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				m_outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				m_outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);

			}
			break;

			default:
				throw std::exception("Invalid Reference");
			}
			break;
	}
}

void FBXExporter::ReadTangent(FbxMesh* m_Mesh, int m_CtrlPointIndex, int m_VertexCounter, DirectX::XMFLOAT3& m_outTangent)
{
	if (m_Mesh->GetElementTangentCount() < 1)
	 	{
	 		throw std::exception("Invalid Tangent Number");
	 	}
	

	 	FbxGeometryElementTangent* vertexTangent = m_Mesh->GetElementTangent(0);
	switch (vertexTangent->GetMappingMode())
	 	{
	 	case FbxGeometryElement::eByControlPoint:
			switch (vertexTangent->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
			 		m_outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(m_CtrlPointIndex).mData[0]);
			 		m_outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(m_CtrlPointIndex).mData[1]);
			 		m_outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(m_CtrlPointIndex).mData[2]);
			 	}
				break;
			

			case FbxGeometryElement::eIndexToDirect:
			{
			 	int index = vertexTangent->GetIndexArray().GetAt(m_CtrlPointIndex);
			 	m_outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			 	m_outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			 	m_outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			
			default:
				throw std::exception("Invalid Reference");
			}
			break;
			

		case FbxGeometryElement::eByPolygonVertex:
			 switch (vertexTangent->GetReferenceMode())
			 {
				 case FbxGeometryElement::eDirect:
				  {
						m_outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(m_VertexCounter).mData[0]);
						m_outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(m_VertexCounter).mData[1]);
						m_outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(m_VertexCounter).mData[2]);
				  }
				  break;
					

				 case FbxGeometryElement::eIndexToDirect:
				 {
					    int index = vertexTangent->GetIndexArray().GetAt(m_VertexCounter);
					    m_outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
					    m_outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
					    m_outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
				  }
				  break;
				  default:
					throw std::exception("Invalid Reference");
		       }
			 break;
			 	}
 }

void FBXExporter::ProcessMesh(FbxNode* m_Node)
{
	FbxMesh* currentMesh = m_Node->GetMesh();

	m_TriangleCount = currentMesh->GetPolygonCount();
	int vertexCounter = 0;
	m_Triangles.reserve(m_TriangleCount);

	for (unsigned int i = 0; i < m_TriangleCount; i++)
	{
		DirectX::XMFLOAT3 normal[3];
		DirectX::XMFLOAT3 tangent[3];
		//DirectX::XMFLOAT3 binormal[3];
		DirectX::XMFLOAT2 UV[3][2];
		Triangle currentTriangle;
		m_Triangles.push_back(currentTriangle);

		for (unsigned int j = 0; j < 3; j++)
		{
			int ctrlPointIndex = currentMesh->GetPolygonVertex(i, j);
			CtrlPoint* currentCtrlPoint = m_ControlPoints[ctrlPointIndex];

			ReadNormal(currentMesh, ctrlPointIndex, vertexCounter, normal[j]);
			// diffuse texture
			for (int k = 0; k < 1; k++)
			{
				ReadUV(currentMesh, ctrlPointIndex, currentMesh->GetTextureUVIndex(i, j), k, UV[j][k]);
			}

			PNTIWVertex temp;
			temp.m_Position = currentCtrlPoint->m_Position;
			temp.m_Normal = normal[j];
			temp.m_UV = UV[j][0];

			//Copying the blending info from each control point
			for (unsigned int i = 0; i < currentCtrlPoint->m_BlendingInfo.size(); i++)
			{
				VertexBlendingInfo currentBlendingInfo;
				currentBlendingInfo.m_BlendingIndex = currentCtrlPoint->m_BlendingInfo[i].m_BlendingIndex;
				currentBlendingInfo.m_BlendingWeight = currentCtrlPoint->m_BlendingInfo[i].m_BlendingWeight;
				temp.m_VertexBlendingInfos.push_back(currentBlendingInfo);
			}
			// Sorting the blending info -duplicated vertices
			temp.SortBlendingInfoByWeight();
			m_Vertices.push_back(temp);
			m_Triangles.back().m_Indices.push_back(vertexCounter);
			vertexCounter++;
				
		}
	}

	//Now mControlPoints has served its purpose, free the memory
	for (auto itr = m_ControlPoints.begin(); itr != m_ControlPoints.end(); itr++)
	{
		delete itr->second;
	}
	m_ControlPoints.clear();
}

void FBXExporter::ProcessGeometry(FbxNode *m_Node)
{
	if (m_Node->GetNodeAttribute())
	 	{
	 		switch (m_Node->GetNodeAttribute()->GetAttributeType())
			{
			case FbxNodeAttribute::eMesh:
	 			ProcessControlPoints(m_Node);
	 			if (m_HasAnimation)
	 			{
	 				ProcessJointsAndAnimations(m_Node);
	 			}
	 			ProcessMesh(m_Node);
	 			break;
	 		}
	}

	for (int i = 0; i < m_Node->GetChildCount(); ++i)
	{
		ProcessGeometry(m_Node->GetChild(i));
	}
}

