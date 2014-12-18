#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "PowerUp.h"


//-----------------------------------------------------
// PowerUpLetter Class									
//-----------------------------------------------------
class PowerUpLetter : public PowerUp 
{
public:
	PowerUpLetter(int x, int y);				// Constructor
	virtual ~PowerUpLetter();		// Destructor

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
	static Bitmap* m_BmpPowerUpLetterSpriteSheetPtr;
	HitRegion *m_HitRegionPtr;
	DOUBLE2 m_Pos;
	
	int m_TickCount, m_CurrentState;
	double m_SlowDownAnimations;

	
	static const int RESET_SLOW_DOWN=-10;

	static const int WIDTH=24, HEIGHTSTART=56, HEIGHT=24;
	static const int FRAMES=11;
	static const int ANIMATION_SPEED=100;

	static const int POINTS=500;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PowerUpLetter(const PowerUpLetter& yRef);									
	PowerUpLetter& operator=(const PowerUpLetter& yRef);	
};

 
