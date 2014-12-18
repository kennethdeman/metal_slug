//---------------------------
// Includes
//---------------------------
#include "Bullet.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* Bullet::m_BmpBulletPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
Bullet::Bullet(int x, int y, double angle): m_Pos(x,y), m_Angle(angle), 
	 m_Velocity(0,0), m_Speed(800), m_CurrentState(STATE_NORMAL)
{
	
	if (Bullet::m_BmpBulletPtr==nullptr)
	{
		Bullet::m_BmpBulletPtr= new Bitmap(IDB_BULLETGRENADE);
	}	


}

Bullet::~Bullet()
{
	
}

//---------------------------
// Own methods
//---------------------------

void Bullet::Tick(double deltaTime)
{
	//Key input setting the speed.	

	m_Velocity.x = m_Speed * cos(m_Angle);
	m_Velocity.y = m_Speed * sin(m_Angle);

	m_Pos += m_Velocity*deltaTime;

	//Positions the hitboxes

	
}

void Bullet::Paint(MATRIX3X2 matTransform)
{
	RECT frame;

	frame.top= 117;
	frame.bottom= m_BmpBulletPtr->GetHeight();
	frame.left=0;
	frame.right=26;


	MATRIX3X2 matCenter,matRotate,matScale,matTranslate;

	matCenter.SetAsTranslate(-CENTER_X,-CENTER_Y);

	matRotate.SetAsRotate(m_Angle);

	matScale.SetAsScale(1,1);
	
	matTranslate.SetAsTranslate(m_Pos);

	GAME_ENGINE->SetTransformMatrix(matCenter*matRotate*matScale*matTranslate*matTransform);

	GAME_ENGINE->DrawBitmap(m_BmpBulletPtr,0,0,frame);
}

void Bullet::HandleCollision(HitRegion* hitRegionPtr, double deltaTime)
{
	if (hitRegionPtr->HitTest(m_Pos)==true) m_CurrentState=STATE_DESTROYED;
}


void Bullet::AddGravity(DOUBLE2 force)
{
	//Do nothing
}

bool Bullet::IsDestroyed()
{
	return m_CurrentState==STATE_DESTROYED;
}

DOUBLE2 Bullet::GetPos()
{
	return m_Pos;
}

bool Bullet::CheckHit(HitRegion* hitTargetPtr)
{
	if (hitTargetPtr->HitTest(m_Pos)==true)
	{
		m_CurrentState=STATE_DESTROYED;
		return true;
	}

	return false;
	
}

bool Bullet::IsFromEnemy()
{
	return false;
}
