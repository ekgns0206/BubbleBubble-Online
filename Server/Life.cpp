#include "Life.h"

static const float GRAVITY = 350.0f;		// �߷� ��
static const float GRAVITY_MAX = 120.0f;	// �߷� �ִ� ��
static const float JUMP_VEL = 160.0f;		// ���� ��

Life::Life(int id, int roomIndex, const point_f& position, int width, int height, const point_f& velocity, Direction direction, bool isGravity, StateChangeRoutine callback) : Object(position, width, height)
{
	this->_id = id;
	this->_roomIndex = roomIndex;
	this->_speed = 60;
	this->_direction = direction;
	this->_jumping = false;
	this->_velocity = velocity;
	this->_isGravity = isGravity;
	this->_isAlive = true;
	this->_callback = callback;
	this->_onGround = false;
}

Life::~Life()
{}

int Life::id() const
{
	return this->_id;
}

//
// stop
//	�������� �����.
//
// Parameters
//	����
//
// Return
//	����
//
void Life::stop()
{
	this->velocity(0, this->_velocity.y);
}

//
// velocity
//	�ӵ��� �����Ѵ�.
//
// Parameters
//	x		�ӵ� x��
//	y		�ӵ� y��
//
// Return
//	����
//
void Life::velocity(float x, float y)
{
	this->_velocity.x = x;
	this->_velocity.y = y;

	auto moving = this->_velocity.x != 0.0f;
	auto direction = x > 0.0f ? Life::Direction::Right : Life::Direction::Left;
	if(x == 0.0f)
		direction = this->_direction;


	if(this->_callback != NULL && (this->_moving != moving || this->_direction != direction))
	{
		Json::Value json;
		json["state"] = moving;
		json["jumping"] = this->_jumping;
		json["direction"] = direction == Life::Direction::Left ? "left" : "right";
		this->_callback(this, "move", json);
	}

	this->_moving = moving;
	this->_direction = direction;
}

//
// velocity
//	�ӵ��� ��´�.
//
// Parameters
//	����
//
// Return
//	�ӵ����� ����.
//
const point_f & Life::velocity() const
{
	return this->_velocity;
}

bool Life::onGround() const
{
	return this->_onGround;
}

void Life::onGround(bool value)
{
	if(this->_onGround == value)
		return;

	if(value && this->isJumpingUp())
		return;

	this->_onGround = value;
	if(this->_onGround)
	{
		this->_jumping = false;
		this->_velocity.y = 0.0f;

		if(this->_callback != NULL)
		{
			Json::Value json;
			json["state"] = false;
			json["direction"] = "down";
			json["moving"] = this->_moving;
			this->_callback(this, "jump", json);
		}
	}
}

//
// isJumping
//	�������������� �Ǻ��Ѵ�.
//
// Parameters
//	����
//
// Return
//	�������̶�� true, �ƴ϶�� false�� ����
//
bool Life::isJumping() const
{
	return this->_jumping;
}

//
// isJumpingUp
//	���� �����ϴ� �������� �Ǻ��Ѵ�.
//
// Parameters
//	����
//
// Return
//	���� �������̶�� true, �ƴ϶�� false�� ����
//
bool Life::isJumpingUp() const
{
	if (this->_jumping && this->_velocity.y > 0.0f)		// ���� : ���������̸鼭 y�� �ӵ��� 0���� Ŀ�� �Ѵ�.
		return true;

	return false;
}

//
// isJumpingDown
//	�Ʒ��� �����ϴ� �������� �Ǻ��Ѵ�.
//
// Parameters
//	����
//
// Return
//	�Ʒ��� �������̶�� true, �ƴ϶�� false�� ����
//
bool Life::isJumpingDown() const
{
	if (this->_jumping && this->_velocity.y < 0.0f)		// ���� : ���������̸鼭 y�� �ӵ��� 0���� �۾ƾ� �Ѵ�.
		return true;

	return false;
}

//
// direction
//	�ٶ󺸴� ������ ��´�.
//
// Parameters
//	����
//
// Return
//	�ٶ󺸴� ������ �������̶�� Direction::Right
//	�����̶�� Direction::Left�� ����
//
Life::Direction Life::direction() const
{
	return this->_direction;
}

//
// speed
//	���ǵ带 ��´�.
//
// Parameters
//	����
//
// Return
//	���ǵ带 ����
//
int Life::speed() const
{
	return this->_speed;
}

//
// moveX
//	x������ �̵��Ѵ�.
//
// Parameters
//	direction		�̵��� ����
//
// Return
//	����
//
void Life::moveX(Life::Direction direction)
{
	this->velocity(this->_speed * (direction == Direction::Left ? -1 : 1), this->_velocity.y);
}

//
// isGraivty
//	�߷��� ������ �޴��� �Ǻ��Ѵ�.
//
// Parameters
//	����
//
// Return
//	�߷��� ������ �޴´ٸ� true, �ƴ϶�� false�� ����
//
bool Life::isGravity() const
{
	return this->_isGravity;
}

