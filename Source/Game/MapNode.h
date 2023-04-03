#pragma once
namespace game_framework {
	class NodeData {
		friend class MapNode;
	public:
		NodeData() {
			w = -1;
			x = -1;
			y = -1;
			z = -1;
		}
		NodeData(int ww, int xx, int yy, int zz):w(ww),x(xx),y(yy),z(zz){
		}
		NodeData(int ww, int xx) :w(ww), x(xx), y(-1), z(-1) {
		}
		~NodeData(){}
		NodeData(const NodeData& other) {
			w = other.w;
			x = other.x;
			y = other.y;
			z = other.z;
		}
		NodeData& operator=(const NodeData& other) {
			if (this != &other) {
				this->w = other.w;
				this->x = other.x;
				this->y = other.y;
				this->z = other.z;
			}
			return *this;
		}
		NodeData& operator=(const NodeData&& other) {
			if (this != &other) {
				this->w = other.w;
				this->x = other.x;
				this->y = other.y;
				this->z = other.z;
			}
			return *this;
		}
		bool operator==(const NodeData&& other) {
			if (this->w == other.w&&
				this->x == other.x)
			{
				return true;
			}
			else {
				return false;
			}

		}
		bool IsEmpty() { return (w == -1 && x == -1 && y == -1 && z == -1) ? true : false; }
		int GetW() { return w; }
		int GetX() { return x; }
		int GetY() { return y; }
		int GetZ() { return z; }
	private:
		int w, x, y, z;
	};
	class MapNode {
		
	public:
		MapNode(){
			_ID = -1;
			_index = 0;
		}
		MapNode(NodeData &&points, int id):_ID(id) {
			_index = 0;
			_points[_index].w = points.w;
			_points[_index].x = points.x;
			_points[_index].y = points.y;
			_points[_index].z = points.z;
			_index ++;
		}
		~MapNode(){}
		void AddEdge(NodeData &&nd) {
			_points[_index].w = nd.w;
			_points[_index].x = nd.x;
			_points[_index].y = nd.y;
			_points[_index].z = nd.z;
			if(_index <5)_index++;
			//TRACE("In edge : %d %d %d %d\n",_points->w,_points->x,_points->y,_points->z);
		}
		int GetSize() const {
			return _index;
		}
		int GetID() {
			return _ID; 
		}
		void debug();
		NodeData GetPointByIndex(int index) {
			return _points[index];
		}
	private:
		int _index;
		int _ID;
		NodeData _points[5];
	};

}
