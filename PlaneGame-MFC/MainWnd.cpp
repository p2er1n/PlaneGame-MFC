#include <string>

#include "MainWnd.h"
#include "Friend.h"

#include <fstream>

BEGIN_MESSAGE_MAP(MainWnd,CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

MainWnd::MainWnd(GameMode gm) :
	CFrameWnd(),m_eMode(gm),m_iPort(0)
{
	if (!Create(nullptr, L"PlaneGame")) {
		AfxMessageBox(LPTSTR((std::to_string(GetLastError()) + "MainWnd created was failed!").c_str()));
	}

	//重新设置窗口的宽高
	SetWindowPos(nullptr,0,0,GAME_WIDTH,GAME_HEIGHT,SWP_NOMOVE | SWP_NOZORDER);

	//disable maximization and resize window size
	ModifyStyle(WS_MAXIMIZEBOX | WS_THICKFRAME, 0);
}

MainWnd::MainWnd():
	MainWnd(mSingle)
{
}

MainWnd::~MainWnd()
{
	delete m_curLevel;
	delete m_pPlayer;

	//delete friends
	for (auto pair = m_Friends.begin(); pair != m_Friends.end();) {
		delete pair->second;
		pair = m_Friends.erase(pair);
	}

	//delete friends buffer
	while (m_Friends_buffer.size() > 0) {
		auto pair = m_Friends_buffer.front();
		m_Friends_buffer.pop();
		delete pair.second;
	}
}

int MainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	srand(time(nullptr));
	//设备DC
	m_pDC = new CClientDC(this);

	//内存DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//内存位图
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC, GAME_WIDTH, GAME_HEIGHT);

	m_pMemDC->SelectObject(m_pMemBitmap);

	return 0;
}

//定时更新函数 
void MainWnd::OnTimer(UINT_PTR timerID)
{
	UpdateGame();//游戏逻辑更新
	Draw();//绘制

	CWnd::OnTimer(timerID);
}

void MainWnd::SetGameMode(GameMode gm)
{
	this->m_eMode = gm;
	//AfxMessageBox(LPTSTR((std::to_string(m_eMode)).c_str()));
}

GameMode MainWnd::GetGameMode()
{
	return m_eMode;
}

void MainWnd::SetServerPort(int port)
{
	m_iPort = port;
}

int MainWnd::GetServerPort()
{
	return m_iPort;
}

void MainWnd::SetClientConnPort(int p)
{
	m_iSrvPort = p;
}

int MainWnd::GetClientConnPort()
{
	return m_iSrvPort;
}

void MainWnd::SetClientConnIpAddr(std::string& s)
{
	m_sIpAddr = s;
}

std::string MainWnd::GetClientConnIpAddr()
{
	return m_sIpAddr;
}

void MainWnd::Init()
{
	m_curLevel = new Level;
	m_pPlayer = new Player(0, 0, 10);

	CString sTitle;
	switch (m_eMode) {
	case mSingle:
		sTitle.Format(_T("%s"),L"PlaneGame-SingleMode");
		break;
	case mServer:
		sTitle.Format(_T("%s %d"),L"PlaneGame-ServerMode Port:",m_iPort);
		InitServer();
		break;
	case mJoin:
		sTitle.Format(_T("%s"), L"PlaneGame-JoinMode");
		InitClient();
		break;
	}

	SetWindowTextW(sTitle);	

	//游戏定时更新器
	SetTimer(1, 30, nullptr);

}

