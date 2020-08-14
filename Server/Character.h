#ifndef __CHARACTER_H__
#define	__CHARACTER_H__

#include "Life.h"
#include "Bubble.h"

#define	RESPAWN_TIME	3
#define	LIFE			3

class Character : public Life
{
private:
	float					_shootDelay, _shotInterval;
	float					_shotRange, _shotSpeed;
	float					_currentRespawnTime;
	int						_life;
	bool					_respawning;
	point					_respawnPoint;

public:
	Character(int roomIndex, int id, StateChangeRoutine callback = NULL);
	~Character();

public:
	Bubble					shotBubble(bool& result);
	int						life() const;
	bool					respawnable() const;
	bool					isAlive() const;
	void					isAlive(bool value);
	bool					isRespawning() const;
	void					respawnPoint(const point& point);
	const point&			respawnPoint() const;

	void					init();

public:
	void					update(float elapsedTime);
};

#endif // !__CHARACTER_H__