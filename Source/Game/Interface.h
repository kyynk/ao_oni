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
		bool isPause() const; // check pause
		bool isStop() const;
		void StartCount();
		void PauseCount();
		void StopCount();
		long getStartTime();
		string getGameTime();
		// game time counter
		string GetRealTime();
		void PlayerStep();
		void ShowCursor();
		void ShowText(CDC* pDC);
		void StoreItem(string item_name);
		
	private:
		int _cursorX, _cursorY;
		CMovingBitmap _cursor;
		vector<string> items;
		long start_time, pause_time;
		bool _isPause;
		bool _isStop;
	};
}