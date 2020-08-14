#ifndef __CLIENT_H__
#define	__CLIENT_H__

#include "PBSocket.h"
#include "Character.h"
using namespace PB;

class Client : public tcp
{
private:
	std::vector<char>		_buffer;

private:
	int						_room;
	bool					_connected;
	Character				_character;
	bool					_isGenerated;
	bool					_isReady;
	int						_order;				//게임방의 슬롯 순서

public:
	Client(const socket& socket, StateChangeRoutine callback);
	~Client();

public:
	int						id() const;
	bool					enterRoom(int roomIndex);
	bool					leaveRoom();
	bool					isEnterRoom() const;
	int						currentRoom() const;
	bool					isGenerated() const;
	void					isGenerated(bool value);

	bool					ready() const;
	void					ready(bool state);

	int						order() const;
	void					order(int order);

public:
	bool					send(const Json::Value& json);
	bool					recv(Json::Value& json);

public:
	operator				Character& ();
};

#endif // !__CLIENT_H__