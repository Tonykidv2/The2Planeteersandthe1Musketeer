#pragma once
#include "..\FBXImporter\Utilities.h"

struct BoneAnimation
{
	float GetStartTime()const;
	float GetEndTime()const;

	void Interpolate(float t, DirectX::XMFLOAT4X4& M)const;

	std::vector<KeyFrame*> Keyframes;

	~BoneAnimation()
	{
		
	}
};

struct AnimationClip
{
	float GetClipStartTime()const;
	float GetClipEndTime()const;

	void Interpolate(float t, DirectX::XMFLOAT4X4(*boneTransforms)[96])const;

	std::vector<BoneAnimation*> BoneAnimations;

	~AnimationClip()
	{
		
	}
};

struct AnimationController
{
	float CurrTime;
	int WhichAnimation;
	std::vector<AnimationClip> Anim;

	AnimationController()
	{
		CurrTime = 0.1f;
		WhichAnimation = 0;
	}
	void Update(float _dt, DirectX::XMFLOAT4X4(*boneTransforms)[96]);
	
};