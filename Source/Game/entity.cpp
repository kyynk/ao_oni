#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "entity.h"
#define HERO_DOWN 0
#define HERO_DOWN_WALK_1 1
#define HERO_DOWN_WALK_2 2

#define HERO_UP	4
#define HERO_UP_WALK_1 5
#define HERO_UP_WALK_2 6

#define HERO_LEFT 7
#define HERO_LEFT_WALK_1 8
#define HERO_LEFT_WALK_2 9

#define HERO_RIGHT 10
#define HERO_RIGHT_WALK_1 11
#define HERO_RIGHT_WALK_2 12
namespace game_framework {

	Entity::Entity() {

	}
	int Entity::getX1() {
		return _x;
	}
	int Entity::getX2() {
		return _x + _animation.GetWidth();
	}
	int Entity::getY1() {
		return _y;
	}
	int Entity::getY2() {
		return _y + _animation.GetHeight();
	}
	int Entity::getDirection() {
		return _direction;
	}
	int Entity::getState() {
		return _state;
	}
	void Entity::SetState(FSM state) {
		_state = state;
	}
	void Entity::OnMove() {
		if (_blocked) {

		}
		else if (_state > 0 && _state < 5) {
			if (TimerGetCount() < 8) {
				_bstate = s1;
			}
			else {
				_bstate = s2;
			}
			if (TimerGetCount() == 16) {
				TimerReset();
				_walkiter = !_walkiter;
				_state = still;
				//_MovingUp = _MovingDown = _MovingLeft = _MovingRight = false;
			}
			else if (_state == movingup) {
				_y -= step;
			}
			else if (_state == movingdown) {
				_y += step;
			}
			else if (_state == movingleft) {
				_x -= step;
			}
			else if (_state == movingright) {
				_x += step;
			}
		}
		if (_state != 0)
			TimerUpdate();

	}
	void Entity::OnShow() {
		_animation.SetTopLeft(_x, _y);
		switch (_state) {
		case movingup:
			if (_bstate == s1) {
				_walkiter ? _animation.SetFrameIndexOfBitmap(HERO_UP_WALK_1) : _animation.SetFrameIndexOfBitmap(HERO_UP_WALK_2);
			}
			else {
				_animation.SetFrameIndexOfBitmap(HERO_UP);
			}
			break;


		case movingdown:
			if (_bstate == s1) {
				_walkiter ? _animation.SetFrameIndexOfBitmap(HERO_DOWN_WALK_1) : _animation.SetFrameIndexOfBitmap(HERO_DOWN_WALK_2);
			}
			else {
				_animation.SetFrameIndexOfBitmap(HERO_DOWN);
			}
			break;

		case movingleft:
			if (_bstate == s1) {
				_walkiter ? _animation.SetFrameIndexOfBitmap(HERO_LEFT_WALK_1) : _animation.SetFrameIndexOfBitmap(HERO_LEFT_WALK_2);
			}
			else {
				_animation.SetFrameIndexOfBitmap(HERO_LEFT);
			}
			break;

		case movingright:
			if (_bstate == s1) {
				_walkiter ? _animation.SetFrameIndexOfBitmap(HERO_RIGHT_WALK_1) : _animation.SetFrameIndexOfBitmap(HERO_RIGHT_WALK_2);
			}
			else {
				_animation.SetFrameIndexOfBitmap(HERO_RIGHT);
			}
			break;
		default:
			__asm {nop}
		}
		_animation.ShowBitmap();
	}

	void Entity::PressKeyDown(bool flag) {
		(_direction == down) ? _state = movingdown : SetDirection(down);
	}
	void Entity::PressKeyUp(bool flag) {
		 (_direction == up) ? _state = movingup : SetDirection(up);
	}
	void Entity::PressKeyLeft(bool flag) {
		 (_direction == left) ? _state = movingleft : SetDirection(left);
	}
	void Entity::PressKeyRight(bool flag) {
		 (_direction == right) ? _state = movingright : SetDirection(right);
	}

	void Entity::SetXY(int x, int y) {
		_x = x;
		_y = y;
	}
	void Entity::LoadBitmap() {
		vector<string> tmp;
		for (int i = 1; i < 37; i++) {
			tmp.push_back("res/player_" + to_string(i) + ".bmp");
		}
		_animation.LoadBitmapByString(tmp, RGB(255, 0, 228));
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
	void Entity::SelectShowBitmap(int index) {
		_animation.SetFrameIndexOfBitmap(index);
	}
	void Entity::SetDirection(Direction direction) {
		_direction = direction;
	}


}