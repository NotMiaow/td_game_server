#include "gameServer.h"

GameServer::GameServer(const int& gameServerPort, Client* clients)
{
	m_alive = true;
	m_serverPort = gameServerPort;
	m_futureObj = m_exitSignal.get_future();
	
	m_actionQueue = SharedQueue<Action*>();
	m_networkManager = new NetworkManager(std::move(m_futureObj), gameServerPort, m_actionQueue);
	m_eventQueue = SharedQueue<Event*>();
	m_ecs = new ECS(m_networkManager, m_eventQueue, clients, std::move(m_futureObj), std::ref(m_alive));
	m_logic = new Logic(std::move(m_futureObj), clients, m_actionQueue, m_eventQueue);

	m_terminateThread = std::thread(&GameServer::WaitForTerminate, this);
}

GameServer::GameServer()
{

}

GameServer::~GameServer()
{
	m_terminateThread.join();
}

void GameServer::Stop()
{
	if(m_futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
		m_exitSignal.set_value();
}

void GameServer::WaitForTerminate()
{
    while(m_alive && m_futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
		 std::this_thread::sleep_for(std::chrono::milliseconds(POST_GAME_DURATION));
    m_alive = false;
	Stop();
}
