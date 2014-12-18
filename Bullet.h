#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Projectile.h"

//-----------------------------------------------------
// Bullet Class									
//-----------------------------------------------------
class Bullet : public Projectile
{
public:
	Bullet(int x, int y, double angle);				// Constructor
	virtual ~Bullet();		// Destructor

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

	static const int BULLET_WIDTH=26, BULLET_HEIGHT=26;
	static const int CENTER_X = BULLET_WIDTH/2;
	static const int CENTER_Y = BULLET_HEIGHT/2;
private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static Bitmap* m_BmpBulletPtr;


	DOUBLE2 m_Pos, m_Velocity;
	double m_Angle, m_Speed;
	int m_CurrentState;

	static const int STATE_NORMAL=0;
	static const int STATE_DESTROYED=1;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Bullet(const Bullet& yRef);									
	Bullet& operator=(const Bullet& yRef);	
};

 
