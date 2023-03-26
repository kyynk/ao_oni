#pragma once
namespace game_framework {
	class NodeData {
	public:
		NodeData(int ww,int xx,int yy,int zz):w(ww),x(xx),y(yy),z(zz){}

		int w, x, y, z;

	};
	class MapNode {
	public:
		MapNode(){}
		MapNode(vector<NodeData> points,int id):_points(points),_ID(id) {}
		void AddEdge(NodeData nd) {
			_points.push_back(nd);
		}
		int GetID() { return _ID; }
		void debug();
		~MapNode(){}
		
	private:
		int _ID;
		vector<NodeData> _points;
	};

}
