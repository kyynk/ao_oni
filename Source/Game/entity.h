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
		enum direction {
			none,
			up,
			down,
			left,
			right
		};
		void SetXY(int x, int y);
		void SelectShowBitmap(int index);
		void TimerUpdate(clock_t start) {
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
		void SetClock(clock_t timer) { _timer = timer; }
		clock_t GetClock() { return _timer; }
		double getClockInterval(clock_t start) {
			return (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
		}
		clock_t _timer = clock();
		bool _clockstart;
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