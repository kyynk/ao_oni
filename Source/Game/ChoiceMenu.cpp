#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "ChoiceMenu.h"
namespace game_framework {
	ChoiceMenu::ChoiceMenu()	{

	}
	void ChoiceMenu::ShowMenu(int x,int y)	{
		_bitmap.SetTopLeft(x, y);
		_bitmap.SetFrameIndexOfBitmap(_focus);
		_bitmap.ShowBitmap();
	}
	void ChoiceMenu::OnMovingCursor(UINT nChar)	{
		while (_focus >= 0 && _focus < (int)_choices.size()) {
			if (nChar ==VK_UP ) {
				_focus -= 1;
			}
			else if (nChar == VK_DOWN) {
				_focus += 1;
			}
			else {
				;
			}
		}
	}
	void ChoiceMenu::Load(vector<string> fn,COLORREF color){
		_bitmap.LoadBitmapByString(fn, color);
	}
}