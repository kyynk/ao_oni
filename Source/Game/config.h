/////////////////////////////////////////////////////////////////////////////
// 定義遊戲可設定的環境與條件
/////////////////////////////////////////////////////////////////////////////

#define GAME_TITLE           "Game"
//! 設定遊戲畫面的解析度（寬度）
#define SIZE_X				 816	
//! 設定遊戲畫面的解析度（高度）
#define SIZE_Y				 820
//! 是否以全螢幕方式開啟遊戲
#define OPEN_AS_FULLSCREEN	 false		
//! 是否顯示loading(OnInit)的進度
#define SHOW_LOAD_PROGRESS   true		
// 遊戲畫面預設的背景顏色(黑色)
#define DEFAULT_BG_COLOR	 RGB(0, 0, 0)	
//! 每33ms跑一次Move及Show(每秒30次)
#define GAME_CYCLE_TIME		 15		    
//! 是否在debug mode顯示cycle time
#define SHOW_GAME_CYCLE_TIME false		
//! 是否允許以 Ctrl-Q 暫停遊戲
#define ENABLE_GAME_PAUSE	 false		
//! 啟動音效介面
#define ENABLE_AUDIO		 true		
 //! 是否關閉 toolbar
#define ENABLE_TOOLBAR       false     
//! 是否關閉 menu
#define ENABLE_MENU          false      
//! 是否關閉 statusbar
#define ENABLE_STATUSBAR     false      
//! 全螢幕用，請設定成目前視窗的解析度 (width)。
#define RESOLUTION_X     1920           
//! 全螢幕用，請設定成目前視窗的解析度 (height)。
#define RESOLUTION_Y     1080   
#define TILE 32
#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39
#define KEY_A 0x41
#define KEY_B 0x42
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_G 0x47
#define KEY_H 0x48
#define KEY_I 0x49
#define KEY_J 0x4a
#define KEY_K 0x4b
#define KEY_L 0x4c
#define KEY_M 0x4d
#define KEY_N 0x4e
#define KEY_O 0x4f
#define KEY_P 0x50
#define KEY_Q 0x51
#define KEY_R 0x52
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_U 0x55
#define KEY_V 0x56
#define KEY_W 0x57
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_Z 0x5a
#define VK_ENTER VK_RETURN
#define TOILET 0
#define TUB_ONCE 1
#define PHILLIPS 2
#define TUB_FIXED 3
#define BROKEN_DISH 4
#define LIB_BOOK 5
#define KEY_3F_L 6
#define KEY_LIB 7
#define DOOR_KNOB 8
#define DOOR_NO_KNOB 9
#define LIGHTER 10
#define TATAMI_L 11
#define TATAMI_R 11

/*
debug mode
press
e : enable map path edit
i : enable show map tile index
9 amd 0 : navigate through indexes of each map layers
j and k : navigate through maps
w : enable input box
space :  when input box is enable, escape input box.
		otherwise, if input is in the range of map index, teleport to to corresponding map

*/
#define BROKEN_DISH_E 0