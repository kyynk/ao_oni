#pragma once
namespace game_framework {
	class Interface {
	public:
		enum status {
			esc, 
			item, 
			save, 
			end
		};
		Interface();
		~Interface();
		void init();
		// game time counter
		bool IsPause() const; // check pause
		bool IsStop() const;
		void StartCount();
		void PauseCount();
		void StopCount();
		time_t GetStartTime();
		string GetGameTime();
		// game time counter
		string GetRealTime();
		void StorePlayerStep(int step);
		void StoreItem(string item_name);
		void ShowCursorInit();
		void ShowCursorItem();
		void ShowCursorSave();
		void ShowCursorEnd();
		void ShowTextInit(CDC* pDC);
		void ShowTextItem(CDC* pDC);
		void ShowTextSave(CDC* pDC);
		void ShowTextEnd(CDC* pDC);
		void ShowTotal();
		void OnKeyDown(UINT nChar);
	private:
		int _cursorX, _cursorY,
			_boxX, _boxY, _txtX, _txtY,
			_lineSpacing, _step, 
			_initChoose, _itemChoose, 
			_saveChoose, _endChoose;
		vector<string> items;
		time_t start_time, pause_time;
		bool _isPause;
		bool _isStop;
		CMovingBitmap _cursor;
		CMovingBitmap _init;
		CMovingBitmap _item;
		CMovingBitmap _save;
		CMovingBitmap _end;
	};
}