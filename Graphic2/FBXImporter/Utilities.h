#pragma once
#include <fbxsdk.h>
#include "stdafx.h"

struct BlendingIndexWeightPair
{
	unsigned int m_BlendingIndex;
	double m_BlendingWeight;

	BlendingIndexWeightPair() : m_BlendingIndex(0), m_BlendingWeight(0) {}
};

struct CtrlPoint
{
	DirectX::XMFLOAT3 m_Position;
	std::vector<BlendingIndexWeightPair> m_BlendingInfo;
	CtrlPoint()
	{
		m_BlendingInfo.reserve(4);
	}
};

struct KeyFrame
{
	FbxLongLong m_FrameNum;
	FbxAMatrix m_GlobalTransform;
	KeyFrame* m_Next;

	KeyFrame() : m_Next(nullptr) {}
};

struct Joint
{
	std::string m_Name;
	int m_ParentIndex;
	FbxAMatrix m_GlobalBindPoseInverse;
	KeyFrame* m_Animation;
	FbxNode* m_Node;

	Joint() : m_Node(nullptr), m_Animation(nullptr)
	{
		m_GlobalBindPoseInverse.SetIdentity();
		m_ParentIndex = -1;
	}
	~Joint()
	{
		while (m_Animation)
		{
			KeyFrame* temp = m_Animation->m_Next;
			delete m_Animation;
			m_Animation = temp;
		}
	}
};

struct Skeleton
{
	std::vector<Joint> m_Joints;
};

struct Triangle
{
	std::vector<unsigned int> m_Indices;
	std::string m_MaterialName;
	unsigned int m_MaterialIndex;

	bool operator<(const Triangle& rhs)
	{
		return m_MaterialIndex < rhs.m_MaterialIndex;
	}
};

class Utilities
{
public:
	static FbxAMatrix GetGeometryTransformation(FbxNode* m_Node);
};
