#include "SkinnedAnimations.h"

float BoneAnimation::GetStartTime()const
{
	// Keyframes are sorted by time, so first keyframe gives start time.
	return (float)Keyframes.front()->m_FrameNum;
}

float BoneAnimation::GetEndTime()const
{
	// Keyframes are sorted by time, so last keyframe gives end time.
	float f = (float)Keyframes.back()->m_FrameNum;

	return f;
}

void BoneAnimation::Interpolate(float t, DirectX::XMFLOAT4X4& M)const
{
	if (t <= Keyframes.front()->m_FrameNum)
	{
		
		DirectX::XMVECTOR S = DirectX::XMLoadFloat3(&Keyframes.front()->Scale);
		DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&Keyframes.front()->Translation);
		DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&Keyframes.front()->RotationQuat);

		DirectX::XMVECTOR zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		DirectX::XMStoreFloat4x4(&M, DirectX::XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else if (t >= Keyframes.back()->m_FrameNum)
	{
		float lerpPercent = (t / (Keyframes.back()->m_FrameNum + Keyframes[Keyframes.size() -1]->m_FrameNum));

		
		DirectX::XMVECTOR S = DirectX::XMLoadFloat3(&Keyframes.back()->Scale);
		DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&Keyframes.back()->Translation);
		DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&Keyframes.back()->RotationQuat);

		DirectX::XMVECTOR zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		DirectX::XMStoreFloat4x4(&M, DirectX::XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else 
	{
		for (UINT i = 0; i < Keyframes.size() - 1; ++i)
		{
			if (t >= Keyframes[i]->m_FrameNum && t <= Keyframes[i + 1]->m_FrameNum)
			{
				float lerpPercent = (t - Keyframes[i]->m_FrameNum) / (Keyframes[i + 1]->m_FrameNum - Keyframes[i]->m_FrameNum);

				DirectX::XMVECTOR s0 = DirectX::XMLoadFloat3(&Keyframes[i]->Scale);
				DirectX::XMVECTOR s1 = DirectX::XMLoadFloat3(&Keyframes[i + 1]->Scale);

				DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&Keyframes[i]->Translation);
				DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&Keyframes[i + 1]->Translation);

				DirectX::XMVECTOR q0 = DirectX::XMLoadFloat4(&Keyframes[i]->RotationQuat);
				DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4(&Keyframes[i + 1]->RotationQuat);

				DirectX::XMVECTOR S = DirectX::XMVectorLerp(s0, s1, lerpPercent);
				DirectX::XMVECTOR P = DirectX::XMVectorLerp(p0, p1, lerpPercent);
				DirectX::XMVECTOR Q = DirectX::XMQuaternionSlerp(q0, q1, lerpPercent);

				DirectX::XMVECTOR zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				DirectX::XMStoreFloat4x4(&M, DirectX::XMMatrixAffineTransformation(S, zero, Q, P));
				
				break;
			}
		}
	}
	
}


float AnimationClip::GetClipStartTime()const
{
	// Find smallest start time over all bones in this clip.
	float t = FLT_MAX;
	for (unsigned int i = 0; i < BoneAnimations.size(); ++i)
	{
		if (BoneAnimations[i].Keyframes.size() == 0)
			continue;
		t = min(t, BoneAnimations[i].GetStartTime());
	}

	return t;
}

float AnimationClip::GetClipEndTime()const
{
	// Find largest end time over all bones in this clip.
	float t = 0.0f;
	for (unsigned int i = 0; i < BoneAnimations.size(); ++i)
	{
		if (BoneAnimations[i].Keyframes.size() == 0)
			continue;
		t = max(t, BoneAnimations[i].GetEndTime());
	}

	return t;
}

void AnimationClip::Interpolate(float t, DirectX::XMFLOAT4X4(*boneTransforms)[50])const
{
	for (unsigned int i = 0; i < BoneAnimations.size(); ++i)
	{
		if (BoneAnimations[i].Keyframes.size() == 0)
			continue;
		BoneAnimations[i].Interpolate(t, (*boneTransforms)[i]);
	}
}

void AnimationController::Update(float _dt, DirectX::XMFLOAT4X4(*boneTransforms)[50])
{
	if (GetAsyncKeyState(VK_NUMPAD0) & 0x1)
	{
		WhichAnimation++;
		if (WhichAnimation > Anim.size() - 1)
			WhichAnimation = 0;
	}
	CurrTime += _dt;

	Anim[WhichAnimation].Interpolate(CurrTime, boneTransforms);

	if (CurrTime >= Anim[WhichAnimation].GetClipEndTime())
		CurrTime = 0.95f;
}

void AnimationBlending::Update(float _dt, AnimationClip fromthis, AnimationClip ToThis, DirectX::XMFLOAT4X4(*OutTransform)[50])
{
	
}