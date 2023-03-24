#pragma once
namespace game_framework {
	
	class MapNode {
	public:
		MapNode(){}
		MapNode(int a,int b,int c,int d,int id):x1(a),y1(b),x2(c),y2(d),_ID(id) {}
		int GetID() { return _ID; }
		int GetX1() { return x1; }
		int GetY1() { return y1; }
		int GetX2() { return x2; }
		int GetY2() { return y2; }

		~MapNode(){}
		
	private:
		int _ID;
		int x1, y1, x2, y2;
	};

}
