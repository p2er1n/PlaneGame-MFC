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

	//�������ô��ڵĿ��
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
	//�豸DC
	m_pDC = new CClientDC(this);

	//�ڴ�DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//�ڴ�λͼ
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC, GAME_WIDTH, GAME_HEIGHT);

	m_pMemDC->SelectObject(m_pMemBitmap);

	return 0;
}

//��ʱ���º��� 
void MainWnd::OnTimer(UINT_PTR timerID)
{
	UpdateGame();//��Ϸ�߼�����
	Draw();//����

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

	//��Ϸ��ʱ������
	SetTimer(1, 30, nullptr);

}

void MainWnd::InitServer()
{
	m_Srv.createsocket(m_iPort);
	m_Srv.onConnection = [&](const hv::SocketChannelPtr& channel) {
		if (!channel->isConnected())
			return;

		/*
		//�ͻ��˵�һ������,��¼������ID���ҳ�ʼ�����ѷɻ�
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
		//ɾ��ͷ,ȥͷ���ܳ� :)
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

			//�ͻ��˷�����ֻ����ս���İ��������жϱ�ʶ
			//ɾ��ͷ,ȥͷ���ܳ� :)
			auto it = std::find(packet.begin(),packet.end(),',');
			packet.erase(packet.begin(),it+1);

			//��ֱ�Ӳ���������ȾҪ�õģ��ȷŵ���������
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
			//AfxMessageBox(L"����������");

			/*
			auto id = channel->id();
			auto newFri = new Friend;
			m_Friends_buffer.push(std::pair<uint32_t,Plane*>(id, newFri));
			*/

			m_Friends.push_back(std::pair<uint32_t,Plane*>(0,new Friend));
		}
		else {
			//AfxMessageBox(L"��������");
		}
	};

	m_Clt.onMessage = [&](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
		//ÿ�ν��յ���Ϣ�ͽ�������¹ؿ��ͶԷ�ս��״̬
		//���ݰ�ͷ��ʶ�ж��ǹؿ�������ս����

		std::string packet = static_cast<char*>(buf->data());

		//debug log
		//std::ofstream outputfile("C:\\Users\\user\\Desktop\\received_packet.log");
		//outputfile << packet << std::endl;
		//outputfile.close();
		

		auto it = find(packet.begin(),packet.end(),',');
		std::string sign = std::string(packet.begin(),it);
		packet = std::string(it + 1 , packet.end());


		if (sign == "Level") {
			m_curLevel->Unpack(packet);//������Ϣ��������ȫͬ����ObjList buffer�У���ֱ���޸�ObjList
		}
		else if (sign == "Player") {
			//std::lock_guard<std::mutex> lock_guard(m_Friends_buffer_lock);
			//��֤friends buffer������һ���߳��޸ĵ�ʱ����һ���̷߳��ʽ�����ɵ�����ʧЧ

			/*
			//����������ʱ�����������first�������ӵĿͻ��˵�id
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
	ȷ�ϵ�ǰ�ؿ��������ؿ���Ĺ���͵��ߵȣ��Լ������ѵķɻ������ڹؿ��������Ǵ����ߣ�
	�ж��߼��������߼�������Щ��Ϸ�����״̬��λ�ã�Ѫ����buff�ȣ�
	�жϹؿ�����ȷ���Ƿ�����ؿ�

	���GameMode�Ƿ�����ģʽ���͸��¹ؿ�״̬�����͹ؿ���ս��״̬�����նԷ�ս��״̬

	���GameMode�ǵ���ģʽ��ֻ��Ҫ���¹ؿ�״̬���� ��

	���GameMode������ģʽ������Ҫ���¹ؿ�״̬�����չؿ�״̬�����ܶԷ�ս��״̬�������Լ�״̬��ͬ���ؿ�״̬

	================================
    �����

    onConnection �ͻ��˵�һ�������Ϸ������ˣ���¼������ID����������friend

    ʹ��ÿ����¼������ID��Ŀǰֱ�ӹ㲥���ͣ���
	����ÿ�����ѵ�״̬�����±�������״̬�������Լ�״̬��
	���¹ؿ�״̬������Լ�״̬������ؿ�״̬������״̬���°�

    ================================

    ================================
    �ͻ���
	onConnection �ͻ��˵�һ�������Ϸ�������ʱ�� �壬����֪����ʱ���ɶ��д�ϰ���

    onMessage ÿ�ν�����Ϣ�����������״̬
	(�ҵķ����ǣ�������ؿ�����״̬�����������á���(lll�V�ةV))

    ÿ��updateGame�����Լ���pack

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
			//......�ٵĴ��ӵ�ʵ��
			static_cast<Friend*>(m_Friends.begin()->second)->Update();
			m_curLevel->Update(m_Friends.begin()->second);
		}



		//������͹ؿ�����
		packet += m_curLevel->Pack();
		m_Srv.broadcast(packet);

		//debug log
		//outputfile << packet << std::endl;

		//ս������
		packet.clear();
		packet += "Player,";//�Ӹ���ͷ���������ĺ���û��
		packet += m_pPlayer->Pack();
		m_Srv.broadcast(packet);
		
		/*
		//�Է�ս��״̬�жϺ͸��£��ӵ��Ƿ��ɱ���˵ȵ�
		for (auto it = m_Friends.begin(); it != m_Friends.end(); ++it) {
			m_curLevel->Update(static_cast<Friend*>((*it).second));
		}
		*/

		//SyncFriends();//�Ѷ�����ķŵ�Friends��

		break;
	case mJoin:

		//���±��ص��Լ����ƶ���꣬���ӵ�ɶ��
		m_pPlayer->Update();

		if (m_Friends.size() == 1) {
			//......�ٵĴ��ӵ�ʵ��
			static_cast<Friend*>(m_Friends.begin()->second)->Update();
		}


		packet += "Player,";//�Ӹ�ͷ
		packet += m_pPlayer->Pack();
		m_Clt.send(packet);//���ͣ�Ŷ�𣡣�

		m_curLevel->Sync();//��ObjList buffer��ͬ����ObjList��
		//SyncFriends();//��Friends buffer��ͬ����Friends

		break;
	}

	//outputfile.close();
}

void MainWnd::Draw()
{
	/*
`   �������õ�ǰ�ؿ�������Ϸ�����Draw�����������ڴ�DC���Ƶ��豸DC
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
	//��֤�ڷ���buffer��������ʱ�򲻻���Ϊ��һ���߳��޸�buffer�����µ�����ʧЧ

	//��ֻ֤��һ���߳��޸�Friends����ʹ��Friends�ĵ�����

	while (m_Friends_buffer.size() > 0) {
		auto friendPair = m_Friends_buffer.front(); m_Friends_buffer.pop();

		auto id = friendPair.first;
		auto it = find_if(m_Friends.begin(), m_Friends.end(), [&](const decltype(friendPair)& pair) {
			return id == pair.first;
			});
		if (it == m_Friends.end()) {
			//����û�����
			m_Friends.push_back(std::pair<uint32_t,Plane*>(id,
				new Friend(*static_cast<Friend*>(friendPair.second))));

			//ɾ��buffer�����,����ָ���Ѿ�pop����
			delete friendPair.second;
		} 
		else{
			//�����������
			//ɾ������˵ı�����ʵ������������һ���µ�ʵ����Ȼ��ɾ����������ʵ��
			delete it->second;

			it->second = new Friend(*static_cast<Friend*>(friendPair.second));

			delete friendPair.second;
		}
	}
	
}
