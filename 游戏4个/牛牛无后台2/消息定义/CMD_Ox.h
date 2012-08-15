#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						5										//游戏 I D
#define GAME_PLAYER					4										//游戏人数
#define GAME_NAME					TEXT("牛牛")							//游戏名字

#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)		//游戏类型
#define MAXCOUNT					5										//扑克数目

//结束原因
#define GER_NO_PLAYER				0x10									//没有玩家

//游戏状态
#define GS_TK_FREE					GS_FREE									//等待开始
#define GS_TK_CALL					GS_PLAYING								//叫庄状态
#define GS_TK_SCORE					GS_PLAYING+1							//下注状态
#define GS_TK_PLAYING				GS_PLAYING+2							//游戏进行

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_ADD_SCORE					101									//加注结果
#define SUB_S_PLAYER_EXIT				102									//用户强退
#define SUB_S_SEND_CARD					103									//发牌消息
#define SUB_S_GAME_END					104									//游戏结束
#define SUB_S_OPEN_CARD					105									//用户摊牌
#define SUB_S_CALL_BANKER				106									//用户叫庄

//游戏状态
struct CMD_S_StatusFree
{
	LONGLONG								lCellScore;							//基础积分
	//房间信息
	TCHAR									szGameRoomName[32];			//房间名称
	LONGLONG								lOtherScore;					//对家积分
};

//游戏状态
struct CMD_S_StatusCall
{
	WORD								wCallBanker;						//叫庄用户
};

//游戏状态
struct CMD_S_StatusScore
{
	//下注信息
	LONGLONG								lTurnMaxScore;						//最大下注
	//LONGLONG								lTurnLessScore;						//最小下注
	LONGLONG								lTableScore[GAME_PLAYER];			//下注数目
	WORD								wBankerUser;						//庄家用户
};

//游戏状态
struct CMD_S_StatusPlay
{
	//状态信息
	LONGLONG								lTurnMaxScore;						//最大下注
	//LONGLONG								lTurnLessScore;						//最小下注
	LONGLONG								lTableScore[GAME_PLAYER];			//下注数目
	WORD								wBankerUser;						//庄家用户

	//扑克信息
	BYTE								cbHandCardData[GAME_PLAYER][MAXCOUNT];//桌面扑克
	BYTE								bOxCard[GAME_PLAYER];				//牛牛数据
};

//用户叫庄
struct CMD_S_CallBanker
{
	WORD								wCallBanker;						//叫庄用户
	bool								bFirstTimes;						//首次叫庄
};

//游戏开始
struct CMD_S_GameStart
{
	//下注信息
	LONGLONG								lTurnMaxScore;						//最大下注
	WORD								wBankerUser;						//庄家用户
};

//用户下注
struct CMD_S_AddScore
{
	WORD								wAddScoreUser;						//加注用户
	LONGLONG								lAddScoreCount;						//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	LONGLONG								lGameTax[GAME_PLAYER];				//游戏税收
	LONGLONG								lGameScore[GAME_PLAYER];			//游戏得分
	BYTE								cbCardData[GAME_PLAYER];			//用户扑克
	LONGLONG								lOtherScore;					//对家信息
};

//发牌数据包
struct CMD_S_SendCard
{
	BYTE								cbCardData[GAME_PLAYER][MAXCOUNT];	//用户扑克
};

//用户退出
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//退出用户
};

//用户摊牌
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//摊牌用户
	BYTE								bOpen;								//摊牌标志
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER				1									//用户叫庄
#define SUB_C_ADD_SCORE					2									//用户加注
#define SUB_C_OPEN_CARD					3									//用户摊牌

//用户叫庄
struct CMD_C_CallBanker
{
	BYTE								bBanker;							//做庄标志
};

//用户加注
struct CMD_C_AddScore
{
	LONGLONG								lScore;								//加注数目
};

//用户摊牌
struct CMD_C_OxCard
{
	BYTE								bOX;								//牛牛标志
};

//////////////////////////////////////////////////////////////////////////
struct tagAdminReq
{
	BYTE							m_cbExcuteTimes;					//执行次数
	BYTE							m_cbControlStyle;					//控制方式
#define		CS_BANKER_LOSE    1
#define		CS_BANKER_WIN	  2
#define		CS_BET_AREA		  3
	bool							m_bWinArea[3];						//赢家区域
};


struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA		1
#define RQ_RESET_CONTROL	2
#define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//附加数据
};

#define SUB_S_AMDIN_COMMAND			110									//管理员命令

//请求回复
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//回复类型
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
	BYTE cbResult;
#define CR_ACCEPT  2			//接受
#define CR_REFUSAL 3			//拒绝
	BYTE cbExtendData[20];			//附加数据
};

#define AREA_COUNT					4									//区域数目
#define SUB_C_AMDIN_COMMAND			5									//管理员命令
#endif
