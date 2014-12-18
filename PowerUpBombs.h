#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "PowerUp.h"

//-----------------------------------------------------
// PowerUpBombs Class									
//-----------------------------------------------------
class PowerUpBombs : public PowerUp
{
public:
	PowerUpBombs(int x, int y);				// Constructor
	virtual ~PowerUpBombs();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	bool CollisionWithPlayer(HitRegion* hitPlayerPtr);
	
	void SetPos(int x, int y);
	void SetPos(DOUBLE2 pos);
	void SetXPos(int x);
	void SetYPos(int y);
	DOUBLE2 GetPos();

	HitRegion* GetHitRegion();

	int GivePoints();
	int GiveBombAmmunition();

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static Bitmap* m_BmpPowerUpBombsSpriteSheetPtr;	
	HitRegion *m_HitRegionPtr;
	
	DOUBLE2 m_Pos;
	 
	int m_TickCount, m_CurrentState;
	double m_SlowDownAnimations;

	
	//This constant value is used to reset the doubles that take care of slowing down the animations
	//according to deltaTime
	static const int RESET_SLOW_DOWN=-10;

	static const int WIDTH=32, HEIGHT=32;
	static const int FRAMES=7;
	static const int ANIMATION_SPEED=100;

	static const int BOMB_AMMUNITION=10;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PowerUpBombs(const PowerUpBombs& yRef);									
	PowerUpBombs& operator=(const PowerUpBombs& yRef);	
};

 
