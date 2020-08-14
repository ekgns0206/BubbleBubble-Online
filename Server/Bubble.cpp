#include "Bubble.h"

Bubble::Bubble(int roomIndex, int owner, const point_f& position, float range, bool isRight) : Life(-1, roomIndex, position, 20, 20, point_f(200.0f * (isRight ? 1 : -1), 0.0f), Life::Direction::Right, false), _createPoint(position)
{
	this->_owner			= owner;
	this->_range			= range;
	this->_currentRange		= 0.0f;
	this->_shotting			= true;
	this->_isRight			= isRight;
	this->_prisonEnemy		= NULL;
}

Bubble::~Bubble()
{}

int Bubble::owner() const
{
	return this->_owner;
}

//
// update
//	������ ���¸� ������Ʈ�Ѵ�.
//
// Paramters
//	elapsedTime		���� �ð�
//
// Return
//	����
//
void Bubble::update(float elapsedTime)
{
	__super::update(elapsedTime);

	// �߻���¶��
	if(this->isShooting())
	{
		if(abs(this->xPos() - this->_createPoint.x) > this->_range)
			this->isShooting(false);
	}
	// �ƴ϶��
	else
	{
		// ���� ������ �ִٸ� ��ġ�� ������ �����.
		if(this->_prisonEnemy != NULL)
			this->_prisonEnemy->position(this->xPos(), this->collisionRect().bottom);
	}
}

//
// prison
//	������ ���д�.
//
// Parametteres
//	enemy		���� ����
//
// Return
//	������ true, ���н� false
//
bool Bubble::prison(Enemy * enemy)
{
	if(this->_prisonEnemy != NULL)
		return false;

	// ������ ���û��°� �ƴ϶�� ����
	if(this->_shotting == false)
		return false;

	// ������ �̹� ���� ���¶�� ����
	if(enemy->isPrison())
		return false;

	this->_prisonEnemy			= enemy;
	this->isShooting(false);
	enemy->isPrison(true);
	return true;
}

//
// prisonEnemy
//	���� ���� ��´�.
//
// Parameters
//	����
//
// Return
//	���� ����. ���ٸ� NULL
//
Enemy * Bubble::prisonEnemy() const
{
	return this->_prisonEnemy;
}

//
// unprison
//	���� ������ Ǯ���ش�.
//
// Parameters
//	����
//
// Return
//	�����ִ� ����. ���ٸ� NULL�� ����
//
Enemy* Bubble::unprison()
{
	Enemy*				ret		= this->_prisonEnemy;
	this->_prisonEnemy			= NULL;

	return ret;
}

//
// isShooting
//	���� �������� �Ǻ��Ѵ�.
//
// Parameters
//	����
//
// Return
//	���� ���¶�� true, �ƴϸ� false�� ����
//
bool Bubble::isShooting() const
{
	return this->_shotting;
}

void Bubble::isShooting(bool value)
{
	this->_shotting				= value;
	
	if(value == false)
		this->velocity(0.0f, 10.0f);
}