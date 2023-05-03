#pragma once
namespace game_framework {
	class Human : public Entity {
	public:
		Human();
		void SetPos(int x, int y);
		void init(int step, int offset, Direction dir);
		void StorePlayerPos(int x, int y);
		void OnMove();
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void SetDirection(Direction d) { _direction = d;}
		int GetDirection() const { return _direction; }
		void Load(vector<string> &filenames, COLORREF color);
		void OnShow();
	protected:
		Direction _direction;
		Direction _nowmove;
		bool _machine_done;
		bool _walkiter;
		int _machine_count;
		int _offsetY;
		int _step;
		bstate _bstate;
	private:
		bool _trigger;
		int _playerX;
		int _playerY;
	};
}