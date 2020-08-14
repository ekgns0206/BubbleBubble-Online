#include "Client.h"

Client::Client(const socket & socket, StateChangeRoutine callback) : tcp(socket), _room(-1), _character(-1, this->id(), callback), _order(-1)
{
	this->_isGenerated			= false;
	this->_isReady				= false;
}

Client::~Client()
{
}

int Client::id() const
{
	return (int)this->_socket;
}

bool Client::enterRoom(int roomIndex)
{
	if(this->isEnterRoom())
		return false;

	this->_room			= roomIndex;
	this->_character.roomIndex(roomIndex);
	return true;
}

bool Client::leaveRoom()
{
	if(this->isEnterRoom() == false)
		return false;

	this->_room			= -1;
	this->_isReady = false;
	this->_character.roomIndex(-1);
	return true;
}

bool Client::isEnterRoom() const
{
	return this->_room != -1;
}

int Client::currentRoom() const
{
	return this->_room;
}

bool Client::isGenerated() const
{
	return this->_isGenerated;
}

void Client::isGenerated(bool value)
{
	this->_isGenerated			= value;
}

bool Client::ready() const
{
	return this->_isReady;
}

void Client::ready(bool state)
{
	this->_isReady = state;
}

int Client::order() const
{
	return this->_order;
}

void Client::order(int order)
{
	this->_order = order;
}

bool Client::send(const Json::Value & json)
{
	std::string			str = json.toStyledString();
	std::vector<char>	bytes;

	if(json["method"].asString().compare("clear stage") == 0)
	{
		puts("clear stage");
	}

	int					size = str.length();
	char*				sizeRef = (char*)&size;
	
	bytes.insert(bytes.end(), sizeRef, sizeRef + sizeof(int));
	bytes.insert(bytes.end(), str.begin(), str.end());

	return tcp::send(bytes);
}

bool Client::recv(Json::Value & json)
{
	if(tcp::recv(this->_buffer, sizeof(int)) == false)
		return false;

	int						size = *(int*)(this->_buffer.data());
	if(tcp::recv(this->_buffer, size) == false)
		return false;

	std::string				str(this->_buffer.begin(), this->_buffer.end());
	Json::Reader			reader;

	if(reader.parse(str, json) == false)
		return false;

	return true;
}

Client::operator Character&()
{
	return this->_character;
}
