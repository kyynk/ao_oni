#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "Entity.h"
namespace game_framework {
	Entity::Entity() {
		_counter = 0;		//initialize
		_istimerstart = false;	
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
	
	void Entity::SelectShowBitmap(int index) {
		bitmap.SetFrameIndexOfBitmap(index);
	}

	bool Entity::CheckInVec(int n, vector<int> vec)
	{
		for (auto f : vec) {
			if (n == f) {
				return true;
			}
		}
		return false;
	}

	

}

