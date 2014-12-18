#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Projectile.h"

//-----------------------------------------------------
// Rocket Class									
//-----------------------------------------------------
class Rocket : public Projectile
{
public:
	Rocket(int x, int y);				// Constructor
	Rocket(int x, int y, bool isEnemyGrenade);
	virtual ~Rocket();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);

	void HandleCollision(HitRegion* hitRegionPtr, double deltaTime);

	void AddGravity(DOUBLE2 force);

	bool IsDestroyed();

	DOUBLE2 GetPos();

	bool CheckHit(HitRegion* hitTargetPtr);
	
	bool IsFromEnemy();

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	static const int BOMB_FRAME_X=51, BOMB_FRAME_Y=30;
	static const int BOMB_WIDTH=14, BOMB_HEIGHT=27;


	static Bitmap* m_BmpProjectilePtr, *m_BmpExplosionPtr;
	HitRegion* m_HitRocketPtr;
	DOUBLE2 m_Pos, m_Velocity;
	double m_SlowDownAnimations;

	int m_CurrentState, m_TickCount;

	static const int STATE_NORMAL=0;
	static const int STATE_EXPLODING=1;
	static const int STATE_DESTROYED=2;
	
	//This constant value is used to reset the doubles that take care of slowing down the animations
	//according to deltaTime
	static const int RESET_SLOW_DOWN=-10;

	static const int ROCKET_WIDTH=65;
	static const int ROCKET_HEIGHT_END=36;
	static const int ROCKET_FRAMES=6;
	static const int ROCKET_ANIMATION_SPEED=50;
	static const int ROCKET_OFFSET_X=0;

	static const int CENTER_X = 7;
	static const int CENTER_Y = ROCKET_HEIGHT_END/2;

	static const int EXPLOSION_WIDTH=35;
	static const int EXPLOSION_HEIGHT_START=64;
	static const int EXPLOSION_HEIGHT=35;
	static const int EXPLOSION_FRAMES=4;
	static const int EXPLOSION_ANIMATION_SPEED=100;
	static const int EXPLOSION_OFFSET_X=-17;

	static const int ROCKET_SPEED=300;


	bool m_IsFromEnemy;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Rocket(const Rocket& yRef);									
	Rocket& operator=(const Rocket& yRef);	
};

 
