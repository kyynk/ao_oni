#pragma once
namespace game_framework {
	class Conment {
	public:
		enum character {
			none,
			hirosi,
			mika,
			takesi,
			takurou
		};
		Conment();
		~Conment();
		void SetNow(character a);
		void SetParam(int _posX, int _posY, 
			int linespacing, vector<string>  st);
		void ShowBox();
		void ShowHead();
		void ShowCursor();
		void ShowText(CDC* pDC);
	private:
		int _cursorX, _cursorY, _boxX, _boxY,
			_txtX, _txtY, _headX, _headY, _lineSpacing;
		character _now;
		CMovingBitmap _cursor;
		CMovingBitmap _box;
		CMovingBitmap _head;
		vector<string> _store;
	};
}