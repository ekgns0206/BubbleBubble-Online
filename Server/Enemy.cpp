#include "Enemy.h"

int Enemy::autoIncrementId = 0;

Enemy::Enemy(int roomIndex, const std::string& name, const point_f& position, StateChangeRoutine callback) : Life(Enemy::autoIncrementId++, roomIndex, position, 15, 20, point_f(20, 0), Direction::Right, true, callback)
{
	this->_name				= name;
	this->_isPrison			= false;
}

Enemy::Enemy(int roomIndex, const Enemy& enemy, StateChangeRoutine callback) : Enemy(roomIndex, enemy._name, enemy.position(), callback)
{ }

Enemy::~Enemy()
{
}

const std::string& Enemy::name() const
{
	return this->_name;
}

//
// update
//	������ ���¸� ������Ʈ�Ѵ�.
//
// Parameters
//	elapsedTime		���� �ð�
//	
// Return
//	����
//
void Enemy::update(float elapsedTime)
{
	__super::update(elapsedTime);
}

//
// isPrison
//	�����ִ����� �Ǻ��Ѵ�.
//
// Parameters
//	����
//
// Return
//	�����ִٸ� true, �ƴϸ� false�� ����
//
bool Enemy::isPrison() const
{
	return this->_isPrison;
}

//
// isPrison
//	���� ���¸� �����Ѵ�.
//
// Parameters
//	value		���� ���·� ������� true, �����Ϸ��� false
//
// Return
//	����
//
void Enemy::isPrison(bool value)
{
	if(this->_isPrison == value)
		return;

	this->_isPrison			= value;
	if(this->_callback != NULL)
	{
		Json::Value json;
		json["value"] = value;
		json["x"] = this->xPos();
		json["y"] = this->yPos();
		this->_callback(this, "prison", json);
	}
}

//
// reverseDirection
//	������ ��ȯ�Ѵ�.
//
// Parameters
//	����
//
// Return
//	����
//
void Enemy::reverseDirection()
{
	this->velocity(this->velocity().x * -1, this->velocity().y);
}