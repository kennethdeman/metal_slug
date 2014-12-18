//---------------------------
// Includes
//---------------------------
#include "PowerUpList.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
PowerUpList::PowerUpList()
{
	for (int count=0; count < MAX_POWERUPS; ++count)
	{
		m_PowerUpPtrArr[count]=nullptr;
	}
}

PowerUpList::~PowerUpList()
{
	RemoveAll();
}

//---------------------------
// Eigen methoden
//---------------------------

bool PowerUpList::Add(PowerUp* powerUpPtr)
{
	for (int count=0; count < MAX_POWERUPS; ++count)
	{
		if (m_PowerUpPtrArr[count] == nullptr)
		{
			m_PowerUpPtrArr[count]=powerUpPtr;
			return true;
		}
	}
	return false;
}

bool PowerUpList::Remove(PowerUp* powerUpPtr)
{
	for (int count=0; count < MAX_POWERUPS; ++count)
	{
		if (m_PowerUpPtrArr[count] == powerUpPtr)
		{
			delete m_PowerUpPtrArr[count];
			m_PowerUpPtrArr[count] = nullptr; 
			return true;
		}
	}
	return false;
}

void PowerUpList::RemoveAll()
{
	for (int count=0; count < MAX_POWERUPS; ++count)
	{
		delete m_PowerUpPtrArr[count];
		m_PowerUpPtrArr[count] = nullptr; 
	}
}


void PowerUpList::Tick(double deltaTime)
{
	for (int count=0; count < MAX_POWERUPS; ++count)
	{
		if (m_PowerUpPtrArr[count] != nullptr)
		{
			m_PowerUpPtrArr[count]->Tick(deltaTime);
		}
			
	}
}

void PowerUpList::Paint(MATRIX3X2 matTransform)
{
	for (int count=0; count < MAX_POWERUPS; ++count)
	{
		if (m_PowerUpPtrArr[count] != nullptr)
		{
			m_PowerUpPtrArr[count]->Paint(matTransform);
		}

			
	}
}

int PowerUpList::ScoreGainedByPowerUps(HitRegion* hitPlayerPtr)
{
	for (int count=0; count < MAX_POWERUPS; ++count)
	{
		if (m_PowerUpPtrArr[count] != nullptr)
		{
			int points=m_PowerUpPtrArr[count]->GivePoints();
			if (points>0) 
			{
				if (m_PowerUpPtrArr[count]->CollisionWithPlayer(hitPlayerPtr)==true)
				{
					
					Remove(m_PowerUpPtrArr[count]);

					return points;
				}
			}
		}
	}
	
	return 0;
}

int PowerUpList::AmmunitionGainedByPowerUps(HitRegion* hitPlayerPtr)
{
	for (int count=0; count < MAX_POWERUPS; ++count)
	{
		if (m_PowerUpPtrArr[count] != nullptr)
		{
			int ammunition=m_PowerUpPtrArr[count]->GiveBombAmmunition();
			if (ammunition>0) 
			{	
				if (m_PowerUpPtrArr[count]->CollisionWithPlayer(hitPlayerPtr)==true)
				{				
					
					Remove(m_PowerUpPtrArr[count]);
					
					return ammunition;
				}
			}
		}
	}
	
	return 0;
}
