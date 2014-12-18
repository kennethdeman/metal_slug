#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------
// Projectile Class									
//-----------------------------------------------------
class Projectile
{
public:
	Projectile();				// Constructor
	virtual ~Projectile();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------

	virtual void Tick(double deltaTime)=0;
	virtual void Paint(MATRIX3X2 matTransform)=0;

	virtual void HandleCollision(HitRegion* hitRegionPtr, double deltaTime)=0;

	virtual void AddGravity(DOUBLE2 force)=0;

	virtual bool IsDestroyed()=0;

	virtual DOUBLE2 GetPos()=0;

	virtual bool CheckHit(HitRegion* hitTargetPtr)=0;
	
	virtual bool IsFromEnemy()=0;

	static const int NOTHING=0;
	static const int BULLET=1;
	static const int GRENADE=2;
	static const int BOMB=3;
	static const int ROCKET=4;

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------


	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Projectile(const Projectile& yRef);									
	Projectile& operator=(const Projectile& yRef);	
};

 
