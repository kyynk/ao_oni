#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "Entity.h"
namespace game_framework {
	Entity::Entity() {
		_counter = 0;		//initialize
		_istimerstart = false;
	}

	int Entity::getX1() {
		return _pos_x;
	}
	int Entity::getX2() {
		return _pos_x + bitmap.GetWidth();
	}
	int Entity::getY1() {
		return _pos_y;
	}
	int Entity::getY2() {
		return _pos_y + bitmap.GetHeight();
	}
	void Entity::SetXY(int x, int y) {
		_pos_x = x;
		_pos_y = y;
	}
	void Entity::TimerReset() {
		_counter = 0;
	}
	void Entity::TimerStop() {
		_counter = 0;
		_istimerstart = false;
	}
	void Entity::TimerUpdate() {
		if(_istimerstart)_counter++;
	}
	void Entity::TimerStart() {
		_istimerstart = true;
	}
	int Entity::TimerGetCount() {
		return _counter;
	}
	bool Entity::IsTimerStart()
	{
		return _istimerstart;
	}
	void Entity::OnMove()
	{
	}
	void Entity::OnShow()
	{
	}
	void Entity::Load()
	{
	}
	void Entity::SelectShowBitmap(int index) {
		bitmap.SetFrameIndexOfBitmap(index);
	}
	

}

