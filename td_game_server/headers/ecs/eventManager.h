#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>
#include <tuple>
#include <math.h>
#include <deque>

//Paths
#include "vector2.h"

//Events
#include "eventLanguage.h"
#include "event.h"
#include "shared_queue.h"

#include "entityCounter.h"
#include "componentArray.h"
//Components
#include "client.h"
#include "playerComponent.h"
#include "bankComponent.h"
#include "offenseComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Networking
#include "networkManager.h"

//Misc
#include "definitions.h"
#include "gameEnums.h"

class EventManager
{
public:
    EventManager() { }
    ~EventManager();
    void Init(Client* clients, NetworkManager& networkManager, SharedQueue<Event*>& eventQueue, EntityCounter& entityCounter, Players& players, Banks& banks,
            Offenses& offenses, Motors& motors, Transforms& transforms);
    void Loop();
private:
    void SwitchEvent();
    void ConnectPlayer();
    void DisconnectPlayer();
    void ReadyUpPlayer();
    void SpawnUnitGroup();
    void SendNewPath();
    void Rage();
    void BuildTower();
    void SellTower();
    int GetPlayerPosition(const int& clientId, PlayerComponent* player);
private:
    EntityCounter* m_entityCounter;
    Event* m_event;
    SharedQueue<Event*>* m_eventQueue;

   	Players* m_players;
    Banks* m_banks;
    Offenses* m_offenses;
   	Motors* m_motors;
	Transforms* m_transforms;

    NetworkManager* m_networkManager;
};

#endif
