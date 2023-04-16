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
#include "config.h"
#include "Entity.h"
#include "GameMap.h"
#include "Human.h"
#include "Oni.h"
#include "MapNode.h"
#include "MapRouter.h"
#include "ChoiceMenu.h"
#include "Item.h"
#include "ObjMove.h"
#include "Dialog.h"
#include "InputBox.h"
#include "Human.h"
#include "Event.h"
#include "MapRes.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	
	enum AUDIO_ID {
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
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
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							
		void OnInit();  								
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  
		void OnLButtonUp(UINT nFlags, CPoint point);	
		void OnMouseMove(UINT nFlags, CPoint point);	
		void OnRButtonDown(UINT nFlags, CPoint point);  
		void OnRButtonUp(UINT nFlags, CPoint point);	
	protected:
		void OnMove();
		void OnShow();								
	private:
		
		void DeBugRecursive();
		//game
		int _substate;
		vector<Dialog> dialogs;
		Human player;
		Human human_mika;
		Human human_takeshi;
		Human human_Takuruo;
		Oni oni1;
		ObjMove house1_2F_TR_chair;
		ObjMove house1_2F_TL_chair;
		ObjMove house1_basement2_chair;
		vector<Event> events;
		vector<Item> items;
		//map related
		CMovingBitmap mapmask;
		vector<GameMap> gamemaps;
		int _nowID;
		int _dialogID;
		int _dialogcount;
		int _eventID;
		vector <bool> darkroom;
		vector<vector<int>> blockLeftCor; //x y nowID
		vector<vector<int>> blockRightCor; //x y nowID
		vector<vector<int>> blockTeleportCor; //x y nowID
		vector<int> mapoverlayindex;
		MapRouter router;
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
		//int isbs;
		vector<int> twowayvec;
		vector<int> pointvec;
		//vector<int> bsvec;
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
