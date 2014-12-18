#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Projectile.h"

//-----------------------------------------------------
// Character Class									
//-----------------------------------------------------
class Character
{
public:
	Character();				// Constructor
	virtual ~Character();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	virtual void Tick(double deltaTime)=0;
	virtual void Paint(MATRIX3X2 matTransform)=0;
	virtual void HandleCollision(HitRegion* hitRegionPtr)=0;
	virtual void HandleSemiCollision(HitRegion* hitRegionPtr)=0;
	virtual void AddGravity(DOUBLE2 force)=0;
	virtual int GivePoints()=0;


	virtual void SetPos(int x, int y)=0;
	virtual void SetPos(DOUBLE2 pos)=0;
	virtual void SetXPos(int x)=0;
	virtual void SetYPos(int y)=0;
	virtual DOUBLE2 GetPos()=0;

	virtual void SetVelocity(int x, int y)=0;
	virtual void SetVelocity(DOUBLE2 velocity)=0;
	virtual void SetXVelocity(int x)=0;
	virtual void SetYVelocity(int y)=0;
	virtual DOUBLE2 GetVelocity()=0;

	virtual HitRegion* GetHitRegion()=0;
	 
	virtual void SetDirection(bool isLeft)=0;
	virtual int GetDirection()=0;
	 
	virtual void SetState(int state)=0;
	
	virtual bool IsAlive()=0;
	virtual bool IsReadyToBeDeleted()=0;
	
	virtual bool SetTarget(DOUBLE2 targetPos)=0;

	virtual bool IsHostile()=0;
	virtual bool UsesBombs()=0;
	virtual int GetProjectileType()=0;
	 
	virtual void SubtractHealth(double health)=0;
	
	virtual bool ExecuteAttack()=0;
	virtual bool ExecuteSecondaryAttack(DOUBLE2 targetPos)=0;

	virtual void GetCameraBounds(DOUBLE2 pos, int width, int height)=0;



private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------


	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Character(const Character& yRef);									
	Character& operator=(const Character& yRef);	
};

 
