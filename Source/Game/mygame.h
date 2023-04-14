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
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/
#include "config.h"
#include "Entity.h"
#include "GameMap.h"
#include "Human.h"
#include "Oni.h"
#include "MapNode.h"
#include "MapRouter.h"
#include "MapRes.h"
#include "ChoiceMenu.h"
#include "Item.h"
#include "ObjMove.h"
#include "Dialog.h"
#include "InputBox.h"
#include "Human.h"
#include "Event.h"
#include <fstream>

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	
	enum AUDIO_ID {				// 定義各種音效的編號
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
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnKeyDown(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
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
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	protected:
		void OnMove();
		// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		
		void DeBugRecursive();
		//game
		int _substate;
		
		vector<Dialog> dialogs;
		Human player;
		Human player2;
		Human player3;
		Human player4;
		Oni oni1;
		ObjMove redChair;
		vector<Event> events;
		vector<Item> items;
		//map related
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
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
		void OnKeyDown(UINT, UINT, UINT);

	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
		CMovingBitmap GameOver;
	};

}
