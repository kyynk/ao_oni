#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "ChoiceMenu.h"
namespace game_framework {
	ChoiceMenu::ChoiceMenu()	{
		_selection = 0;
	}
	void ChoiceMenu::ShowText(CDC *pDC)	{
		for (int i = 0; i < int(_choices.size()); i++) {
			CTextDraw::Print(pDC, _textX,_textY + i*_lineSpacing,_choices.at(i));
		}
	}
	
	void ChoiceMenu::ShowBitmap(){
		//TRACE("%d\n", _focus);
		_bitmap.ShowBitmap();
	}
	void ChoiceMenu::ShowCursor()
	{
		_cursor.SetTopLeft(_cursorX, _cursorY + _selection * _lineSpacing);
		_cursor.SetAnimation(200,false);
		_cursor.ShowBitmap();
	}
	void ChoiceMenu::OnMovingCursor(UINT nChar)	{
	
		if (nChar == VK_UP ) {
			_selection -= 1;
			if (_selection == -1)_selection = (int)_choices.size() - 1;
		}
		else if (nChar == VK_DOWN ) {
			_selection += 1;
			if (_selection ==(int) _choices.size() )_selection = 0;
		}
			
	}
	
	void ChoiceMenu::Load(vector<string> cfn, vector<string> fn,COLORREF color1, COLORREF color2){
		_cursor.LoadBitmapByString(cfn, color1);
		_bitmap.LoadBitmapByString(fn, color2);
	}
}