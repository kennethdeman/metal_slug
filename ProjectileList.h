#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Projectile.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Bomb.h"
#include "Rocket.h"

//-----------------------------------------------------
// ProjectileList Class									
//-----------------------------------------------------
class ProjectileList
{
public:
	ProjectileList();				// Constructor
	virtual ~ProjectileList();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	bool Add(Projectile* projectilePtr);
	bool Remove(Projectile* projectilePtr);
	void RemoveAll();
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void CheckOutOfBounds(DOUBLE2 pos, int gameWidth, int gameHeight);
	void HandleCollision(HitRegion* hitRegionPtr, double deltaTime);
	void AddGravity(DOUBLE2 force);
	bool CheckHit(HitRegion* hitTargetPtr, bool isEnemy);

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static const int MAX_PROJECTILES=100;
	Projectile* m_ProjectilePtrArr[MAX_PROJECTILES];

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ProjectileList(const ProjectileList& yRef);									
	ProjectileList& operator=(const ProjectileList& yRef);	
};

 
