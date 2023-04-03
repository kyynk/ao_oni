#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "config.h"
#include "MapRouter.h"
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
	/*bool Entity::UBlocked()
	{
		return MapRouter::GetInstance()->IsInBanlist(_pos_x/TILE, _uy / TILE);
	}
	bool Entity::DBlocked()
	{
		return MapRouter::GetInstance()->IsInBanlist(_pos_x / TILE, _dy / TILE);
	}
	bool Entity::LBlocked()
	{
		return MapRouter::GetInstance()->IsInBanlist(_lx / TILE, _pos_y / TILE);
	}
	bool Entity::RBlocked()
	{
		return MapRouter::GetInstance()->IsInBanlist(_rx / TILE,_pos_y / TILE);
	}*/
	//void Entity::OnMove()
	//{
	//}
	//void Entity::OnShow()
	//{
	//}
	//void Entity::Load()
	//{
	//}
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

