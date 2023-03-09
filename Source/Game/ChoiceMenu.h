#pragma once

namespace game_framework {
	class ChoiceMenu {
	public:
		ChoiceMenu();
		void ShowText(CDC * pDC, CFont *& fp);
		int GetSelection() {
			return _selection;
		}
		void SetParam(int tx, int ty, int bx, int by,int curx,int cury,int linespacing,vector<string> ch) {
			_bitmapX = bx;
			_bitmapY = by;
			_textX = tx;
			_textY = ty;
			_lineSpacing = linespacing;
			_choices = ch;
			_cursorX = curx;
			_cursorY = cury;
		}
		void ShowBitmap();
		void ShowCursor();
		void OnMovingCursor(UINT nChar);
		void Load(vector<string> cfn, vector<string> fn, COLORREF color1, COLORREF color2);
	private:
		vector<string> _choices;
		int _selection;
		CMovingBitmap _bitmap, _cursor;
		int _bitmapX, _bitmapY, _textX, _textY, _lineSpacing;
		int _cursorX, _cursorY;
	};

}