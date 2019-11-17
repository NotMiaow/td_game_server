#include "eventManager.h"

EventManager::EventManager(NetworkManager* networkManager, Client* clients, SharedQueue<Event*>& eventQueue,
                            CheckpointList<PlayerComponent>& players, CheckpointList<MotorComponent>& motors,
                            CheckpointList<TransformComponent>& transforms)
{
    m_networkManager = networkManager;
    m_eventQueue = &eventQueue;

    m_players = &players;
    m_motors = &motors;
    m_transforms = &transforms;

    //Seed players
    CheckpointList<PlayerComponent>::Node<PlayerComponent>* pit = m_players->GetNodeHead();
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        PlayerComponent player;
        player.client = &clients[i];
        player.connected = false;
        player.ready = false;
        player.lives = STARTING_LIVES;
        pit = m_players->InsertAfterNode(player, pit);
    }

    //Create Motor
//    MotorComponent motor;
//    Queue<Vector2> path(30);
//    Vector2 target1;
//    Vector2 target2;
//    Vector2 target3;
//    Vector2 target4;
//    target1.x = 8;
//    target1.y = 3;
//    target2.x = 2;
//    target2.y = 6;
//    target3.x = 10;
//    target3.y = 8;
//    target4.x = 2;
//    target4.y = GRID_SIZE_Y + DESPAWN_SIZE;
//    path.Push(target4);
//    path.Push(target3);
//    path.Push(target2);
//    path.Push(target1);
//    motor.path = path;
//    motor.behaviour = Move;
//    motor.baseSpeed = 2;
//    motor.curSpeed = 2;
//    m_motors->InsertAfterNode(motor, m_motors->GetNodeHead());
//
//    //Create Transform
//    TransformComponent transform;
//    transform.position.x = SPAWN_POSITION_X;
//    transform.position.y = SPAWN_POSITION_Y;
//    //Update normalised target
//	float distanceX = motor.path.Front().x - transform.position.x;
//	float distanceY = motor.path.Front().y - transform.position.y;
//	float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
//	transform.normalizedTarget.x = distanceX / distance;
//	transform.normalizedTarget.y = distanceY / distance;
//    m_transforms->InsertAfterNode(transform, m_transforms->GetNodeHead());
}

EventManager::~EventManager()
{
    
}

void EventManager::Loop()
{
    while (m_eventQueue->GetSize())
    {
        m_event = m_eventQueue->Pop();
        if(m_event != 0)
        {
            std::cout << m_event->ToNetworkable() << std::endl;
            SwitchEvent();
        }
        delete m_event;
    }
    PlayerComponent player;
}

void EventManager::SwitchEvent()
{
    switch(m_event->GetType())
    {
    case EError:
        break;
    case EConnect:
        ConnectPlayer();
        std::cout << "works" << std::endl;
        break;
    case EDisconnect:
        break;
    case EReadyUp:
        break;
    case ESpawnUnitGroup:
        break;
    case ENewPath:
        break;
    case ERage:
        break;
    case EBuildTower:
        break;
    case ESellTower:
        break;
    case ESendUnitGroup:
        break;
    }
}

void EventManager::ConnectPlayer()
{
    m_event = dynamic_cast<ConnectEvent*>(m_event);
    PlayerComponent player;
    if(FindPlayerByClientId(m_event->clientId, player))
        m_networkManager->MessageClient(player.client->socketId, m_event->ToNetworkable());
    std::cout << "sent: " << m_event->ToNetworkable() << std::endl;
}

bool EventManager::FindPlayerByClientId(const int& clientId, PlayerComponent& player)
{
    CheckpointList<PlayerComponent>::Node<PlayerComponent>* pit = m_players->GetNodeHead();
    while(pit->data.client->id != clientId && pit)
    {
        if(pit->data.client->id)
        {
            player = pit->data;
            return true;
        }
        pit = m_players->GetNextNode(&*pit);
    }
    return false;
}
