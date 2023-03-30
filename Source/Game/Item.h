#pragma once
namespace game_framework {
	class Item : public Entity {
	public:
		enum itemtype {
			select,
			once,
			repeat
		};
		enum move {
			none,
			isup,
			isdown,
			isleft,
			isright
		};
		Item();
		~Item();
		void SetParam(int step, int delay, itemtype type, 
			int boxX, int boxY);
		void Load(vector<string> filename, COLORREF color);
		void GetPlayerPos(int x, int y);
		void CheckMoveDirection();
		void OnMove(UINT nChar);
		void OnShow();
		bool Collide();
		void Animation(int n);
		void SetTrigger();
	
	private:
		int _step, _anidelay, 
			_playerX, _playerY, 
			_boxX, _boxY;
		itemtype _type;
		move _move;
;		bool _triggered, _close;
	};
}