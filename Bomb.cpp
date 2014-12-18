//---------------------------
// Includes
//---------------------------
#include "Bomb.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* Bomb::m_BmpGrenadePtr=nullptr;
Bitmap* Bomb::m_BmpExplosionPtr=nullptr;


//---------------------------
// Constructor & Destructor
//---------------------------
Bomb::Bomb(int x, int y) : m_Pos(x,y), 
	 m_CurrentState(STATE_NORMAL)
	 , m_TickCount(0), m_SlowDownAnimations(-10),
	 m_IsFromEnemy(false)
{

	if (Bomb::m_BmpGrenadePtr==nullptr)
	{
		Bomb::m_BmpGrenadePtr= new Bitmap(IDB_BULLETGRENADE);
	}

	if (Bomb::m_BmpExplosionPtr==nullptr)
	{
		Bomb::m_BmpExplosionPtr= new Bitmap(IDB_EXPLOSION);
	}

	m_HitBombPtr = new HitRegion();
	m_HitBombPtr->CreateFromRect(-CENTER_X, -CENTER_Y, CENTER_X, CENTER_Y);
	m_HitExplosionPtr = new HitRegion();	
	m_HitExplosionPtr->CreateFromRect(-CENTER_X-10, -CENTER_Y-30, CENTER_X+10, CENTER_Y);
}

Bomb::Bomb(int x, int y, bool isEnemyGrenade) : m_Pos(x,y), 
	 m_CurrentState(STATE_NORMAL)
	 , m_TickCount(0), m_SlowDownAnimations(-10), 
	 m_IsFromEnemy(isEnemyGrenade)
{

	if (Bomb::m_BmpGrenadePtr==nullptr)
	{
		Bomb::m_BmpGrenadePtr= new Bitmap(IDB_BULLETGRENADE);
	}

	if (Bomb::m_BmpExplosionPtr==nullptr)
	{
		Bomb::m_BmpExplosionPtr= new Bitmap(IDB_EXPLOSION);
	}

	m_HitBombPtr = new HitRegion();
	m_HitBombPtr->CreateFromRect(-CENTER_X, -CENTER_Y, CENTER_X, CENTER_Y);
	m_HitExplosionPtr = new HitRegion();	
	m_HitExplosionPtr->CreateFromRect(-CENTER_X-10, -CENTER_Y-30, CENTER_X+10, CENTER_Y);
}

Bomb::~Bomb()
{
	delete m_HitBombPtr;
	delete m_HitExplosionPtr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void Bomb::Tick(double deltaTime)
{
	m_HitBombPtr->SetPos(m_Pos);
	m_HitExplosionPtr->SetPos(m_Pos);

	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 		

		//Key input setting the speed.	
		m_Pos += m_Velocity*deltaTime;

		

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

	case STATE_EXPLODING2 : 
		
		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount>=EXPLOSION2_FRAMES)
			{
				m_CurrentState=STATE_DESTROYED;
			}
		}
		m_SlowDownAnimations+=EXPLOSION2_ANIMATION_SPEED*deltaTime;

		break;
	}

	
}

