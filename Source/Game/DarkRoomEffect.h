#pragma once

namespace game_framework {

	class DarkRoomEffect {
	public:
		enum lightstate {
			dim,
			dark,
			bright
		};
		DarkRoomEffect() = default;
		void load(const vector<string> &str,COLORREF color);
		void OnShow();
		void SetXY(int x,int y);
		void SetShow(bool isshow) { _isshow = isshow; }
		bool IsShow() { return _isshow; }
		void SetState(int a);
	private:
		CMovingBitmap bitmap;
		bool _isshow;
	};
}