#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------
// PowerUp Class									
//-----------------------------------------------------
class PowerUp
{
public:
	PowerUp();	
	virtual ~PowerUp();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------

	virtual void Tick(double deltaTime)=0;
	virtual void Paint(MATRIX3X2 matTransform)=0;
	virtual bool CollisionWithPlayer(HitRegion* hitPlayerPtr)=0;
	
	virtual void SetPos(int x, int y)=0;
	virtual void SetPos(DOUBLE2 pos)=0;
	virtual void SetXPos(int x)=0;
	virtual void SetYPos(int y)=0;
	virtual DOUBLE2 GetPos()=0;

	virtual HitRegion* GetHitRegion()=0;
	
	virtual int GivePoints()=0;
	virtual int GiveBombAmmunition()=0;

protected:
	
	static const int STATE_IDLE=0;
	static const int STATE_POINTSGAINED=1;

private:

	PowerUp(const PowerUp& yRef);									
	PowerUp& operator=(const PowerUp& yRef);	
};

 
