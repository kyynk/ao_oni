#pragma once

namespace game_framework {

	class Entity {
	public:
		Entity();
		enum bstate {
			s1,
			s2,
			s3,
			s4
		};
		enum Direction {
			none,
			up,
			down,
			left,
			right,
			machinetransmap
		};
		enum BitmapMove{
			BITMAP_DOWN = 0,
			BITMAP_DOWN_1 = 1,
			BITMAP_DOWN_2 = 2,
			BITMAP_UP = 9,
			BITMAP_UP_1 = 10,
			BITMAP_UP_2 = 11,
			BITMAP_LEFT = 3,
			BITMAP_LEFT_1 = 4,
			BITMAP_LEFT_2 = 5,
			BITMAP_RIGHT = 6,
			BITMAP_RIGHT_1 = 7,
			BITMAP_RIGHT_2 = 8
		};
		
		void SetXY(int x, int y);
		void SelectShowBitmap(int index);
		void TimerUpdate() {
			if (_istimerstart){
				if (getClockInterval(_timer)>=0.001) {
					_counter++;
					_timer = clock();
				}
			}
		}
		void TimerReset() {
			_counter = 0;
		}
		void TimerStop() {
			_counter = 0;
			_istimerstart = false;
		}
		int TimerGetCount() {
			return _counter;
		}
		void TimerStart() {
			_istimerstart = true;
		}
		bool IsTimerStart() {
			return _istimerstart;
		}
		CMovingBitmap bitmap;
	protected:
		double getClockInterval(clock_t start) {
			return (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
		}
		clock_t _timer = clock();
		bool _clockstart;
		int _pos_x, _pos_y;
		/*int _uy;
		int _dy;
		int _lx;
		int _rx;*/
		int _counter;
		bool _istimerstart;

		
	};
}