#pragma once

namespace game_framework{
	class Human : public Entity {
	public:
		enum Direction {
			up,
			down, 
			left,
			right
		};
		enum bstate {
			s1,
			s2,
			s3,
			s4
		};
		enum state {
			still,
			movingup,
			movingdown,
			movingleft,
			movingright
		};
		Human(int x, int y);
		
		
		void OnMove(bool pressed);
		void OnShow();
		void Load(vector<string> filenames,COLORREF color );


		void SetDirection(Direction d) {
			_direction = d;
		}
		int GetDirection() {
			return _direction;
		}
	private:
		bool _walkiter;
		bool _blocked;
		Direction _direction;
		bstate _bstate;
		state _state;
		int step;


	};
}