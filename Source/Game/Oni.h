#pragma once
namespace game_framework {
	class Oni{
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
		Oni();
		~Oni();
		
	private:
		int _posX, _posY, 
			_counter, _time;
		OniType _type;
		CMovingBitmap bitmap;
	};
}