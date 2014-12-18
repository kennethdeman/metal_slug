//---------------------------
// Includes
//---------------------------
#include "ProjectileList.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
ProjectileList::ProjectileList()
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		m_ProjectilePtrArr[count]=nullptr;
	}
}

ProjectileList::~ProjectileList()
{
	RemoveAll();
}

//---------------------------
// Own methods
//---------------------------

bool ProjectileList::Add(Projectile* projectilePtr)
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		if (m_ProjectilePtrArr[count] == nullptr)
		{
			m_ProjectilePtrArr[count]=projectilePtr;
			return true;
		}
	}
	return false;
}

bool ProjectileList::Remove(Projectile* projectilePtr)
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		if (m_ProjectilePtrArr[count] == projectilePtr)
		{
			delete m_ProjectilePtrArr[count];
			m_ProjectilePtrArr[count] = nullptr; 
			return true;
		}
	}
	return false;
}

void ProjectileList::RemoveAll()
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		delete m_ProjectilePtrArr[count];
		m_ProjectilePtrArr[count] = nullptr; 
	}
}

void ProjectileList::Tick(double deltaTime)
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		if (m_ProjectilePtrArr[count] != nullptr)
		{
			m_ProjectilePtrArr[count]->Tick(deltaTime);

			if (m_ProjectilePtrArr[count]->IsDestroyed()==true)
			{
				Remove(m_ProjectilePtrArr[count]);
			}

		}
			
	}
}

void ProjectileList::Paint(MATRIX3X2 matTransform)
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		if (m_ProjectilePtrArr[count] != nullptr)
		{
			m_ProjectilePtrArr[count]->Paint(matTransform);
		}
			
	}
}

void ProjectileList::CheckOutOfBounds(DOUBLE2 pos, int gameWidth, int gameHeight)
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		if (m_ProjectilePtrArr[count] != nullptr)
		{
			if (m_ProjectilePtrArr[count]->GetPos().x > pos.x + gameWidth-200 || 
				m_ProjectilePtrArr[count]->GetPos().x < pos.x )
			{
				Remove(m_ProjectilePtrArr[count]);
			}
		}
			
	}
}

void ProjectileList::HandleCollision(HitRegion* hitRegionPtr, double deltaTime)
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		if (m_ProjectilePtrArr[count] != nullptr)
		{
			m_ProjectilePtrArr[count]->HandleCollision(hitRegionPtr, deltaTime);
		}
			
	}
}

void ProjectileList::AddGravity(DOUBLE2 force)
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		if (m_ProjectilePtrArr[count] != nullptr)
		{
			m_ProjectilePtrArr[count]->AddGravity(force);
		}
	}
}

bool ProjectileList::CheckHit(HitRegion* hitTargetPtr, bool isEnemy)
{
	for (int count=0; count < MAX_PROJECTILES; ++count)
	{
		
		if (m_ProjectilePtrArr[count] != nullptr)
		{
			
			if (m_ProjectilePtrArr[count]->IsFromEnemy()==true && isEnemy==false)
			{
				if (m_ProjectilePtrArr[count]->CheckHit(hitTargetPtr)==true)
				{
					return true;
				}
			}
			else if (m_ProjectilePtrArr[count]->IsFromEnemy()==false && isEnemy==true)
			{
				if (m_ProjectilePtrArr[count]->CheckHit(hitTargetPtr)==true)
				{
					return true;
				}
				
			}

		}
			
	}

	return false;
}



