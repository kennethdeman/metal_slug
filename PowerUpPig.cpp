//---------------------------
// Includes
//---------------------------
#include "PowerUpPig.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* PowerUpPig::m_BmpPowerUpPigSpriteSheetPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
PowerUpPig::PowerUpPig(int x, int y) : m_Pos(x,y), m_HitRegionPtr(nullptr), m_TickCount(0),
	 m_CurrentState(0),	m_SlowDownAnimations(0)
{
	// If not yet done, create bitmaps and set transparency color
	if (PowerUpPig::m_BmpPowerUpPigSpriteSheetPtr==nullptr)
	{
		PowerUpPig::m_BmpPowerUpPigSpriteSheetPtr= new Bitmap(IDB_POWERUP);
	}
	
	m_HitRegionPtr = new HitRegion();
	m_HitRegionPtr->CreateFromRect(0, 0, WIDTH, HEIGHT);
}

PowerUpPig::~PowerUpPig()
{
	delete m_HitRegionPtr;
	m_HitRegionPtr = nullptr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void PowerUpPig::Tick(double deltaTime)
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

void PowerUpPig::Paint(MATRIX3X2 matTransform)
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
		heightStart=HEIGHTSTART;
		heightEnd=HEIGHTSTART+HEIGHT;

		frame.left=width*m_TickCount+1;
		frame.right=width+width*m_TickCount-1;		
		frame.top=heightStart+2;
		frame.bottom=heightEnd;

	}

	MATRIX3X2 matTranslate;

	matTranslate.SetAsTranslate(m_Pos);		

	GAME_ENGINE->SetTransformMatrix(matTranslate*matTransform);
	
	GAME_ENGINE->DrawBitmap(m_BmpPowerUpPigSpriteSheetPtr,0,0,frame);	

	GAME_ENGINE->SetColor(COLOR(0,255,0,128));

	GAME_ENGINE->SetTransformMatrix(matTransform);
	//GAME_ENGINE->FillHitRegion(m_HitRegionPtr);
}

bool PowerUpPig::CollisionWithPlayer(HitRegion* hitPlayerPtr)
{
	if ( m_CurrentState==STATE_POINTSGAINED) return false;
	if ( m_HitRegionPtr->HitTest(hitPlayerPtr) )
	{
		m_CurrentState=STATE_POINTSGAINED;
		return true;
	}

	return false;
}


void PowerUpPig::SetPos(int x, int y)
{
	m_Pos.x=x;
	m_Pos.y=y;
}

void PowerUpPig::SetPos(DOUBLE2 pos)
{
	m_Pos.x=pos.x;
	m_Pos.y=pos.y;
}

void PowerUpPig::SetXPos(int x)
{
	m_Pos.x=x;
}

void PowerUpPig::SetYPos(int y)
{
	m_Pos.y=y;
}

DOUBLE2 PowerUpPig::GetPos()
{
	return m_Pos;
}

HitRegion* PowerUpPig::GetHitRegion()
{
	return m_HitRegionPtr;
}

void PowerUpPig::IsPig()
{

}

int PowerUpPig::GivePoints()
{
	return POINTS;
}

int PowerUpPig::GiveBombAmmunition()
{
	return 0;
}


