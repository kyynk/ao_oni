#pragma once

namespace game_framework {

	class Entity {
	public:
		Entity();
		Entity(int x, int y);
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
		// every entity must implement these three function 
		virtual void OnMove();
		virtual void OnShow();
		virtual void Load();

		CMovingBitmap bitmap;
		int pos_x, pos_y;

	private:
		int _counter;
	};
}