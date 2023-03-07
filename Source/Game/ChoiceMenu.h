#pragma once

namespace game_framework {
	class ChoiceMenu {
	public:
		ChoiceMenu();
		void SetChoices(vector<string> ch) {
			_choices = ch;
		}
		int GetFocus() {
			return _focus;
		}
		void SetFocus(int f = 0) {
			_focus = f;
		}
		void ShowMenu(int x,int y);
		void OnMovingCursor(UINT nChar);
		void Load(vector<string> fn, COLORREF color);
	private:
		vector<string> _choices;
		int _focus;
		CMovingBitmap _bitmap;

	};

}