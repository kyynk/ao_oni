#pragma once
namespace game_framework {
	class Dialog {
	public:
		enum character {
			none,
			hirosi,
			mika,
			takesi,
			takurou
		};
		Dialog();
		~Dialog();
		void SetNow(character a);
		void SetParam(vector<string>  st);
		void ShowBox();
		void ShowHead();
		void ShowCursor();
		void ShowText(CDC *pDC);
		void ShowNameBox();
		void ShowTotal();
		bool isClose();
		void Show();
		void Close();
	private:
		int _cursorX, _cursorY, _boxX, _boxY,
			_txtX, _txtY, _headX, _headY, 
			_nameX, _nameY, _nBoxX, _nBoxY, 
			_lineSpacing;
		bool _isClose;
		bool _isChoice;
		character _now;
		CMovingBitmap _cursor;
		CMovingBitmap _box;
		CMovingBitmap _head;
		CMovingBitmap _nameBox;
		vector<string> _name;
		vector<string> _store;
	};
}