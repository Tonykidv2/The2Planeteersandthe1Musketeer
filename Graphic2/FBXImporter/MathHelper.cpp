#include "stdafx.h"
#include "MathHelper.h"

const DirectX::XMFLOAT2 MathHelper::vector2Epsilon = DirectX::XMFLOAT2(0.00001f, 0.00001f);
const DirectX::XMFLOAT3 MathHelper::vector3Epsilon = DirectX::XMFLOAT3(0.00001f, 0.00001f, 0.00001f);

bool MathHelper::CompareVector3WithEpsilon(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	return DirectX::XMVector3NearEqual(DirectX::XMLoadFloat3(&lhs), DirectX::XMLoadFloat3(&rhs), DirectX::XMLoadFloat3(&vector3Epsilon)) == TRUE;
}

bool MathHelper::CompareVector2WithEpsilon(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	return DirectX::XMVector3NearEqual(DirectX::XMLoadFloat2(&lhs), DirectX::XMLoadFloat2(&rhs), DirectX::XMLoadFloat2(&vector2Epsilon)) == TRUE;
}