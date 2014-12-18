#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "PowerUp.h"
#include "PowerUpBombs.h"
#include "PowerUpLetter.h"
#include "PowerUpPig.h"

//-----------------------------------------------------
// PowerUpList Class									
//-----------------------------------------------------
class PowerUpList
{
public:
	PowerUpList();				// Constructor
	virtual ~PowerUpList();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------
	bool Add(PowerUp* powerUpPtr);
	bool Remove(PowerUp* powerUpPtr);
	void RemoveAll();
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	int ScoreGainedByPowerUps(HitRegion* hitPlayerPtr);
	int AmmunitionGainedByPowerUps(HitRegion* hitPlayerPtr);

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static const int MAX_POWERUPS =10;
	PowerUp* m_PowerUpPtrArr[MAX_POWERUPS];
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PowerUpList(const PowerUpList& yRef);									
	PowerUpList& operator=(const PowerUpList& yRef);	
};


 
