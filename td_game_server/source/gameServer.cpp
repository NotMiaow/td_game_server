#include "gameServer.h"

GameServer::GameServer(const int& gameServerPort, Client* clients)
{
	//Terminate
	m_alive = true;
	m_serverPort = gameServerPort;
	m_futureObj = m_exitSignal.get_future();
	m_terminateThread = std::thread(&GameServer::WaitForTerminate, this);
	
	//Networking
	m_clients = clients;
	m_networkManager.Init(std::move(m_futureObj), gameServerPort, m_actionQueue);
	m_actionManager.Init(clients, m_actionQueue, m_eventQueue);
	
	//Initialise component data structure
	m_players.Initialise({ 1, 1, 1, 1 });
	m_banks.Initialise({ 1, 1, 1, 1 });
	m_motors.Initialise({ 9, 9, 9, 9 });
	m_transforms.Initialise({ 9, 110, 9, 110, 9, 110, 9, 110 });
	m_offenses.Initialise({ 9, 110, 9, 110, 9, 110, 9, 110 });
	
	//ECS
	m_eventManager.Init(m_clients, m_networkManager, m_eventQueue, m_entityCounter, m_players, m_banks, m_offenses, m_motors, m_transforms);	
	m_ecs.Init(m_eventQueue, m_entityCounter, m_players, m_banks, m_offenses, m_motors, m_transforms);

    //Main loop
    m_mainLoopThread = std::thread(&GameServer::Loop, this);
}

GameServer::~GameServer()
{
	m_mainLoopThread.join();
	m_terminateThread.join();
	delete[] m_clients;
}

void GameServer::Loop()
{
	while(m_alive)
	{
		m_actionManager.Loop();
		m_eventManager.Loop();
		if(!m_ecs.Loop()) Stop();
	}
}

void GameServer::Stop()
{
	if(m_futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
		m_exitSignal.set_value();
    m_alive = false;
}

void GameServer::WaitForTerminate()
{
    while(m_alive && m_futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout);
		std::this_thread::sleep_for(std::chrono::milliseconds(POST_GAME_DURATION));
}
