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
		_focus = 0;
		_selection = 0;
	}
	void ChoiceMenu::ShowText(CDC *pDC, CFont* &fp)	{
		for (int i = 0; i < int(_choices.size()); i++) {
			CTextDraw::Print(pDC, _textX,_textY + i*_lineSpacing,_choices.at(i).c_str());
		}
	}
	
	void ChoiceMenu::ShowBitmap(){
		TRACE("%d\n", _focus);
		_bitmap.ShowBitmap();
	}
	void ChoiceMenu::ShowCursor()
	{
		_cursor.SetTopLeft(_cursorX, _cursorY + _focus * _lineSpacing);
		_cursor.ShowBitmap();
	}
	void ChoiceMenu::OnMovingCursor(UINT nChar)	{
		TRACE("%x\n", nChar);
	
		if (nChar == VK_UP && _focus != 0) {
			_focus -= 1;
		}
		else if (nChar == VK_DOWN && _focus != (int)_choices.size()-1) {
			_focus += 1;
		}

			
	}
	
	void ChoiceMenu::Load(vector<string> cfn, vector<string> fn,COLORREF color1, COLORREF color2){
		_cursor.LoadBitmapByString(cfn, color1);
		_bitmap.LoadBitmapByString(fn, color2);
	}
}