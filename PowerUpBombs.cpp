//---------------------------
// Includes
//---------------------------
#include "PowerUpBombs.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* PowerUpBombs::m_BmpPowerUpBombsSpriteSheetPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
PowerUpBombs::PowerUpBombs(int x, int y) : m_Pos(x,y), m_HitRegionPtr(nullptr), m_TickCount(0),
	 m_CurrentState(0),	m_SlowDownAnimations(0)
{
	// If not yet done, create bitmaps and set transparency color
	if (PowerUpBombs::m_BmpPowerUpBombsSpriteSheetPtr==nullptr)
	{
		PowerUpBombs::m_BmpPowerUpBombsSpriteSheetPtr= new Bitmap(IDB_POWERUP);
	}
	
	m_HitRegionPtr = new HitRegion();
	m_HitRegionPtr->CreateFromRect(0, 0, WIDTH, HEIGHT);
}

PowerUpBombs::~PowerUpBombs()
{
	delete m_HitRegionPtr;
	m_HitRegionPtr = nullptr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void PowerUpBombs::Tick(double deltaTime)
{
	
	m_HitRegionPtr->SetPos(GetPos());
	

	if ( m_CurrentState == STATE_IDLE)
	{
		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount>=FRAMES)
			{
				m_TickCount=0;
			}
		}
		m_SlowDownAnimations+=ANIMATION_SPEED*deltaTime;
				

	}
}

void PowerUpBombs::Paint(MATRIX3X2 matTransform)
{	
	RECT frame;
	frame.left=0;
	frame.right=0;		
	frame.top=0;
	frame.bottom=0;

	int width=0;
	int heightStart=0;
	int heightEnd=0;


	//Standing still, no action
	if ( m_CurrentState == STATE_IDLE )
	{
		// See SpriteSheet for values	
		width=WIDTH;
		heightStart=0;
		heightEnd=HEIGHT;

		frame.left=width*m_TickCount+1;
		frame.right=width+width*m_TickCount-1;		
		frame.top=heightStart+2;
		frame.bottom=heightEnd;

	}

	MATRIX3X2 matTranslate;

	matTranslate.SetAsTranslate(m_Pos);		

	GAME_ENGINE->SetTransformMatrix(matTranslate*matTransform);
	
	GAME_ENGINE->DrawBitmap(m_BmpPowerUpBombsSpriteSheetPtr,0,0,frame);	

	GAME_ENGINE->SetColor(COLOR(0,255,0,128));

	GAME_ENGINE->SetTransformMatrix(matTransform);
	//GAME_ENGINE->FillHitRegion(m_HitRegionPtr);
}

bool PowerUpBombs::CollisionWithPlayer(HitRegion* hitPlayerPtr)
{
	if ( m_CurrentState==STATE_POINTSGAINED) return false;
	if ( m_HitRegionPtr->HitTest(hitPlayerPtr) )
	{
		m_CurrentState=STATE_POINTSGAINED;
		return true;
	}

	return false;
}

void PowerUpBombs::SetPos(int x, int y)
{
	m_Pos.x=x;
	m_Pos.y=y;
}

void PowerUpBombs::SetPos(DOUBLE2 pos)
{
	m_Pos.x=pos.x;
	m_Pos.y=pos.y;
}

void PowerUpBombs::SetXPos(int x)
{
	m_Pos.x=x;
}

void PowerUpBombs::SetYPos(int y)
{
	m_Pos.y=y;
}


DOUBLE2 PowerUpBombs::GetPos()
{
	return m_Pos;
}


HitRegion* PowerUpBombs::GetHitRegion()
{
	return m_HitRegionPtr;
}

int PowerUpBombs::GivePoints()
{
	return 0;
}

int PowerUpBombs::GiveBombAmmunition()
{
	return BOMB_AMMUNITION;
}