//
// isGravity
//	�߷��� ������ ������ �����Ѵ�.
//
// Parameters
//	value		�߷��� ������ �ް� �Ϸ��� true, �ƴ϶�� false
//
// Return
//	����
//
void Life::isGravity(bool value)
{
	this->_isGravity		= value;
}

//
// isAlive
//	����ִ��� ���θ� �Ǻ��Ѵ�.
//
// Parameters
//	����
//
// Return
//	����ִٸ� true, �ƴ϶�� false�� ����
//
bool Life::isAlive() const
{
	return this->_isAlive;
}

//
// isAlive
//	����ִ��� ���θ� �����Ѵ�.
//
// Parameters
//	value		����ְ� �Ϸ��� true, �ƴ϶�� false
//
// Return
//	����
//
void Life::isAlive(bool value)
{
	if(this->_isAlive == value)
		return;

	this->_isAlive			= value;
	if(this->_isAlive == false)
		this->velocity(0, 0);

	if(this->_callback != NULL)
	{
		Json::Value json;
		json["value"] = this->_isAlive;
		this->_callback(this, "alive", json);
	}
}

int Life::roomIndex() const
{
	return this->_roomIndex;
}

void Life::roomIndex(int index)
{
	this->_roomIndex = index;
}

bool Life::containsTile(const rect_f& collisionTileRect, const char* type, bool left, bool top, bool right, bool bottom)
{
	bool inner = strcmp(type, "inner") == 0;
	bool containsTileLeft = this->collisionRect().right - (inner ? 1e-4 : -1e-4) > collisionTileRect.left;
	bool containsTileRight = this->collisionRect().left + (inner ? 1e-4 : -1e-4) < collisionTileRect.right;
	bool containsTileTop = this->collisionRect().bottom - (inner ? 1e-4 : -1e-4) > collisionTileRect.top;
	bool containsTileBottom = this->collisionRect().top + (inner ? 1e-4 : -1e-4) < collisionTileRect.bottom;

	if(left && containsTileLeft == false)
		return false;

	if(right && containsTileRight == false)
		return false;

	if(top && containsTileTop == false)
		return false;

	if(bottom && containsTileBottom == false)
		return false;

	return true;
}

bool Life::comparePosition(const rect_f & collisionTileRect, const char * type, bool left, bool top, bool right, bool bottom)
{
	bool greater = strcmp(type, "greater") == 0;
	bool compareLeft = greater ? this->collisionRect().left > collisionTileRect.left : this->collisionRect().left < collisionTileRect.left;
	bool compareRight = greater ? this->collisionRect().right > collisionTileRect.right : this->collisionRect().right < collisionTileRect.right;
	bool compareTop = greater ? this->collisionRect().top > collisionTileRect.top : this->collisionRect().top < collisionTileRect.top;
	bool compareBottom = greater ? this->collisionRect().bottom > collisionTileRect.bottom : this->collisionRect().bottom < collisionTileRect.bottom;
	
	if(left && compareLeft == false)
		return false;

	if(right && compareRight == false)
		return false;

	if(top && compareTop == false)
		return false;

	if(bottom && compareBottom == false)
		return false;

	return true;
}

//
// update
//	������Ʈ�� ���¸� ������Ʈ�Ѵ�.
//
// Parameters
//	elapsedTime		����ð�
//
// Return
//	����
//
void Life::update(float elapsedTime)
{
	// ������� �ʴٸ� ����
	if(this->_isAlive == false)
		return;

	// �߷��� ������ �޴´ٸ� �߷¿� ���� �ӵ��� �����Ѵ�.
	if(this->_isGravity)
	{
		auto velocity = this->_velocity;

		if(this->_onGround == false)
		{
			auto jumping = true;

			velocity.y = max(velocity.y - GRAVITY * elapsedTime, -GRAVITY_MAX);
			
			if(this->_callback != NULL)
			{
				bool fall = this->_jumping != jumping;
				bool reverse = (velocity.y <= 0 && this->_velocity.y >= 0);
				if(fall || reverse)
				{
					Json::Value json;
					json["direction"] = "down";
					json["state"] = true;

					this->_callback(this, "jump", json);
				}
			}

			this->_jumping = jumping;
		}

		this->_velocity = velocity;
	}
	this->move(this->_velocity.x * elapsedTime, -this->_velocity.y * elapsedTime);
}

//
// jump
//	�����Ѵ�.
//
// Parameters
//	velocity		������ �ӵ�
//
// Return
//	������ true, ���н� false�� ����
//
bool Life::jump(float velocity)
{
	// ���� �������¶�� ����
	if(this->_onGround == false)
		return false;

	// �ӵ��� �����Ѵ�.
	this->_velocity.y = JUMP_VEL;
	this->_jumping = true;

	// �������� �ö� ��
	if(this->_callback != NULL)
	{
		Json::Value json;
		json["state"] = true;
		json["direction"] = "up";
		this->_callback(this, "jump", json);
	}
	return true;
}