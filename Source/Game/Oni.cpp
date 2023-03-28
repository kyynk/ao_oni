#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include <vector>
#include "config.h"
#include "Entity.h"
#include "Oni.h"


#define ONI_DOWN 0
#define ONI_DOWN_1 1
#define ONI_DOWN_2 2
#define ONI_UP 9
#define ONI_UP_1 10
#define ONI_UP_2 11
#define ONI_LEFT 3
#define ONI_LEFT_1 4
#define ONI_LEFT_2 5
#define ONI_RIGHT 6
#define ONI_RIGHT_1 7
#define ONI_RIGHT_2 8

namespace game_framework {
	Oni::Oni() {
		_type = normal;
		_posX, _posY, _time,
		_humanX, _humanY = 0;
		_isDisappear = false;
	}
	Oni::~Oni() {
	}
	void Oni::SetParam(Oni::OniType tp) {
		_type = tp;
		_time = 10;
		string name;
		vector<string> oniVec;
		if (_type == normal) {
			name = "oni_";
		}
		else if (_type == mika) {
			name = "mika_";
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				oniVec.push_back("img/ao oni_action/" + oniVec.at(0) +
					to_string(i) + to_string(j) + ".bmp");
			}
		}
		bitmap.LoadBitmapByString(oniVec, RGB(204, 255, 0));
	}
	void Oni::SetPos(int x, int y) {
		_posX = x;
		_posY = y;
	}
	void Oni::GetPlayerPos(int playerX, int playerY) {
		_humanX = playerX;
		_humanY = playerY;
	}
	void Oni::OnMove() {
		//according humanXY
		//we find the abs((human - pos)X) abs((human - pos)Y)
		//which is bigger than the other
		//we will choose it
		//and move it first
	}
	void Oni::OnShow() {
		//if (!_isDisappear)
		//show...
	}
	void Oni::Countdown() {
		//_time--
		//if _time == 0
		//_isDisappear = true
	}
	void Oni::ChangeMap() {
		//SetPos
		//_time ++
	}
	bool Oni::isCatch(int playerX, int playerY) {
		return _posX == playerX && _posY == playerY;
	}
}