void MainWnd::InitServer()
{
	m_Srv.createsocket(m_iPort);
	m_Srv.onConnection = [&](const hv::SocketChannelPtr& channel) {
		if (!channel->isConnected())
			return;

		/*
		//客户端第一次连上,记录下连接ID并且初始化朋友飞机
		auto id = channel->id();
		if (std::find_if(m_Friends.begin(), m_Friends.end(),
			[id](const std::pair<uint32_t, Plane*>& p)->bool {return id == p.first; })
			== m_Friends.end())
		{
			m_Friends_buffer.push(std::pair<uint32_t, Plane*>(id, new Friend));
		}
		*/

		m_Friends.push_back(std::pair<uint32_t, Plane*>(0, new Friend));
	};

	m_Srv.onMessage = [&](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
		if (!channel->isConnected())
			return;

		std::string packet(static_cast<char*>(buf->data()));
		//删掉头,去头才能吃 :)
		auto it = std::find(packet.begin(), packet.end(), ',');
		packet.erase(packet.begin(), it + 1);

		auto fri = m_Friends.begin();
		static_cast<Friend*>(fri->second)->Unpack(packet);

		/*
		auto id = channel->id();
		decltype(m_Friends.begin()) it;
		if ((it = std::find_if(m_Friends.begin(), m_Friends.end(),
			[id](const std::pair<uint32_t, Plane*>& p)->bool{return id == p.first; }))
			!= m_Friends.end()) 
		{
			std::string packet(static_cast<char*>(buf->data()));

			//客户端发来的只能是战机的包，不用判断标识
			//删掉头,去头才能吃 :)
			auto it = std::find(packet.begin(),packet.end(),',');
			packet.erase(packet.begin(),it+1);

			//不直接操作本地渲染要用的，先放到队列里面
			auto newFri = new Friend();
			static_cast<Friend*>(newFri)->Unpack(packet);
			m_Friends_buffer.push(std::pair<uint32_t,Plane*>(id,newFri));
		}
		*/

	};

	m_Srv.start();
}

void MainWnd::InitClient()
{
	m_Clt.createsocket(m_iSrvPort,m_sIpAddr.c_str());
	m_Clt.onConnection = [&](const hv::SocketChannelPtr& channel) {
		if (channel->isConnected()) {
			//AfxMessageBox(L"连上啦！！");

			/*
			auto id = channel->id();
			auto newFri = new Friend;
			m_Friends_buffer.push(std::pair<uint32_t,Plane*>(id, newFri));
			*/

			m_Friends.push_back(std::pair<uint32_t,Plane*>(0,new Friend));
		}
		else {
			//AfxMessageBox(L"断啦！！");
		}
	};

	m_Clt.onMessage = [&](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
		//每次接收到消息就解包，更新关卡和对方战机状态
		//根据包头标识判断是关卡包还是战机包

		std::string packet = static_cast<char*>(buf->data());

		//debug log
		//std::ofstream outputfile("C:\\Users\\user\\Desktop\\received_packet.log");
		//outputfile << packet << std::endl;
		//outputfile.close();
		

		auto it = find(packet.begin(),packet.end(),',');
		std::string sign = std::string(packet.begin(),it);
		packet = std::string(it + 1 , packet.end());


		if (sign == "Level") {
			m_curLevel->Unpack(packet);//接收信息，并且完全同步到ObjList buffer中，不直接修改ObjList
		}
		else if (sign == "Player") {
			//std::lock_guard<std::mutex> lock_guard(m_Friends_buffer_lock);
			//保证friends buffer不会在一个线程修改的时候被另一个线程访问进而造成迭代器失效

			/*
			//开服务器的时候朋友链表的first代表连接的客户端的id
			auto newFri = new Friend;
			newFri->Unpack(packet);
			m_Friends_buffer.push(std::pair<uint32_t, Plane*>(channel->id(), newFri));
			*/

			auto fri = m_Friends.begin();
			static_cast<Friend*>((*fri).second)->Unpack(packet);

		}
	};

	m_Clt.start();
}

