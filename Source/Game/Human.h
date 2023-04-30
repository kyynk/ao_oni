#pragma once
namespace game_framework {
	class Human : public Entity {
	public:
		Human();
		void SetPos(int x, int y);
		void init(int step, int offset, Direction dir);
		void OnMove();
		void SetDirection(Direction d) {
			_direction = d;
		}
		int GetDirection() const { return _direction; }
		void Load(vector<string> filenames, COLORREF color);
		void OnShow();
	protected:
		Direction _direction;
		int _machine_count;
		bool _machine_done;
		int _offsetY;
		bool _walkiter;
		int _step;
		bstate _bstate;
		Direction _nowmove;

	};
}