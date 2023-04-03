#pragma once

namespace game_framework {

	class Entity {
	public:
		Entity();
		//getter
		
		void SetXY(int, int);
		void SelectShowBitmap(int index);
		bool CheckInVec(int n,vector<int> vec);

		void TimerUpdate();
		void TimerReset();
		void TimerStop();
		int TimerGetCount();
		void TimerStart();
		bool IsTimerStart(); 
		//bool UBlocked();
		//bool DBlocked();
		//bool LBlocked();
		//bool RBlocked();
		// every entity must implement these three function 
		/*virtual void OnMove();
		virtual void OnShow();
		virtual void Load();*/

		CMovingBitmap bitmap;
	protected:
		int _pos_x, _pos_y;
		int _uy;
		int _dy;
		int _lx;
		int _rx;

	private:
		int _counter;
		bool _istimerstart;
	};
}