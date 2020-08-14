#include "Room.h"

Room::Room(const std::string& name, Client* priest, int capacity) : _name(name), _priest(priest), _capacity(capacity), _level(-1)
{
	static int			ROOM_INDEX = 0;		// 방이 생성될때마다 부여될 id

	this->_index		= ROOM_INDEX++;
	this->_state		= State::WAIT;

	memset(&this->_clients, NULL, sizeof(this->_clients));
}

Room::~Room()
{
	for (std::vector<Enemy*>::iterator i = this->_enemies.begin(); i != this->_enemies.end(); i++)
		delete (*i);
}

//
// id
//	방의 고유 id를 얻는다.
//
// Parameters
//	없음
//
// Return
//	방의 고유 id를 리턴
//
int Room::id() const
{
	return this->_index;
}

//
// name
//	방의 이름을 얻는다.
//
// Parameters
//	없음
//
// Return
//	방의 이름을 얻는다.
//
const std::string & Room::name() const
{
	return this->_name;
}

//
// priest
//	방장의 정보를 얻는다.
//
// Parameters
//	없음
//
// Return
//	방장을 리턴
//
Client * Room::priest() const
{
	return this->_priest;
}

bool Room::priest(int index)
{
	if(index < 0 || index > this->_capacity)
		return false;

	if(this->_clients[index] == NULL)
		return false;

	this->_priest = this->_clients[index];
	return true;
}

void Room::autoResetPriest()
{
	for(int i = 0; i < this->_capacity; i++)
	{
		if(this->_clients[i] == NULL)
			continue;

		if(this->_clients[i] == this->_priest)
			continue;

		this->_priest = this->_clients[i];
		break;
	}
}

//
// Parameters
//	최대 수용 인원을 얻는다.
//
// Parameters
//	없음
//
// Return
//	게임방의 최대 수용 인원을 리턴
//
int Room::capacity() const
{
	return this->_capacity;
}

//
// state
//	게임방의 현재 상태를 얻는다.
//
// Parameters
//	없음
//
// Return
//	게임방의 현재 상태를 리턴
//
Room::State Room::state() const
{
	return this->_state;
}

void Room::state(Room::State value)
{
	this->_state = value;
}

//
// clients
//	게임방에 존재하는 클라이언트 정보를 얻는다.
//
// Parameters
//	없음
//
// Return
//	게임방에 존재하는 클라이언트 정보를 리턴
//
Client** Room::clients()
{
	return this->_clients;
}

Client* Room::client(int index)
{
	if(index < 0 || index > MAX_CLIENT_COUNT_IN_ROOM - 1)
		return NULL;

	return this->_clients[index];
}

int Room::clientCount() const
{
	int count = 0;
	for(int i = 0; i < this->_capacity; i++)
	{
		if(this->_clients[i] == NULL)
			continue;

		count++;
	}

	return count;
}

//
// enemies
//	게임방에 존재하는 적군의 정보를 얻는다.
//
// Parameterse
//	없음
//
// Return
//	게임방에 존재하는 적군의 정보를 리턴
//
std::vector<Enemy*>& Room::enemies()
{
	return this->_enemies;
}

//
// enter
//	게임방에 클라이언트를 추가한다.
//
// Parameters
//	client		추가할 클라이언트
//
// Return
//	성공시 true, 실패시 false를 리턴
//
bool Room::enter(Client * client)
{
	if(this->isFull())
		return false;

	if(this->_state != WAIT)
		return false;

	for(int i = 0; i < this->_capacity; i++)
	{
		if(this->_clients[i] != NULL)
			continue;

		this->_clients[i] = client;
		this->_clients[i]->order(i);
		break;
	}

	return true;
}

//
// leave
//	게임방에서 클라이언트를 내보낸다.
//
// Parameters
//	client		내보낼 클라이언트
//
// Return
//	성공시 true,  실패시 false를 리턴
//
bool Room::leave(Client * client)
{
	int order = client->order();
	if(order == -1)
		return false;

	if (this->_priest == this->_clients[order])
		this->autoResetPriest();

	this->_clients[order]->order(-1);
	this->_clients[order] = NULL;

	return true;
}

//
// level
//	게임방의 현재 스테이지 레벨을 얻는다.
//
// Parameters
//	없음
//
// Return
//	게임방의 현재 스테이지 레벨을 리턴
//
int Room::level() const
{
	return this->_level;
}

//
// init
//	게임을 시작하기 전 초기화한다.
//
// Parameters
//	없음
//
// Return
//	없음
//
void Room::init()
{
	for(int i = 0; i < this->_capacity; i++)
	{
		if(this->_clients[i] == NULL)
			continue;

		Character&				character = *this->_clients[i];
		character.init();
	}
}

//
// nextLevel
//	다음 스테이지 레벨로 바꾼다.
//
// Parameters
//	없음
//
// Return
//	최대 레벨에 도달한 경우 false, 진행 가능한 경우 true를 리턴
//
bool Room::nextLevel()
{
	this->_bubbles.clear();
	for (int i = 0; i < this->_capacity; i++)
	{
		if (this->_clients[i] == NULL)
			continue;

		Character&				character = *this->_clients[i];
		character.stop();
	}

	this->_level++;
	if(this->_level < MAX_LEVEL)
		return false;

	return true;
}

//
// mapName
//	현재 사용되는 맵의 이름을 얻는다.
//
// Parameters
//	없음
//
// Return
//	현재 사용되는 맵의 이름
//
std::string Room::mapName() const
{
	char			buffer[256]	= {0,};
	sprintf(buffer, "map%03d.pbm", this->_level);

	return std::string(buffer);
}

//
// addBubble
//	버블을 추가한다.
//
// Parameters
//	bubble		추가할 버블
//
// Return
//	없음
//
void Room::addBubble(const Bubble& bubble)
{
	this->_bubbles.push_back(bubble);
}

//
// bubbles
//	현재 존재하는 버블의 정보를 얻는다.
//
// Parameters
//	없음
//
// Return
//	버블들의 정보를 리턴
//
std::vector<Bubble>& Room::bubbles()
{
	return this->_bubbles;
}

bool Room::isLose() const
{
	for (int i = 0; i < this->_capacity; i++)
	{
		if (this->_clients[i] == NULL)
			continue;

		Character&				character = *this->_clients[i];
		if(character.respawnable())
			return false;
	}

	return true;
}

bool Room::isClear() const
{
	return this->_enemies.size() == 0;
}

bool Room::isFull() const
{
	for(int i = 0; i < this->_capacity; i++)
	{
		if(this->_clients[i] == NULL)
			return false;
	}

	return true;
}

bool Room::isEmpty() const
{
	for(int i = 0; i < this->_capacity; i++)
	{
		if(this->_clients[i] != NULL)
			return false;
	}

	return true;
}

bool Room::respawn(int index, point point)
{
	if(index < 0 || index > this->_capacity - 1)
		return false;

	if(this->_clients[index] == NULL)
		return false;

	Character& character = *this->_clients[index];
	character.position(point);
	character.respawnPoint(point);
	return true;
}

void Room::destroy()
{
	for (int i = 0; i < this->_capacity; i++)
	{
		if(this->_clients[i] == NULL)
			continue;

		this->_clients[i]->leaveRoom();
	}

	memset(&this->_clients, 0, sizeof(this->_clients));
}