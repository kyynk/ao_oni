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
	}

	int Entity::getX1() {
		return pos_x;
	}
	int Entity::getX2() {
		return pos_x + bitmap.GetWidth();
	}
	int Entity::getY1() {
		return pos_y;
	}
	int Entity::getY2() {
		return pos_y + bitmap.GetHeight();
	}
	void Entity::SetXY(int x, int y) {
		pos_x = x;
		pos_y = y;
	}
	void Entity::TimerReset() {
		_counter = 0;
	}
	void Entity::TimerUpdate() {
		_counter++;
	}
	int Entity::TimerGetCount() {
		return _counter;
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

