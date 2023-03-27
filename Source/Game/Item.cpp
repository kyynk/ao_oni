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

	Item::Item()
	{
	}
	void Item::init(bool willMove, bool isonce, bool triggered, bool blocking, itemtype type, int anidelay = 100) {
		_isMoving = willMove;
		_isBlock = blocking;
		_isonce = isonce;
		_anidelay = anidelay;
		_triggered = triggered;
		_type = type;	
		if (_type == once) {
			bitmap.SetAnimation(_anidelay, isonce);
			bitmap.ToggleAnimation();
		}
		else if (_type == multi) {

		}


	}

	void Item::OnMove()
	{
		if (_triggered) {
			if (_type == once) {

			}
			else if (_type == multi) {

			}
		}
	
	}

	void Item::OnShow()
	{
		if (_triggered) {
			if (_type == once) {

				
			}
			else if (_type == multi) {
				bitmap.SetAnimation(_anidelay,false);
			}
			bitmap.ShowBitmap();
		}
	}
	void Item::Load(vector <string> file,COLORREF color)
	{
		bitmap.LoadBitmapByString(file, color);
	}
	bool Item::collide(int playerX, int playerY) {
		// will complete can stop on the with Item
		return _posX < playerX && playerX < _posX + _w
			&& _posY < playerY && playerY < _posY + _h;
	}
}