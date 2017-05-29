#include "../../includes/aleung-c_engine.hpp"

/*
** The file for the GameObject's morph targeting animation methods.
**	Class nested in GameObject handling the morph targeting.
**	Implemented as a side component of the GameObject.
*/

MorphTargetAnimation::MorphTargetAnimation()
:
	StartFrame(NULL),
	CurFrame(NULL),
	NextFrame(NULL),
	_isMorphAnimated(false),
	_animCurTime(0.0),
	_animSpeed(0.01),
	_started(false),
	_repeat(false)
{
	if (_started)
	{}
}

MorphTargetAnimation::~MorphTargetAnimation()
{

}

void				MorphTargetAnimation::AddKeyFrame(GameObject *obj)
{
	KeyFrames.push_back(obj);
	// First Call initialization.
	if (_isMorphAnimated == false)
	{
		Frame_i = 1;
		_isMorphAnimated = true;
		CurFrame = StartFrame;
		NextFrame = obj;
	}
}

void				MorphTargetAnimation::ClearFrames()
{
	KeyFrames.clear();
	_isMorphAnimated = false;
	_started = false;
	_repeat = false;
}

// --------------------------------------------------------------------	//
//																		//
//	Morph target animation control										//
//																		//
// --------------------------------------------------------------------	//

void				MorphTargetAnimation::SetSpeed(float newSpeed)
{
	if (newSpeed >= 0.0)
		_animSpeed = newSpeed;
	else
		_animSpeed = 0.0;
}

void				MorphTargetAnimation::Start()
{
	_started = true;
}

void				MorphTargetAnimation::Update()
{
	if (_started == true)
	{
		_animCurTime += _animSpeed;
	}
	if (_animCurTime >= 1.0)
	{
		CurFrame = NextFrame;
		Frame_i += 1;
		if ((int)KeyFrames.size() > Frame_i)
		{
			NextFrame = KeyFrames[Frame_i - 1];
		}
		else
		{
			CurFrame = StartFrame;
			if (_repeat)
			{
				_animCurTime = 0.0;
				Frame_i = 1;
			}
			else
				Stop();
		}
		
	}
}

void				MorphTargetAnimation::Stop()
{
	_started = false;
	_animCurTime = 0.0;
}

/*
**	Accessors / animation settings
*/

float				MorphTargetAnimation::GetCurTime()
{
	return (_animCurTime);
}

void				MorphTargetAnimation::SetRepeat(bool val)
{
	_repeat = val;
}

bool				MorphTargetAnimation::IsRepeating()
{
	return (_repeat);
}

bool				MorphTargetAnimation::HasStarted()
{
	return (_started);
}

bool				MorphTargetAnimation::IsAnimated()
{
	return (_isMorphAnimated);
}
