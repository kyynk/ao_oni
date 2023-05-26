#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "DarkRoomEffect.h"
namespace game_framework {
	// Load the bitmap
	void DarkRoomEffect::load(const vector<string> &str, COLORREF color) {
		bitmap.LoadBitmapByString(str,color);
	}
	// Show the bitmap
	void DarkRoomEffect::OnShow() {
		if (_isshow) {
			bitmap.ShowBitmap();
		}
	}
	 // Set the position of the bitmap
	void DarkRoomEffect::SetXY(int x, int y) {
		bitmap.SetTopLeft (x, y);
	}
	 // Set the frame index of the bitmap
	void DarkRoomEffect::SetState(int a) {
		if(a != bright)
			bitmap.SetFrameIndexOfBitmap(a);
		else
			_isshow = false;
	}
}