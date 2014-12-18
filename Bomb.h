#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Projectile.h"

//-----------------------------------------------------
// Bomb Class									
//-----------------------------------------------------
class Bomb : public Projectile
{
public:
	Bomb(int x, int y);				// Constructor
	Bomb(int x, int y, bool isEnemyGrenade);
	virtual ~Bomb();		// Destructor

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
	static const int CENTER_X = BOMB_WIDTH/2;
	static const int CENTER_Y = BOMB_HEIGHT/2;

	static Bitmap* m_BmpGrenadePtr, *m_BmpExplosionPtr;
	HitRegion* m_HitBombPtr, *m_HitExplosionPtr;
	DOUBLE2 m_Pos, m_Velocity;
	double m_SlowDownAnimations;

	int m_CurrentState, m_TickCount;

	static const int STATE_NORMAL=0;
	static const int STATE_EXPLODING=1;
	static const int STATE_EXPLODING2=2;
	static const int STATE_DESTROYED=3;
	
	//This constant value is used to reset the doubles that take care of slowing down the animations
	//according to deltaTime
	static const int RESET_SLOW_DOWN=-10;

	static const int EXPLOSION_WIDTH=35;
	static const int EXPLOSION_HEIGHT_END=64;
	static const int EXPLOSION_FRAMES=5;
	static const int EXPLOSION_ANIMATION_SPEED=100;
	static const int EXPLOSION_OFFSET_X=-17;

	static const int EXPLOSION2_WIDTH=35;
	static const int EXPLOSION2_HEIGHT_START=64;
	static const int EXPLOSION2_HEIGHT=35;
	static const int EXPLOSION2_FRAMES=4;
	static const int EXPLOSION2_ANIMATION_SPEED=100;
	static const int EXPLOSION2_OFFSET_X=-17;


	bool m_IsFromEnemy;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Bomb(const Bomb& yRef);									
	Bomb& operator=(const Bomb& yRef);	
};

 
