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
	}
	ObjMove::~ObjMove() {
	}
	void ObjMove::SetParam(ObjType tp, int step, int moveTime, 
		int offsetX, int offsetY) {
		_type = tp;
		_step = step;
		_moveTime = moveTime;
		_offsetX = offsetX;
		_offsetY = offsetY;
	}
	void ObjMove::Load(vector<string> filename, COLORREF color) {
		bitmap.LoadBitmapByString(filename, color);
	}
	void ObjMove::SetPos(int x, int y) {
		SetXY(x, y);
	}
	int ObjMove::GetPosX() {
		return _pos_x;
	}
	int ObjMove::GetPosY() {
		return _pos_y;
	}
	int ObjMove::GetPosL() {
		return _pos_x;
	}
	int ObjMove::GetPosU() {
		return _pos_y;
	}
	int ObjMove::GetPosR() {
		return _pos_x + _offsetX;
	}
	int ObjMove::GetPosD() {
		return _pos_y + _offsetY;
	}
	void ObjMove::GetPlayerPos(int playerX, int playerY) {
		_humanX = playerX;
		_humanY = playerY;
	}
	void ObjMove::Track(GameMap &map) {

	}
	void ObjMove::OnMove(GameMap &map) {	// every time obj move, will track first
		
	}
	void ObjMove::OnShow() {

	}
	void ObjMove::Reset() {

	}
	void ObjMove::Fixed() {

	}
}