#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include <cctype>
#include "entity.h"
#include "InputBox.h"
namespace game_framework {
	void InputBox::init(int x,int y, int cursor,int margin)
	{
		_pos_x = x;
		_pos_y = y;
		_iswrite = false;
		_margin = margin;
		_toggleplaceholder = true;
		_box.SetTopLeft(_pos_x + _margin, _pos_y + _margin);
		
	}
	void InputBox::BoxOn(UINT c) {
		if (_iswrite) {
			
			if (isalnum(c)) {
				if (int(_buffer.size()) < buffersize - 1) {  // Make sure there is space in the buffer
					_buffer.push_back(c);
				}
			}
			else if (c == VK_BACK) {
				if (int(_buffer.size()) > 0) {
					_buffer.pop_back();
				}
			}
			else if (c == VK_SPACE) {
				_iswrite = false;
				_placeholder = "\0";
				TimerReset();
			}
		}
	}
	void InputBox::OnMove() {
		if (_iswrite) {
			if (TimerGetCount() == 25) {
				TimerReset();
				if (_toggleplaceholder) {
					_placeholder = "_";
				}
				else {
					_placeholder = "\0";
				}
				_toggleplaceholder = !_toggleplaceholder;
			}
			TimerUpdate();
		}
	}
	void InputBox::ClearBuffer() {
		_buffer.clear();
	}
	void InputBox::Load(string box,COLORREF color1)
	{
		_box.LoadBitmapByString({ box }, color1);
	}
	void InputBox::Show()
	{	
		_box.ShowBitmap();
		CDC *pDC = CDDraw::GetBackCDC();

		CTextDraw::Print(pDC, _pos_x + _margin*2, _pos_y + _margin*2, string(_buffer.begin(), _buffer.end())+_placeholder);
		CDDraw::ReleaseBackCDC();
	}
	
}