#ifndef EVENT_H__
#define	EVENT_H__

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>

#include "node.h"
#include "chain.h"
#include "resourceComponent.h"
#include "basicLib.h"
#include "sharedLanguage.h"
#include "eventLanguage.h"
#include "vector2.h"

struct Event
{
	virtual EventType GetType() const = 0;
	virtual std::string ToNetworkable() const = 0;

	int clientId;
};

struct ErrorEvent : public Event
{
	ErrorEvent(const int& clientId, EventType eType, GameErrorType geType)
	{
		this->clientId = clientId;
		this->eType = eType;
		this->geType = geType;
	}
	EventType GetType() const { return EError; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EError << ";" << eType << ";" << geType << "}";
		return os.str();
	}

	EventType eType;
	GameErrorType geType;
};

struct ConnectEvent : public Event
{
	ConnectEvent(const int&  clientId)
	{
		this->clientId = clientId;
	}
	EventType GetType() const { return EConnect; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EConnect << "}";
		return os.str();
	}
};

struct DisconnectEvent : public Event
{
	DisconnectEvent(const int& clientId, DisconnectReason reason = RKicked)
	{

		this->clientId = clientId;
		this->reason = reason;
	}
	EventType GetType() const { return EDisconnect; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EDisconnect << ";" << reason << "}";
		return os.str();
	}

	DisconnectReason reason;
};

struct ReadyUpEvent : public Event
{
	ReadyUpEvent(const int&  clientId)
	{
		this->clientId = clientId;
	}
	EventType GetType() const { return EReadyUp; }
	std::string ToNetworkable() const
	{
		Node<ResourceComponent>* node = resources->GetHead();
		std::ostringstream os;
		os << "{" << EReadyUp << ";" << playerPosition << ";";
		while (node != NULL)
		{
			os << node->component.gold << ((node->next != NULL) ? ";" : "");
			node = resources->GetNext(*node);
		}
		os << "}";
		std::cout << os.str() << std::endl;
		return os.str();
	}

	int playerPosition;
	Chain<ResourceComponent>* resources;
};

struct SpawnUnitGroupEvent : public Event
{
	SpawnUnitGroupEvent() { }
	EventType GetType() const { return ESpawnUnitGroup; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESpawnUnitGroup << "}";
		return os.str();
	}
};

struct NewPathEvent : public Event
{
	NewPathEvent() { }
	EventType GetType() const { return ENewPath; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ENewPath << ";" << motorPosition << ";";
		Node<Position>* node = path->GetHead();
		while (node != NULL)
		{
			os << "(" << node->component.y << ":" << node->component.x << ")" << (node->next != NULL ? ";" : "");
			node = path->GetNext(*node);
		}
		os << "}";
		return os.str();
	}

	int motorPosition;
	Chain<Position>* path;
};

struct RageEvent : public Event
{
	RageEvent() { }
	EventType GetType() const { return ERage; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ERage << ";" << motorPosition << "}";
		return os.str();
	}

	int motorPosition;
};

struct BuildTowerEvent : public Event
{
	BuildTowerEvent(const int&  clientId, const int& towerType, const Position& position)
	{

		this->clientId = clientId;
		this->towerType = towerType;
		this->position = position;
	}
	EventType GetType() const { return EBuildTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EBuildTower << ";" << towerType << ";(" << position.y << ":" << position.x << ")}";
		return os.str();
	}

	int towerType;
	Position position;
};

struct SellTowerEvent : public Event
{
	SellTowerEvent(const int&  clientId, const int& towerPosition)
	{

		this->clientId = clientId;
		this->towerPosition = towerPosition;
	}
	EventType GetType() const { return ESellTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESellTower << ";" << towerPosition << ";" << sellValue << "}";
		return os.str();
	}

	int towerPosition;
	int sellValue;
};

struct SendUnitGroupEvent : public Event
{
	SendUnitGroupEvent(const int& clientId, const int& unitType)
	{
		this->clientId = clientId;
		this->unitType = unitType;
	}
	EventType GetType() const { return ESendUnitGroup; }
	std::string ToNetworkable() const
	{
		return "";	//This event is recieve only
	}

	int unitType;
};


#endif