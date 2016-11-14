#pragma once
#include <Windows.h>
#include <stdint.h>
#include "stdafx.h"

class MathHelper
{
public:
	static const DirectX::XMFLOAT3 vector3Epsilon;
	static const DirectX::XMFLOAT2 vector2Epsilon;
	static const DirectX::XMFLOAT3 vector3True;
	static const DirectX::XMFLOAT2 vector2True;

	static bool CompareVector2WithEpsilon(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
	static bool CompareVector3WithEpsilon(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);
};
