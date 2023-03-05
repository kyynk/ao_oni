#pragma once
namespace game_framework {

	class Entity {
	public:
		enum Direction {
			up,
			down,
			left,
			right
		};
		enum BitmapState { //to specify animation
			s1,
			s2,
			s3,
			s4
		};
		enum FSM {
			still,
			movingup,
			movingdown,
			movingleft,
			movingright
		};
		Entity();
		//getter
		int getX1();
		int getX2();
		int getY1();
		int getY2();
		int getDirection();
		int getState();
		void SetDirection(Direction d);
		void SetState(FSM state);
		void OnMove();
		void OnShow();
		void PressKeyUp(bool flag);
		void PressKeyDown(bool flag);
		void PressKeyLeft(bool flag);
		void PressKeyRight(bool flag);
		void SetXY(int, int);
		void SelectShowBitmap(int index);
		void LoadBitmap();
		void TimerUpdate();
		void TimerReset();
		int TimerGetCount();
	private:
		CMovingBitmap _bitmap;
		Direction _direction;
		FSM _state;
		BitmapState _bstate;
		int _x, _y;
		bool _walkiter;
		int _counter;
		const int step = 4;
		bool _blocked;
		bool flag;
	};
}