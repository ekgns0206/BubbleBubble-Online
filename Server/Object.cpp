#include "Object.h"

Object::Object(const point_f& position, float width, float height)
{
	this->_position			= position;
	this->_width			= width;
	this->_height			= height;

	this->_xMovable			= true;
	this->_yMovable			= true;
}

Object::~Object()
{
}

//
// collisionArea
//	�浹 ������ ��´�.
//	������Ʈ�� x, y ��ǥ�� left-bottom�� �������� �Ѵ�.
//
// Parameters
//	����
//
// Return
//	�浹 ������ ����
//
const rect_f Object::collisionRect() const
{
	return rect_f(this->_position.x - this->_width / 2.0f, this->_position.y - this->_height, this->_width, this->_height);
}

//
// move
//	������Ʈ�� �̵���Ų��.
//
// Parameters
//	x		�̵���ų x��
//	y		�̵���ų y��
//
// Return
//	����
//
void Object::move(float x, float y)
{
	this->position(this->xPos() + x, this->yPos() + y);
}

//
// position
//	���� ��ġ�� �����Ѵ�.
//
// Parameters
//	x		��ġ��ų x��
//	y		��ġ��ų y��
//
// Return
//	����
//
void Object::position(float x, float y)
{
	if(this->_xMovable)
		this->_position.x	= x;

	if(this->_yMovable)
		this->_position.y	= y;
}

void Object::position(const point_f& point)
{
	this->position(point.x, point.y);
}

//
// position
//	���� ��ġ�� ��´�.
//
// Parameters
//	����
//
// Return
//	���� ��ġ�� ����
//
const point_f Object::position() const
{
	return this->_position;
}

//
// xPos
//	������ x ��ġ�� ���Ѵ�.
//
// Parameters
//	����
//
// Return
//	���� x ��ġ�� ����
//
float Object::xPos() const
{
	return this->_position.x;
}

//
// yPos
//	������ y ��ġ�� ���Ѵ�.
//
// Parameters
//	����
//
// Return
//	���� y ��ġ�� ����
//
float Object::yPos() const
{
	return this->_position.y;
}

void Object::xMovable(bool value)
{
	this->_xMovable = value;
}

void Object::yMovable(bool value)
{
	this->_yMovable = value;
}

bool Object::xMovable() const
{
	return this->_xMovable;
}

bool Object::yMovable() const
{
	return this->_yMovable;
}
