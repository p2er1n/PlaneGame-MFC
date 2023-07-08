#pragma once
#include <afxwin.h>
#include "Level.h"
#include "Player.h"

#include <hv/TcpServer.h>
#include <hv/TcpClient.h>

#include <mutex>

#define GAME_WIDTH 500
#define GAME_HEIGHT 900

typedef enum {mSingle,mServer,mJoin} GameMode;

class MainWnd :
    public CFrameWnd
{
public:
    MainWnd(GameMode gm);
    MainWnd();

    ~MainWnd();

public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTimer(UINT_PTR timerID);

    void SetGameMode(GameMode gm);
    GameMode GetGameMode();

    void SetServerPort(int);
    int GetServerPort();

    void SetClientConnPort(int);
    int GetClientConnPort();

    void SetClientConnIpAddr(std::string& s);
    std::string GetClientConnIpAddr();

    void Init();
    void InitServer();
    void InitClient();

    void UpdateGame();
    void Draw();

    void SyncFriends();

    //member variables
private:
    GameMode m_eMode;
    CDC* m_pDC;
    CDC* m_pMemDC;
    CBitmap* m_pMemBitmap;

    //µ±Ç°¹Ø¿¨
    Level* m_curLevel = nullptr;

    Player* m_pPlayer;

    int m_iPort;
    hv::TcpServer m_Srv;
    std::vector<std::pair<uint32_t, Plane*>> m_Friends;
    std::queue<std::pair<uint32_t, Plane*>> m_Friends_buffer;
	std::mutex m_Friends_buffer_lock;

    std::string m_sIpAddr = "127.0.0.1";
    int m_iSrvPort = 1234;
    hv::TcpClient m_Clt;

    DECLARE_MESSAGE_MAP()
};