void Bomb::Paint(MATRIX3X2 matTransform)
{
	

	RECT frame;

	int width=0;
	int heightStart=0;
	int heightEnd=0;

	MATRIX3X2 matCenter,matRotate,matScale,matTranslate;

	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 		

		frame.top= BOMB_FRAME_Y;
		frame.bottom= BOMB_FRAME_Y+BOMB_HEIGHT;
		frame.left= BOMB_FRAME_X;
		frame.right= BOMB_FRAME_X+BOMB_WIDTH;		

		matCenter.SetAsTranslate(-CENTER_X,-CENTER_Y);

		matScale.SetAsScale(1,1);
		
		matTranslate.SetAsTranslate(m_Pos);

		GAME_ENGINE->SetTransformMatrix(matCenter*matRotate*matScale*matTranslate*matTransform);

		GAME_ENGINE->DrawBitmap(m_BmpGrenadePtr,0,0,frame);

		break;

	case STATE_EXPLODING : 

		width=EXPLOSION_WIDTH;
		heightStart=0;
		heightEnd=EXPLOSION_HEIGHT_END;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		matCenter.SetAsTranslate(EXPLOSION_OFFSET_X,-heightEnd + CENTER_Y);

		matTranslate.SetAsTranslate(m_Pos);

		GAME_ENGINE->SetTransformMatrix(matCenter*matRotate*matScale*matTranslate*matTransform);

		GAME_ENGINE->DrawBitmap(m_BmpExplosionPtr,0,0,frame);

		break;

	case STATE_EXPLODING2 : 

		width=EXPLOSION2_WIDTH;
		heightStart=EXPLOSION2_HEIGHT_START;
		heightEnd=EXPLOSION2_HEIGHT;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightStart+heightEnd;

		matCenter.SetAsTranslate(EXPLOSION2_OFFSET_X,-EXPLOSION2_HEIGHT/2);

		matTranslate.SetAsTranslate(m_Pos);

		GAME_ENGINE->SetTransformMatrix(matCenter*matRotate*matScale*matTranslate*matTransform);

		GAME_ENGINE->DrawBitmap(m_BmpExplosionPtr,0,0,frame);

		break;

	}

	
	GAME_ENGINE->SetTransformMatrix(matTransform);
	//GAME_ENGINE->SetColor(COLOR (255,0,0,127));

	//switch (m_CurrentState) 
	//{

	//case STATE_NORMAL : 
	//	
	//	GAME_ENGINE->FillHitRegion(m_HitBombPtr);
	//break;

	//case STATE_EXPLODING :

	//	GAME_ENGINE->FillHitRegion(m_HitExplosionPtr);

	//	break;
	//}

}

void Bomb::HandleCollision(HitRegion* hitRegionPtr, double deltaTime)
{
	HIT hitArr[1];	
	
	DOUBLE2 RaycastPoint= m_Pos;
	RaycastPoint.y -=CENTER_Y;
	DOUBLE2 RaycastVector = DOUBLE2(0,CENTER_Y*2);
	if (hitRegionPtr->Raycast(RaycastPoint, RaycastVector, hitArr, 1, -1)>=1)
	{		
		m_CurrentState=STATE_EXPLODING;
	}
}

//Used for gravity
void Bomb::AddGravity(DOUBLE2 force)
{
	m_Velocity+=force;
}

bool Bomb::IsDestroyed()
{
	return m_CurrentState==STATE_DESTROYED;
}

DOUBLE2 Bomb::GetPos()
{
	return m_Pos;
}

bool Bomb::CheckHit(HitRegion* hitTargetPtr)
{
	//Only if the bomb is in the beginning stage of exploding (tickcount->0)
	if (m_CurrentState==STATE_EXPLODING && m_TickCount==0)
	{
		
		RECT2 collision = m_HitExplosionPtr->CollisionTest(hitTargetPtr);

		// if verticalCollision values != 0,0,0,0
		if ( (collision.bottom - collision.top) > 0.1 || (collision.right - collision.left) > 0.1)
		{
			return true;
		}

	}
	else
	{
		RECT2 collision = m_HitBombPtr->CollisionTest(hitTargetPtr);

		// if verticalCollision values != 0,0,0,0
		if ( (collision.bottom - collision.top) > 0.1 || (collision.right - collision.left) > 0.1)
		{
			if (m_CurrentState==STATE_NORMAL)
			{				

				//Set the position of the explosion on the target
				RECT2 playerBounds=hitTargetPtr->GetBounds();
				m_Pos.x=playerBounds.right - (playerBounds.right - playerBounds.left)/2;
				m_Pos.y+=10;
				m_CurrentState=STATE_EXPLODING2;
				m_TickCount=0;

				return true;
			}
			
		}
	}


	return false;
}


bool Bomb::IsFromEnemy()
{
	return m_IsFromEnemy;
}