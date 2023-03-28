#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include "Entity.h"
#include "Item.h"
namespace game_framework {

	Item::Item() {

	}
	Item::~Item() {

	}
	void Item::init(bool willMove, bool isonce, 
		bool triggered, bool blocking, itemtype type, 
		int anidelay = 100, int step=0) {
		_isMoving = willMove;
		_isBlock = blocking;
		_isonce = isonce;
		_anidelay = anidelay;
		_triggered = triggered;
		_type = type;
		_step = step;
	}
	//OnKeyDown -> game_run
	//void Item::OnKeyDown(UINT nChar, int playerX, int playerY) {
	//	if (collide(playerX, playerY)) {
	//		if (nChar == VK_SPACE) {
	//			/* will have animation */
	//		}
	//	}
	//}
	
	void Item::OnMove() {
		//according player direction to decide 
		//where the Item want to go
		if (_isMoving) {
		}
	}
	void Item::OnShow() {// show animation
		if (_triggered) {
			if (_type == once) {
				bitmap.SetAnimation(_anidelay, _isonce);
				bitmap.ToggleAnimation();
			}
			else if (_type == multi) {
				bitmap.SetAnimation(_anidelay,false);
			}
			bitmap.ShowBitmap();
		}
	}
	void Item::SetTriggered(bool a) {
		_triggered = a;
	}
	void Item::Load(vector <string> file,COLORREF color)
	{
		bitmap.LoadBitmapByString(file, color);
	}
	void Item::GetPlayerPos(int x, int y) {
		// we need to change x, y to grid (x, y)
		/*
		to do
		*/
		_playerX = x;
		_playerY = y;
	}
	bool Item::collide() {
		//         player(4)
		//player(2) item player(1)
		//         player(3)

		return //full grid collide
			pos_x + 32 == _playerX && pos_y == _playerY ||
			pos_x - 32 == _playerX && pos_y == _playerY ||
			pos_x == _playerX && pos_y + 32 == _playerY ||
			pos_x == _playerX && pos_y - 32 == _playerY;
	}
}