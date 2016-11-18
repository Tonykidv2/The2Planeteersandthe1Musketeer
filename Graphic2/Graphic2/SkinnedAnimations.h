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

	void Interpolate(float t, DirectX::XMFLOAT4X4(*boneTransforms)[50])const;

	std::vector<BoneAnimation> BoneAnimations;

	~AnimationClip()
	{
		BoneAnimations.clear();
	}
};

struct AnimationController
{
	float CurrTime;
	int WhichAnimation;
	std::vector<AnimationClip> Anim;

	AnimationController()
	{
		CurrTime = 0.95f;
		WhichAnimation = 0;
		
	}
	void Update(float _dt, DirectX::XMFLOAT4X4(*boneTransforms)[50]);
	
};

struct AnimationBlending
{
	float CurrTime;
	float TimeOut;

	bool active;
	void Update(float _dt, AnimationClip fromthis, AnimationClip ToThis, DirectX::XMFLOAT4X4(*OutTransform)[50]);

	AnimationBlending()
	{
		active = false;
		TimeOut = 1.0;
	}
};