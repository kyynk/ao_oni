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
		
		enum move {
			none,
			isup,
			isdown,
			isleft,
			isright
		};
		Human();

		void init(int step,int offset);
		
		void OnMove();
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void OnShow();
		void Load(vector<string> filenames,COLORREF color );

		void SetDirection(Direction d) {
			_direction = d;
		}
		int GetDirection() {
			return _direction;
		}
		int GetPremove(){
			return _premove;
		}

	private:
		int _coroffset;
		bool _pressed;
		bool _walkiter;
		bool _blocked;
		Direction _direction;
		bstate _bstate;
		move _premove;
		move _nowmove;
		move _pressing;
		int _step;
		bool _isup;
		bool _isdown;
		bool _isleft;
		bool _isright;

	};
}