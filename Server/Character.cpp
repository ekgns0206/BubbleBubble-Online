#include "Character.h"

Character::Character(int roomIndex, int id, StateChangeRoutine callback) : Life(id, roomIndex, point_f(), 15, 20, point_f(), Direction::Right, true, callback)
{
	this->_shootDelay			= 0.0f;
	this->_shotInterval			= 0.5f;
	this->_shotRange			= 150.0f;
	this->_shotSpeed			= 300.0f;
	this->_currentRespawnTime	= 0.0f;
	this->_respawning			= false;

	this->init();
}

Character::~Character()
{
}

//
// shotBubble
//	������ �߻��Ѵ�.
//
// Parameters
//	result		���� ���θ� ���޹��� ��
//
// Return
//	������ ����
//
Bubble Character::shotBubble(bool& result)
{
	rect_f						collisionArea = this->collisionRect();		// �浹 ����
	point_f						shotPosition;								// �߻� ��ġ
	shotPosition.x				= this->direction() == Direction::Left ? collisionArea.left : collisionArea.right;
	shotPosition.y				= this->yPos();

	// ������ �����Ѵ�.
	Bubble						bubble(this->roomIndex(), this->id(), shotPosition, this->_shotRange, this->direction() == Direction::Right ? true : false);

	// ĳ���Ͱ� ����ִ� ���°� �ƴ϶�� ����
	if (__super::isAlive() == false)
	{
		result = false;
	}
	// ���� �߻� �����̸� ��ä�� ��� ����
	else if(this->_shootDelay < this->_shotInterval)
	{
		result					= false;
	}
	// ������ �߻��Ѵ�.
	else
	{
		this->_shootDelay		= 0.0f;
		result					= true;

		if(this->_callback != NULL)
		{
			Json::Value json;
			json["x"] = this->xPos();
			json["y"] = this->yPos();
			this->_callback(this, "shoot", json);
		}
	}
	return bubble;
}

int Character::life() const
{
	return this->_life;
}

bool Character::respawnable() const
{
	return this->_life > 0;
}

bool Character::isAlive() const
{
	return __super::isAlive();
}

void Character::isAlive(bool value)
{
	__super::isAlive(value);
	if(value == false && this->_respawning == false)
	{
		this->_life--;

		if(this->respawnable())
			this->_respawning		= true;
	}
}

bool Character::isRespawning() const
{
	return this->_currentRespawnTime != 0.0f;
}

void Character::respawnPoint(const point& point)
{
	this->_respawnPoint = point;
}

const point& Character::respawnPoint() const
{
	return this->_respawnPoint;
}

void Character::init()
{
	this->stop();
	this->_life					= LIFE;
}

//
// update
//	ĳ������ ������ ������Ʈ�Ѵ�.
//
// Parameters
//	elapsedTime			���� �ð�
//
// Return
//	����
//
void Character::update(float elapsedTime)
{
	__super::update(elapsedTime);


	// ���� ������ �ð��� ������Ų��.
	this->_shootDelay			+= elapsedTime;


	// �׾��ִ� ���¶�� ������ �ð��� ä���.
	if(this->_respawning)
	{
		this->_currentRespawnTime		+= elapsedTime;

		// ������ �ð��� �� ä���ٸ� ��Ȱ��Ų��.
		if(this->_currentRespawnTime > RESPAWN_TIME)
		{
			this->_respawning			= false;
			this->_currentRespawnTime	= 0;
			this->xMovable(true);
			this->yMovable(true);
			this->position(this->_respawnPoint);
			this->isAlive(true);

			if(this->_callback != NULL)
			{
				Json::Value json;
				json["x"] = this->_respawnPoint.x;
				json["y"] = this->_respawnPoint.y;
				this->_callback(this, "respawn", json);
			}
		}
	}
}