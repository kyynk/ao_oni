#pragma once
namespace game_framework {
	class Item : public Entity {
	public:
		// need to 
		// check the type will be once -> multi or not
		enum itemtype {
			once,
			oncedone,
			multi,
			multidone
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
		void init(bool willMove, bool isonce, bool triggered,
			bool blocking, itemtype type, int anidelay, 
			int step);
		//void OnKeyDown(UINT nChar, int playerX, int playerY);
		void OnMove();
		void OnShow();
		void SetTriggered(bool a);
		void Load(vector<string> file, COLORREF color);
		void GetPlayerPos(int x, int y);
		bool collide();
	
	private:
		int _step, _playerX, _playerY, 
			_w, _h;  // Width Height
		itemtype _type;
;		bool _triggered;
		bool _isonce;
		bool _isMoving;
		bool _isBlock;
		int _anidelay;
	};

}