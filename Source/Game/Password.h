#pragma once
namespace game_framework {
	class Password {
	public:
		Password();
		~Password();
		void SetParam(string pwd);
		void ShowBox();
		void ShowCursor();
		void ShowText(CDC* pDC);
		void ShowTotal();
		void SetShow(bool show);
		void OnKeyDown(UINT nChar);
		bool IsShow();
		bool IsCorrect();
		bool IsOpen();
	private:
		int _cursorX, _cursorY, _boxX, _boxY,
			_txtX, _txtY,
			_lineSpacing, 
			_nowChoose;
		vector<int> _guess;
		bool _isShow;
		bool _open;
		string _pwd; // correct pwd
		string _store; // player guess
		CMovingBitmap _cursor;
		CMovingBitmap _box;
	};
}