void MainWnd::UpdateGame()
{
	/*
	确认当前关卡，遍历关卡里的怪物和道具等（自己和朋友的飞机不属于关卡，他们是闯关者）
	判断逻辑并根据逻辑更新这些游戏物体的状态（位置，血量，buff等）
	判断关卡，并确认是否更换关卡

	如果GameMode是服务器模式，就更新关卡状态，发送关卡和战机状态，接收对方战机状态

	如果GameMode是单人模式，只需要更新关卡状态即可 √

	如果GameMode是联机模式，不需要更新关卡状态，接收关卡状态，接受对方战机状态，发送自己状态，同步关卡状态

	================================
    服务端

    onConnection 客户端第一次连接上服务器端，记录下连接ID，并且新增friend

    使用每个记录的连接ID（目前直接广播发送）：
	接收每个朋友的状态，更新本地朋友状态，更新自己状态，
	更新关卡状态，打包自己状态，打包关卡状态，发送状态更新包

    ================================

    ================================
    客户端
	onConnection 客户端第一次连接上服务器的时候 囧，还不知道这时候干啥，写上吧先

    onMessage 每次接收消息，解包，更新状态
	(我的方案是，先清楚关卡所有状态，再重新设置。。(lll￢ω￢))

    每次updateGame发送自己的pack

    ================================

	*/

	std::string packet;
	//std::ofstream outputfile("C:\\Users\\user\\Desktop\\sended_packet.log", std::ios::trunc);

	switch (m_eMode) {
	case mSingle:
		m_curLevel->Update(m_pPlayer);
		m_pPlayer->Update();
		break;
	case mServer:
		m_curLevel->Update(m_pPlayer);
		m_pPlayer->Update();
		
		
		if (m_Friends.size() == 1) {
			//......假的传子弹实现
			static_cast<Friend*>(m_Friends.begin()->second)->Update();
			m_curLevel->Update(m_Friends.begin()->second);
		}



		//打包发送关卡数据
		packet += m_curLevel->Pack();
		m_Srv.broadcast(packet);

		//debug log
		//outputfile << packet << std::endl;

		//战机数据
		packet.clear();
		packet += "Player,";//加个包头，下面打包的函数没加
		packet += m_pPlayer->Pack();
		m_Srv.broadcast(packet);
		
		/*
		//对方战机状态判断和更新，子弹是否击杀敌人等等
		for (auto it = m_Friends.begin(); it != m_Friends.end(); ++it) {
			m_curLevel->Update(static_cast<Friend*>((*it).second));
		}
		*/

		//SyncFriends();//把队列里的放到Friends中

		break;
	case mJoin:

		//更新本地的自己，移动鼠标，发子弹啥的
		m_pPlayer->Update();

		if (m_Friends.size() == 1) {
			//......假的传子弹实现
			static_cast<Friend*>(m_Friends.begin()->second)->Update();
		}


		packet += "Player,";//加个头
		packet += m_pPlayer->Pack();
		m_Clt.send(packet);//发送，哦吼！！

		m_curLevel->Sync();//从ObjList buffer中同步到ObjList，
		//SyncFriends();//从Friends buffer中同步到Friends

		break;
	}

	//outputfile.close();
}

void MainWnd::Draw()
{
	/*
`   遍历调用当前关卡所有游戏物体的Draw函数，最后把内存DC复制到设备DC
	*/
	m_curLevel->Draw(m_pMemDC);

	//draw player
	m_pPlayer->Draw(m_pMemDC,m_pPlayer->GetImageList());

	//draw friends
	for (auto it = m_Friends.begin(); it != m_Friends.end();++it) {
		auto fri = static_cast<Friend*>((*it).second);
		fri->Draw(m_pMemDC, fri->GetImageList());
	}

	m_pDC->BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, m_pMemDC, 0, 0, SRCCOPY);

}

void MainWnd::SyncFriends()
{
	//std::lock_guard<std::mutex> lock(m_Friends_buffer_lock);
	//保证在访问buffer迭代器的时候不会因为另一个线程修改buffer而导致迭代器失效

	//保证只有一个线程修改Friends，和使用Friends的迭代器

	while (m_Friends_buffer.size() > 0) {
		auto friendPair = m_Friends_buffer.front(); m_Friends_buffer.pop();

		auto id = friendPair.first;
		auto it = find_if(m_Friends.begin(), m_Friends.end(), [&](const decltype(friendPair)& pair) {
			return id == pair.first;
			});
		if (it == m_Friends.end()) {
			//本地没这个人
			m_Friends.push_back(std::pair<uint32_t,Plane*>(id,
				new Friend(*static_cast<Friend*>(friendPair.second))));

			//删掉buffer里面的,他的指针已经pop掉了
			delete friendPair.second;
		} 
		else{
			//本地有这个人
			//删掉这个人的本来的实例，拷贝构造一个新的实例，然后删除被拷贝的实例
			delete it->second;

			it->second = new Friend(*static_cast<Friend*>(friendPair.second));

			delete friendPair.second;
		}
	}
	
}
