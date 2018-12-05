#pragma once
#include "iostream"
#include "WS2tcpip.h"
#include "string"
#include "list"
#include "Card.h"
#include "sstream"
#include "vector"
#include "Graphics.h"
#include "CardController.h"
#include "Commands.h"
#include "regex"
class Connections  {


public:
	Connections(std::string, std::string, CardController *, std::mutex *); 
	void CloseConnection();
		void createlist(Grafik*);
		std::list<Commands> RegularUpdate(std::list<Commands> );
		std::list<Commands> ReturnCommands(std::string);
		bool didconnect = false;
		std::string SendCommand(std::string);
		bool updatecard(Card* korta);
		int clientid = -1;

};