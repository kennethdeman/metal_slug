//---------------------------
// Includes
//---------------------------
#include "Rocket.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* Rocket::m_BmpProjectilePtr=nullptr;
Bitmap* Rocket::m_BmpExplosionPtr=nullptr;


//---------------------------
// Constructor & Destructor
//---------------------------
Rocket::Rocket(int x, int y) : m_Pos(x,y), 
	 m_CurrentState(STATE_NORMAL)
	 , m_TickCount(0), m_SlowDownAnimations(-10),
	 m_IsFromEnemy(false)
{

	if (Rocket::m_BmpProjectilePtr==nullptr)
	{
		Rocket::m_BmpProjectilePtr= new Bitmap(IDB_ROCKET);
		Rocket::m_BmpProjectilePtr->SetTransparencyColor(0,106,250);
	}

	if (Rocket::m_BmpExplosionPtr==nullptr)
	{
		Rocket::m_BmpExplosionPtr= new Bitmap(IDB_EXPLOSION);
	}

	m_HitRocketPtr = new HitRegion();
	m_HitRocketPtr->CreateFromRect(-ROCKET_WIDTH/2+10, -CENTER_Y+10, ROCKET_WIDTH/2-10, CENTER_Y-10);

	m_Velocity.x=-ROCKET_SPEED;
}

Rocket::Rocket(int x, int y, bool isEnemyGrenade) : m_Pos(x,y), 
	  m_CurrentState(STATE_NORMAL)
	 , m_TickCount(0), m_SlowDownAnimations(-10),
	 m_IsFromEnemy(isEnemyGrenade)
{

	if (Rocket::m_BmpProjectilePtr==nullptr)
	{
		Rocket::m_BmpProjectilePtr= new Bitmap(IDB_ROCKET);
		Rocket::m_BmpProjectilePtr->SetTransparencyColor(0,106,250);
	}

	if (Rocket::m_BmpExplosionPtr==nullptr)
	{
		Rocket::m_BmpExplosionPtr= new Bitmap(IDB_EXPLOSION);
	}

	m_HitRocketPtr = new HitRegion();
	m_HitRocketPtr->CreateFromRect(-ROCKET_WIDTH/2+10, -CENTER_Y+10, ROCKET_WIDTH/2-10, CENTER_Y-10);

	m_Velocity.x=-ROCKET_SPEED;
}

Rocket::~Rocket()
{
	delete m_HitRocketPtr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void Rocket::Tick(double deltaTime)
{
	m_HitRocketPtr->SetPos(m_Pos);

	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 		

		//Key input setting the speed.	
		m_Pos += m_Velocity*deltaTime;

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount>=ROCKET_FRAMES)
			{
				m_TickCount=0;
			}
		}
		m_SlowDownAnimations+=ROCKET_ANIMATION_SPEED*deltaTime;

		break;

	case STATE_EXPLODING : 


		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount>=EXPLOSION_FRAMES)
			{
				m_CurrentState=STATE_DESTROYED;
			}
		}
		m_SlowDownAnimations+=EXPLOSION_ANIMATION_SPEED*deltaTime;

		break;
	}

	
}

void Rocket::Paint(MATRIX3X2 matTransform)
{
	

	RECT frame;

	int width=0;
	int heightStart=0;
	int heightEnd=0;

	MATRIX3X2 matCenter,matRotate,matScale,matTranslate;

	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 		

		width=ROCKET_WIDTH;
		heightStart=0;
		heightEnd=ROCKET_HEIGHT_END;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		matCenter.SetAsTranslate(-CENTER_X+EXPLOSION_OFFSET_X,-CENTER_Y);
		
		matScale.SetAsScale(1,1);
		
		matTranslate.SetAsTranslate(m_Pos);

		GAME_ENGINE->SetTransformMatrix(matCenter*matRotate*matScale*matTranslate*matTransform);

		GAME_ENGINE->DrawBitmap(m_BmpProjectilePtr,0,0,frame);

		break;

	case STATE_EXPLODING : 

		width=EXPLOSION_WIDTH;
		heightStart=EXPLOSION_HEIGHT_START;
		heightEnd=EXPLOSION_HEIGHT;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightStart+heightEnd;

		matCenter.SetAsTranslate(EXPLOSION_OFFSET_X,-heightEnd + CENTER_Y);

		matTranslate.SetAsTranslate(m_Pos);

		GAME_ENGINE->SetTransformMatrix(matCenter*matRotate*matScale*matTranslate*matTransform);

		GAME_ENGINE->DrawBitmap(m_BmpExplosionPtr,0,0,frame);

		break;
	}

	
	GAME_ENGINE->SetTransformMatrix(matTransform);
	//GAME_ENGINE->SetColor(COLOR (255,0,0,127));
	//GAME_ENGINE->FillEllipse(m_Pos.x, m_Pos.y,3,3);

	//GAME_ENGINE->FillHitRegion(m_HitRocketPtr);

}

void Rocket::HandleCollision(HitRegion* hitRegionPtr, double deltaTime)
{
	//Do nothing
}

void Rocket::AddGravity(DOUBLE2 force)
{
	//Do nothing
}

bool Rocket::IsDestroyed()
{
	return m_CurrentState==STATE_DESTROYED;
}

DOUBLE2 Rocket::GetPos()
{
	return m_Pos;
}

bool Rocket::CheckHit(HitRegion* hitTargetPtr)
{
	//Only if the bomb is in the beginning stage of exploding (tickcount->0)
	if (m_CurrentState!=STATE_EXPLODING)
	{
		
		RECT2 collision = m_HitRocketPtr->CollisionTest(hitTargetPtr);

		// if verticalCollision values != 0,0,0,0
		if ( (collision.bottom - collision.top) > 0.1 || (collision.right - collision.left) > 0.1)
		{
			m_CurrentState=STATE_EXPLODING;		

			//Set the position of the explosion on the target
			RECT2 playerBounds=hitTargetPtr->GetBounds();
			m_Pos.x=playerBounds.right - (playerBounds.right - playerBounds.left)/2;

			m_TickCount=0;
			return true;
		}

	}


	return false;
}


bool Rocket::IsFromEnemy()
{
	return m_IsFromEnemy;
}