#pragma once
namespace game_framework {
	class Oni : public Entity{
	public:
		enum OniType {
			normal,
			mika,
			takesi,
			takurou,
			flat,
			muddy,
			muscle,
			strange
		};
		enum bstate {
			s1,
			s2,
			s3,
			s4
		};
		enum move {
			none,
			isup,
			isdown,
			isleft,
			isright
		};
		enum changeMap {
			room,
			hallway
		};
		Oni();
		~Oni();
		void SetParam(OniType tp, int step, int moveTime);
		void SetPos(int x, int y);
		int GetPosX();
		int GetPosY();
		int GetPosL();
		int GetPosU();
		int GetPosR();
		int GetPosD();
		void Load(string filename, COLORREF color);
		void GetPlayerPos(int playerX, int playerY);
		void Track(GameMap &map);
		void OnMove(GameMap &map);	// every time oni move, will track first
		void OnShow();
		// every time oni appear, default time is 10 sec
		void ResetOT();
		// 10 -> 0 sec, if is 0 sec, 
		// change stage will make Oni disappear
		void Countdown();
		// if change stage, will add 1 sec
		void ChangeMap();
		void SetWait();
		// if change room wait 1 sec, change hallway 0.5 sec
		void Waiting(changeMap a);
		bool WaitOver();
		bool isCatch();  // collide
		void SetVanish();
		
	private:
		int _humanX, _humanY, 
			_step, _moveTime, _overTime, 
			_offsetX, _offsetY;
		bool _isDisappear;
		bool _walkiter;
		bool _blocked;
		bool _isup;
		bool _isdown;
		bool _isleft;
		bool _isright;
		bool _wait;
		OniType _type;
		bstate _bstate;
		move _nowmove;
		move _tracking;
	};
}