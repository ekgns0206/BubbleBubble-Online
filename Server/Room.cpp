#include "Room.h"

Room::Room(const std::string& name, Client* priest, int capacity) : _name(name), _priest(priest), _capacity(capacity), _level(-1)
{
	static int			ROOM_INDEX = 0;		// ���� �����ɶ����� �ο��� id

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
//	���� ���� id�� ��´�.
//
// Parameters
//	����
//
// Return
//	���� ���� id�� ����
//
int Room::id() const
{
	return this->_index;
}

//
// name
//	���� �̸��� ��´�.
//
// Parameters
//	����
//
// Return
//	���� �̸��� ��´�.
//
const std::string & Room::name() const
{
	return this->_name;
}

//
// priest
//	������ ������ ��´�.
//
// Parameters
//	����
//
// Return
//	������ ����
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
//	�ִ� ���� �ο��� ��´�.
//
// Parameters
//	����
//
// Return
//	���ӹ��� �ִ� ���� �ο��� ����
//
int Room::capacity() const
{
	return this->_capacity;
}

//
// state
//	���ӹ��� ���� ���¸� ��´�.
//
// Parameters
//	����
//
// Return
//	���ӹ��� ���� ���¸� ����
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
//	���ӹ濡 �����ϴ� Ŭ���̾�Ʈ ������ ��´�.
//
// Parameters
//	����
//
// Return
//	���ӹ濡 �����ϴ� Ŭ���̾�Ʈ ������ ����
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
//	���ӹ濡 �����ϴ� ������ ������ ��´�.
//
// Parameterse
//	����
//
// Return
//	���ӹ濡 �����ϴ� ������ ������ ����
//
std::vector<Enemy*>& Room::enemies()
{
	return this->_enemies;
}

//
// enter
//	���ӹ濡 Ŭ���̾�Ʈ�� �߰��Ѵ�.
//
// Parameters
//	client		�߰��� Ŭ���̾�Ʈ
//
// Return
//	������ true, ���н� false�� ����
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
//	���ӹ濡�� Ŭ���̾�Ʈ�� ��������.
//
// Parameters
//	client		������ Ŭ���̾�Ʈ
//
// Return
//	������ true,  ���н� false�� ����
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
//	���ӹ��� ���� �������� ������ ��´�.
//
// Parameters
//	����
//
// Return
//	���ӹ��� ���� �������� ������ ����
//
int Room::level() const
{
	return this->_level;
}

//
// init
//	������ �����ϱ� �� �ʱ�ȭ�Ѵ�.
//
// Parameters
//	����
//
// Return
//	����
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
//	���� �������� ������ �ٲ۴�.
//
// Parameters
//	����
//
// Return
//	�ִ� ������ ������ ��� false, ���� ������ ��� true�� ����
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
//	���� ���Ǵ� ���� �̸��� ��´�.
//
// Parameters
//	����
//
// Return
//	���� ���Ǵ� ���� �̸�
//
std::string Room::mapName() const
{
	char			buffer[256]	= {0,};
	sprintf(buffer, "map%03d.pbm", this->_level);

	return std::string(buffer);
}

//
// addBubble
//	������ �߰��Ѵ�.
//
// Parameters
//	bubble		�߰��� ����
//
// Return
//	����
//
void Room::addBubble(const Bubble& bubble)
{
	this->_bubbles.push_back(bubble);
}

//
// bubbles
//	���� �����ϴ� ������ ������ ��´�.
//
// Parameters
//	����
//
// Return
//	������� ������ ����
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