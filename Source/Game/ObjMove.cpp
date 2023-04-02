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
#include "GameMap.h"
#include "Entity.h"
#include "ObjMove.h"

namespace game_framework {
	ObjMove::ObjMove() {
		_humanX, _humanY,
			_step, _moveTime, _overTime,
			_offsetX, _offsetY,
			_resetX, _resetY,
			_fixedX, _fixedY = 0;
		_isFixedPos = false;
		_press = false;
		_collide = false;
		_type = no;
		_nowmove = none;
		_tracking = none;
	}
	ObjMove::~ObjMove() {
	}
	void ObjMove::SetParam(ObjType tp, int step, int moveTime, 
		int offsetX, int offsetY, int resetX, int resetY, 
		int fixedX, int fixedY) {
		_type = tp;
		_step = step;
		_moveTime = moveTime;
		_offsetX = offsetX;
		_offsetY = offsetY;
		_resetX = resetX;
		_resetY = resetY;
		_fixedX = fixedX;
		_fixedY = fixedY;
	}
	void ObjMove::Load(vector<string> filename, COLORREF color) {
		bitmap.LoadBitmapByString(filename, color);
	}
	int ObjMove::GetPosX() {
		return _pos_x;
	}
	int ObjMove::GetPosY() {
		return _pos_y;
	}
	int ObjMove::GetPosL() {
		return _pos_x - TILE;
	}
	int ObjMove::GetPosU() {
		return _pos_y - TILE;
	}
	int ObjMove::GetPosR() {
		return _pos_x + _offsetX + TILE;
	}
	int ObjMove::GetPosD() {
		return _pos_y + _offsetY + TILE;
	}
	void ObjMove::GetPlayerPos(int playerX, int playerY) {
		_humanX = playerX;
		_humanY = playerY;
	}
	void ObjMove::Track(GameMap &map) {

	}
	void ObjMove::OnMove(GameMap &map) {	// every time obj move, will track first
		
	}
	void ObjMove::OnKeyDown(UINT nChar) {
		if (nChar == VK_SPACE)
			_press = true;
	}
	void ObjMove::OnKeyUp(UINT nChar) {
		if (nChar == VK_SPACE)
			_press = false;
	}
	void ObjMove::OnShow() {

	}
	void ObjMove::Reset() {
		SetXY(_resetX, _resetY);
	}
	void ObjMove::Fixed() {
		if (_pos_x == _fixedX && _pos_y == _fixedY)
			_isFixedPos = true;
		if (_isFixedPos) {
			_resetX = _fixedX;
			_resetY = _fixedY;
		}
	}
	bool ObjMove::isCollide() {
		int x = _pos_x + _offsetX;
		int y = _pos_y + _offsetY;
		if ((_humanX + 32 == _pos_x && (_humanY == _pos_y || _humanY == y))
			|| (_humanX - 32 == x && (_humanY == _pos_y || _humanY == y))
			|| ((_humanX == _pos_x || _humanX == x) && _humanY + 32 == _pos_y)
			|| ((_humanX == _pos_x || _humanX == x) && _humanY - 32 == y))
			_collide = true;
		else
			_collide = false;

		return _collide;
	}
}