#pragma once
namespace game_framework {
	class Dialog {
	public:
		enum option {
			undefined,
			yes,
			no
		};
		Dialog();
		~Dialog();
		void SetFigure(string &&name);
		void SetParam(vector<string>  st, bool ch);
		void SetOption(string str1, string str2);
		void ShowTotal();
		bool isShow();
		void SetShow(bool isshow);
		bool isChoose();
		void GetSelect(UINT nChar);
		int &Choice(){ return _choice;}
		string GetFigureName() const { return _figurename; }
		vector<string> &GetStore() { return _store; }
	private:
		void ShowBox();
		void ShowHead();
		void ShowCursor();
		void ShowText(CDC *pDC);
		void ShowNameBox();
		int _cursorX, _cursorY, _boxX, _boxY,
			_txtX, _txtY, _headX, _headY, 
			_nameX, _nameY, _nBoxX, _nBoxY, 
			_lineSpacing;
		bool _isShow;
		bool _isChoose;
		string _figurename;
		CMovingBitmap _cursor;
		CMovingBitmap _box;
		CMovingBitmap _head;
		CMovingBitmap _nameBox;
		vector<string> _store;
		ChoiceMenu _choicemenu;
		int _choice;
	};
}