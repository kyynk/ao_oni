#pragma once

namespace game_framework {

	class Event {
	public:
		Event();
		void debug ();
		void SetCondition(bool con);
		void SetParam(vector<vector<int> > blockpath, int firstindex,int count);
		bool IsTriggered();
		void SetTriggered(bool triggered);
		vector<vector<int> >& GetBlockPath();
		int GetDialogIndex();
		int GetDialogCount();
		bool &IsTransMap() { return _transmap; }

	private:
		bool _condition;
		bool _triggered;
		vector<vector<int> > _blockpath; // 0 = map0, 1 = map1
		int _dialogindex;
		int _dialogcount;
		bool _transmap;
	};
}