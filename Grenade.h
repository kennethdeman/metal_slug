#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Projectile.h"

//-----------------------------------------------------
// Grenade Class									
//-----------------------------------------------------
class Grenade : public Projectile
{
public:
	Grenade(int x, int y, bool directionLeft);				// Constructor
	Grenade(int x, int y, bool directionLeft, bool isEnemyGrenade);
	virtual ~Grenade();		// Destructor

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

	static const int BULLET_FRAME_X=52, BULLET_FRAME_Y=3;
	static const int BULLET_WIDTH=12, BULLET_HEIGHT=19;
	static const int CENTER_X = BULLET_WIDTH/2;
	static const int CENTER_Y = BULLET_HEIGHT/2;

	static Bitmap* m_BmpGrenadePtr, *m_BmpExplosionPtr;
	HitRegion* m_HitGrenadePtr, *m_HitExplosionPtr;
	DOUBLE2 m_Pos, m_Velocity;
	double m_Angle, m_SlowDownAnimations, m_ExplosionTimer;

	int m_AmountOfBounces, m_CurrentState, m_TickCount;

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


	bool m_IsDirectionLeft, m_IsFromEnemy;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Grenade(const Grenade& yRef);									
	Grenade& operator=(const Grenade& yRef);	
};

 
