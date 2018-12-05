#include "Connections.h"


std::string id = "";
std::string ip = "127.0.0.1";
SOCKET sock;
int port = 54000;
int lastupdate = 0;
int theamazingplayerid;
CardController* CardControllerConnections;
std::mutex* updateslower;

Connections::Connections(std::string theip, std::string theport, CardController* cardc, std::mutex* theupodate) {
	try {
		updateslower = theupodate;
		CardControllerConnections = cardc;
		ip = theip;
		port = stoi(theport);

		WSAData data;
		WORD ver = MAKEWORD(2, 2);

		int wsResult = WSAStartup(ver, &data);

		if (wsResult != 0) {
			OutputDebugString(L"Error!");
			return;
		}

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
			OutputDebugString(L"Error!");
			return;
		}

		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

		/*
		--const char optval = 1;
		int resulter = setsockopt(sock, NULL, SO_KEEPALIVE, &optval, sizeof(int));

		if (resulter < 0) {
			OutputDebugString(L" BROKEN? ! ");
		}
		*/


		if (connResult == SOCKET_ERROR) {
			OutputDebugString(L"ERROR");
			closesocket(sock);
			WSACleanup();
			return;
		}
		// yea

		char buf[16384];
		std::string userInput = "CREATE ID";

		while (userInput.size() > 0)
		{
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				ZeroMemory(buf, 16384);
				int bytesReceived = recv(sock, buf, 16384, 0);
				if (bytesReceived > 0) {
					id = std::string(buf, 0, bytesReceived);
					std::wstring wsTmp(id.begin(), id.end());
					OutputDebugString(L"Our id is = ");
					theamazingplayerid = stoi(id);
					OutputDebugString(wsTmp.c_str());
					clientid = theamazingplayerid;
					break;
					closesocket(sock);
					WSACleanup();
				}


			}



		}

		didconnect = true;
	}
	catch(int e){

		didconnect = false;
	}
}

void Connections::CloseConnection() {
	closesocket(sock);
}

std::string Connections::SendCommand(std::string command) {

	std::string theid;

	char buf[16384];

	while (command.size() > 0)
	{
		int sendResult = send(sock, command.c_str(), command.size() + 1, 0);
		if (sendResult != SOCKET_ERROR) {
			ZeroMemory(buf,16384);
			int bytesReceived = recv(sock, buf, 16384, 0);
			theid = std::string(buf, 0, bytesReceived);
			return theid;
		}
		command = "";
	}

	return "";
}



void Connections::createlist(Grafik* graphics) {

	updateslower->lock();
	char buf[16384];


	std::string userInput = "CARD GET";


	while (userInput != "CLOSE") {

		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);

		if (sendResult != SOCKET_ERROR) {




			ZeroMemory(buf, 16384);



			int bytesReceived = recv(sock, buf, 16384, 0);

			if (bytesReceived > 0) {

				userInput = std::string(buf, 0, bytesReceived);

				std::string keepinput = userInput;

				std::wstring getrespond(userInput.begin(), userInput.end());


				ZeroMemory(buf, 16384);

				std::stringstream ss(keepinput);
				std::string metexutre;
				std::string cardtheid;
				std::string cardflipped;
				std::string cardxcord;
				std::string cardycord;
				ss >> metexutre;
				ss >> cardtheid;
				ss >> cardtheid;
				ss >> cardflipped;
				ss >> cardflipped;
				ss >> cardxcord;
				ss >> cardxcord;
				ss >> cardycord;
				ss >> cardycord;

				if (metexutre != "CLOSE") {


					SpriteSheet* spriten = new SpriteSheet(std::wstring(metexutre.begin(), metexutre.end()).c_str(), graphics);
					Card* kortet = new Card(stoi(cardtheid), (float)stoi(cardxcord), (float)stoi(cardycord), std::wstring(metexutre.begin(), metexutre.end()).c_str(), stoi(cardflipped), spriten);
					CardControllerConnections->AddToAllList(kortet);
					CardControllerConnections->updateList(false, kortet);


				}
				else {

					break;
				}



			}



		}


	}
	updateslower->unlock();

}



std::list<Commands> Connections::ReturnCommands(std::string message) {
	std::list<Commands> thecommands = std::list<Commands>();

	if (std::regex_search(message, std::regex("COMMANDSLIST;")) > 0) {
		std::string commands = message;

		commands = message.replace(0, 13, "");



		std::list<std::string> ends = std::list<std::string>();

		std::list<std::string> respond = std::list<std::string>();


		std::istringstream iss(commands);

		std::istreambuf_iterator<char> begin(iss), end;
		std::string line = "";
		while (begin != end) {
			std::string current = std::string(1, char(*begin++));
			if (current == ";") {
				line = line + current;
				ends.push_back(line);
				line = "";
			}
			else {
				line = line + current;
			}
		}


		for (std::string x : ends) {
			Commands TheCommand;
			TheCommand.variables = std::list<std::string>();
			int at = 0;

			std::istringstream es(x);
			std::istreambuf_iterator<char> begine(es), ende;
			std::string currentline = "";
			while (begine != ende) {
				std::string current = std::string(1, char(*begine++));

				if (current == ";") {
					if (at == 0) {
						TheCommand.command = currentline;
						thecommands.push_back(TheCommand);
					}
					else {
						
						TheCommand.variables.push_back(currentline);
						thecommands.push_back(TheCommand);
					}

				}
				if (current == ":") {

					if (at == 0) {
						TheCommand.command = currentline;
						currentline = "";
					}
					else {
						TheCommand.variables.push_back(currentline);
						currentline = "";
					}
					at = at + 1;
				}
				else {
					if (current != ";") {
						currentline = currentline + current;
					}
				}

			}



		}


	}
	return thecommands;
}

std::list<Commands>  Connections::RegularUpdate(std::list<Commands> command) { // This will make my networking so much FASTER! 
	
	std::list<Commands> returns = std::list<Commands>();

	std::string tosend = "COMMANDSLIST;";
	for (Commands x : command) {
		tosend = tosend + x.command;

		for (std::string s : x.variables) {
			tosend = tosend + ":" + s;
		}
		tosend = tosend + ";";
	}
	
	char buf[16384];

	int sendResult = send(sock, tosend.c_str(), tosend.size() + 1, 0);

	ZeroMemory(buf, 16384);



	int bytesReceived = recv(sock, buf, 16384, 0);

	std::string response = std::string(buf, 0, bytesReceived);

	//OutputDebugString(std::wstring(response.begin(), response.end()).c_str());



	returns = ReturnCommands(response);

	return returns;
}


bool Connections::updatecard(Card* korta) {
	std::string resuler = "CARDUPDATETOSERVER " + std::to_string(korta->id) + " " + std::to_string(korta->flipped) + " " + std::to_string(korta->xcord) + " " + std::to_string(korta->ycord);
	send(sock, resuler.c_str(), resuler.size() + 1, 0);
	return true;
}
/*
bool Connections::flipcards() {
	std::string userInput = "CARD FLIP";
	int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
	OutputDebugString(L"SENT COMMAND!");
	return true;
}
*/