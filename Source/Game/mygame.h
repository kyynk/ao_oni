/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
*/
#include <fstream>
#include <bitset>
#include <mmsystem.h>
#include <string>
#include <array>
#include "config.h"
#include "Entity.h"
#include "Human.h"
#include "GameMap.h"
#include "MainHuman.h"
#include "Oni.h"
#include "MapNode.h"
#include "MapRouter.h"
#include "ChoiceMenu.h"
#include "Item.h"
#include "ObjMove.h"
#include "Password.h"
#include "Dialog.h"
#include "InputBox.h"
#include "MainHuman.h"
#include "Event.h"
#include "MapRes.h"
#include "InterfaceData.h"
#include "Interface.h"
namespace game_framework {
	
	enum AUDIO_ID {
		AUDIO_DING,			
		AUDIO_LAKE,			
		AUDIO_NTUT			
	};


	class CGameStateInit : public CGameState {
	enum substate {
		startmenustate,
		animationstate,
		nameingstate,
		showstorydialogstate,
		loaddatamenustate,
		jumpstate
	};
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								
		void OnBeginState();							
		void OnKeyUp(UINT, UINT, UINT); 				
		void OnKeyDown(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  
	protected:
		void OnShow();									
	private:
		ChoiceMenu startmenu;
		CMovingBitmap start_animation;
		Dialog story;
		int _substate;
	};


	class CGameStateRun : public CGameState {
	enum substate {
		OnChasing,
		OnDialogs,
		OnWalking,
		OnInputBox
	};
	enum pwd_pos {
		piano,
		basement
	};
	enum obj_move {
		house1_2F_TR_chair,
		house1_2F_TL_chair,
		house1_basement2_chair
	};
	enum {
		BROKEN_DISH_E,
		START_EVENT_E,
		START_EVENT2_E,
		KEY_LIB_E,
		DETERGENT_E,
		PHILLIPS_E,
		KEY_3F_L_E,
		LIGHTER_E,
		DUMB_TAKESHI_E,
		HANDKERCHIEF_E,
		MIKA_SCARE_E,
		MIKA_OK_E,
		MIKA_NOTOK_E,
		MIKA_REPEAT_E,
		LIB_KEY_CHASE,
		DUMB_TAKESHI_REPEAT_E,
		KEY_2F_TL_E,
		KEY_BASEMENT_E,
		KEY_ANNEXE_E,
		DOOR_LOCKED_E,
		DOOR_UNLOCKED_E
	};
	enum item_name {
		TOILET,
		TUB_ONCE,
		PHILLIPS,
		TUB_FIXED,
		BROKEN_DISH,
		LIB_BOOK,
		KEY_3F_L,
		KEY_LIB,
		DOOR_KNOB,
		DOOR_NO_KNOB,
		LIGHTER,
		TATAMI_L,
		TATAMI_R,
		DETERGENT,
		KEY_BASEMENT,
		GATE,
		GATE2,
		BED,
		BOOKCASE_L,
		BOOKCASE_R,
		WHITE_BOOKCASE,
		BOOKCASE_MAP21,
		HANDKERCHIEF,
		CLOSET_SHAKE,
		CLOSET_TAKESI_0,
		CLOSET_TAKESI_1,
		CLOSET_HIROSI_R,
		CLOSET_HIROSI_L,
		CLOSET_MIKA_OUT,
		KEY_JAIL,
		FLATHEAD,
		OIL,
		DOOR_ONI,
		DOOR_OPEN,
		DOOR_DIE,
		DOOR_HALF,
		PIANO_PWD_NOTOPEN,
		PIANO_PWD_TAKE,
		PIANO_KEY,
		BASEMENT_PWD,
		BASEMENT_PWD_TAKE,
		BASEMENT_KEY,
	};
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							
		void OnInit();  								
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  
		void OnMouseMove(UINT nFlags, CPoint point);	
		void OnRButtonDown(UINT nFlags, CPoint point);  
	protected:
		void OnMove();
		void OnShow();								
	private:
		void map0func1(int chairy, int humany, int oniy);
		void ShowOniAndPlayer();
		void SetEventTriggeredDialog(int eventid);
		void DeBugRecursive();
		//game
		bool boolpspace;
		int _substate;
		int _playerStep;
		bool _pwd;
		vector<Dialog> dialogs;
		MainHuman player;
		Human human_mika;
		Human human_takeshi;
		Human human_takuro;
		Interface game_interface;
		Oni normal_oni;
		vector<ObjMove> objs;
		vector<Event> events;
		vector<Item> items;
		vector<Password> pwds;
		// sth in gamerun
		bool _map_show;
		bool _blue_paint_show;
		CMovingBitmap house1_map;
		CMovingBitmap blue_paint;
		//map related
		CMovingBitmap mapmask;
		vector<GameMap> gamemaps;
		int _nowID;
		int _dialogID;
		int _dialogcount;
		int _eventID;
		vector<vector<int>> blockLeftCor; //x y nowID
		vector<vector<int>> blockRightCor; //x y nowID
		vector<vector<int>> blockTeleportCor; //x y nowID
		vector<int> mapoverlayindex;
		MapRouter router;
		bool gameonce;
		//dev related
		int mousex, mousey;
		int mousex_foc,mousey_foc;
		std::ofstream ofs;
		CMovingBitmap grid;
		CMovingBitmap tileplaceholder;
		InputBox inputbox;
		bool isteleportblock;
		bool isgrid;
		bool isedit;
		int istwoway;
		bool isdebugmode;
		vector<int> twowayvec;
		vector<int> pointvec;
		std::vector<int> values;
		std::vector<Entity*> entities;
		ObjMove* obj;
	};
	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							
		void OnInit();
		void OnKeyDown(UINT, UINT, UINT);

	protected:
		void OnMove();									
		void OnShow();									
	private:
		int counter;	
		CMovingBitmap GameOver;
	};

}
