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
		Oni();
		~Oni();
		void SetParam(OniType tp);
		void SetPos(int x, int y);
		void GetPlayerPos(int playerX, int playerY);
		void OnMove();	// will do track in this
		void OnShow();
		// 10 -> 0 sec, if is 0 sec, 
		// change stage will make Oni disappear
		void Countdown();
		// if change stage, will add 1 sec
		void ChangeMap();
		bool isCatch(int playerX, int playerY);  // collide
		
	private:
		int _posX, _posY, _time, 
			_humanX, _humanY;
		bool _isDisappear;
		OniType _type;
		CMovingBitmap bitmap;
	};
}