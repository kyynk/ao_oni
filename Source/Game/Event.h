#pragma once

namespace game_framework {

	class Event {
	public:
		Event();
		void debug ();
		void SetConditionTrue();
		void SetEvents(string evts);
		bool IsTriggered();
		void SetTriggered(bool triggered);
		vector<vector<int> >& GetBlockPath();
	private:
		string _prereq;
		bool _condition;
		bool _triggered;
		vector<vector<int> > blockpath;
	};
}