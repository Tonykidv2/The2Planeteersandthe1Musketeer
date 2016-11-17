#pragma once
#include "MathHelper.h"
#include "stdafx.h"

struct PNTVertex
{
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Normal;
	DirectX::XMFLOAT2 m_UV;

	bool operator ==(const PNTVertex& rhs) const
	{
		uint32_t position;
		uint32_t normal;
		uint32_t uv;

		DirectX::XMVectorEqualR(&position, DirectX::XMLoadFloat3(&(this->m_Position)), DirectX::XMLoadFloat3(&rhs.m_Position));
		DirectX::XMVectorEqualR(&normal, DirectX::XMLoadFloat3(&(this->m_Normal)), DirectX::XMLoadFloat3(&rhs.m_Normal));
		DirectX::XMVectorEqualR(&uv, DirectX::XMLoadFloat2(&(this->m_UV)), DirectX::XMLoadFloat2(&rhs.m_UV));

		return DirectX::XMComparisonAllTrue(position) && DirectX::XMComparisonAllTrue(normal) && DirectX::XMComparisonAllTrue(uv);
	}
};

struct VertexBlendingInfo
{
	unsigned int m_BlendingIndex;
	double m_BlendingWeight;

	VertexBlendingInfo() : m_BlendingIndex(0), m_BlendingWeight(0.0) {}
	bool operator <(const VertexBlendingInfo& rhs)
	{
		return (m_BlendingWeight > rhs.m_BlendingWeight);
	}
};

struct PNTIWVertex
{
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Normal;
	DirectX::XMFLOAT2 m_UV;
	DirectX::XMFLOAT3 m_Tangent;
	std::vector<VertexBlendingInfo> m_VertexBlendingInfos;

	void SortBlendingInfoByWeight()
	{
		std::sort(m_VertexBlendingInfos.begin(), m_VertexBlendingInfos.end());
	}

	bool operator==(const PNTIWVertex& rhs) const
	{
		bool sameBlendingInfo = true;
		
		if (!(m_VertexBlendingInfos.empty() && rhs.m_VertexBlendingInfos.empty()))
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				if (m_VertexBlendingInfos[i].m_BlendingIndex != rhs.m_VertexBlendingInfos[i].m_BlendingIndex ||
					abs(m_VertexBlendingInfos[i].m_BlendingWeight - rhs.m_VertexBlendingInfos[i].m_BlendingWeight) > 0.001)
				{
					sameBlendingInfo = false;
					break;
				}
			}
		}
		bool result1 = MathHelper::CompareVector3WithEpsilon(m_Position, rhs.m_Position);
		bool result2 = MathHelper::CompareVector3WithEpsilon(m_Normal, rhs.m_Normal);
		bool result3 = MathHelper::CompareVector2WithEpsilon(m_UV, rhs.m_UV);

		return result1 && result2 && result3 && sameBlendingInfo;
	}
};

struct SkinnedVertex
{
	DirectX::XMFLOAT3 m_Positon;
	DirectX::XMFLOAT3 m_Normal;
	DirectX::XMFLOAT2 m_UV;
	DirectX::XMFLOAT3 m_Tangent;
	DirectX::XMFLOAT3 m_JointWeight;
	unsigned int m_JointIndex[4];
};

struct cBufferSkeleton
{
	//std::vector<DirectX::XMFLOAT4X4> JointPostion;
	DirectX::XMFLOAT4X4 JointPostion[96];

	cBufferSkeleton()
	{
		//JointPostion.resize(96);
		for (int i = 0; i < 96; i++)
		{
			JointPostion[i] = { 1,0,0,0, 0,1,0,0, 0,0,1,0 ,0,0,0,1 };
		}
	}
};