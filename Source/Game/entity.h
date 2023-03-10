#pragma once

namespace game_framework {

	class Entity {
	public:
		Entity();
		//getter
		int getX1();
		int getX2();
		int getY1();
		int getY2();
		void SetXY(int, int);
		void SelectShowBitmap(int index);

		void TimerUpdate();
		void TimerReset();
		int TimerGetCount();
		void SetTimer(bool b); // set true to start timer , false to stop timer
		bool IsTimerStart(); 
		// every entity must implement these three function 
		virtual void OnMove();
		virtual void OnShow();
		virtual void Load();

		CMovingBitmap bitmap;
		int pos_x, pos_y;

	private:
		int _counter;
		bool _istimerstart;
	};
}