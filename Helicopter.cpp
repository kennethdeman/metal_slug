//---------------------------
// Includes
//---------------------------
#include "Helicopter.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* Helicopter::m_BmpHelicopterSpriteSheetPtr=nullptr;
Bitmap* Helicopter::m_BmpHeliExplosionSpriteSheetPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
Helicopter::Helicopter(int x, int y) : m_Pos(x,y), m_Velocity(0,0), m_HitRegionPtr(nullptr), 
	 m_TickCount(0),
	m_CurrentDirection(Helicopter::DIRECTION_LEFT), m_CurrentState(0),
	m_SlowDownAnimations(0), 
	m_OffsetX(0), m_OffsetY(0),
	m_PointRay1(0,0), m_VectorRay1(0,0), m_PointRay2(0,0), m_VectorRay2(0,0), m_PointRay3(0,0), m_VectorRay3(0,0),
	m_PointRay4(0,0), m_VectorRay4(0,0),
	m_AmountOfBombsReady(0) , m_BombsTimer(1), m_AboveTarget(false), m_HasTarget(false), m_TargetPos(0,0),
	m_Health(30), m_PointsGiven(false)
{
	// If not yet done, create bitmaps and set transparency color
	if (Helicopter::m_BmpHelicopterSpriteSheetPtr==nullptr)
	{
		Helicopter::m_BmpHelicopterSpriteSheetPtr= new Bitmap(IDB_HELICOPTER);
	}
	if (Helicopter::m_BmpHeliExplosionSpriteSheetPtr==nullptr)
	{
		Helicopter::m_BmpHeliExplosionSpriteSheetPtr= new Bitmap(IDB_HELIEXPLOSION);
	}

	m_AudioExplosionPtr=new Audio("./resources/Explosion.wav");
	m_AudioExplosionPtr->SetRepeat(false);

	m_HitRegionPtr = new HitRegion();
	m_HitRegionPtr->CreateFromRect(-CENTER_X, -CENTER_Y+10, CENTER_X, CENTER_Y);

}

Helicopter::~Helicopter()
{
	delete m_AudioExplosionPtr;
	delete m_HitRegionPtr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void Helicopter::Tick(double deltaTime)
{
	m_AudioExplosionPtr->Tick();

	//Key input setting the speed.	

	m_Pos.x += m_Velocity.x*deltaTime;

	m_Pos.y += m_Velocity.y*deltaTime;

	//Positions the hitboxes

	m_HitRegionPtr->SetPos(m_Pos);	



	switch (m_CurrentState) 
	{

	case STATE_FLYING : 

		//Bombs timer
		m_BombsTimer+=1*deltaTime;

		
		//Follow target
		if (m_HasTarget==true)
		{
			//Reset
			m_AboveTarget=false;

			//Fly to target
			if (m_Pos.x>m_TargetPos.x+10) m_Velocity.x=-100;
			else if (m_Pos.x<m_TargetPos.x-10) m_Velocity.x=100;
			else 
			{
				m_AboveTarget=true;

				m_Velocity.x=0;
			}

				

		}

		//Face towards velocity direction
		if (m_Velocity.x<0) m_CurrentDirection=DIRECTION_LEFT;
		else if (m_Velocity.x>0) m_CurrentDirection=DIRECTION_RIGHT;

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;			

			if (m_TickCount==FLYING_FRAMES)
			{
				m_TickCount=0;

			}

						
		}		
		

		m_SlowDownAnimations+=FLYING_ANIMATION_SPEED*deltaTime;

		break;

	case STATE_CRASHING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;			

			if (m_TickCount==CRASHING_FRAMES)
			{
				m_TickCount=0;
			}
						
		}		
		

		m_SlowDownAnimations+=CRASHING_ANIMATION_SPEED*deltaTime;

		break;

	case STATE_EXPLODING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount>=EXPLODING_FRAMES)
			{
				m_TickCount=0;
				m_CurrentState=STATE_DESTROYED;
			}
		}
		m_SlowDownAnimations+=EXPLODING_ANIMATION_SPEED*deltaTime;

		break;

	}
}

void Helicopter::Paint(MATRIX3X2 matTransform)
{	
	RECT frame;
	frame.left=0;
	frame.right=WIDTH;
	frame.top=165;
	frame.bottom=165+HEIGHT;

	int width=0;
	int heightStart=0;
	int heightEnd=0;

	m_OffsetX=0;
	m_OffsetY=0;


	switch (m_CurrentState) 
	{

	case STATE_FLYING : 

		width=FLYING_WIDTH;
		heightStart=FLYING_TOP;
		heightEnd=FLYING_BOTTOM;
		
		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;		

		break;

	case STATE_CRASHING : 

		width=CRASHING_WIDTH;
		heightStart=CRASHING_TOP;
		heightEnd=CRASHING_BOTTOM;
		
		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		break;

	case STATE_EXPLODING : 

		width=EXPLODING_WIDTH;
		heightStart=EXPLODING_TOP;
		heightEnd=EXPLODING_BOTTOM;
		
		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetY=EXPLODING_OFFSET_Y;	

		break;

	}
	
	MATRIX3X2 matCenter,matScale,matTranslate;

	matCenter.SetAsTranslate(-CENTER_X+m_OffsetX,-CENTER_Y+m_OffsetY);

	if (m_CurrentDirection==DIRECTION_LEFT)
	{
		matScale.SetAsScale(1,1);
	}
	else
	{
		matScale.SetAsScale(-1,1);
	}
	
	matTranslate.SetAsTranslate(m_Pos);

	GAME_ENGINE->SetTransformMatrix(matCenter*matScale*matTranslate*matTransform);
	

	if (m_CurrentState!=STATE_EXPLODING && m_CurrentState!=STATE_DESTROYED)
		GAME_ENGINE->DrawBitmap(m_BmpHelicopterSpriteSheetPtr,0,0,frame);	

	if (m_CurrentState==STATE_EXPLODING)
		GAME_ENGINE->DrawBitmap(m_BmpHeliExplosionSpriteSheetPtr,0,0,frame);
	
	//GAME_ENGINE->SetColor(COLOR(255,0,255,127));
	//GAME_ENGINE->FillEllipse(CENTER_X,CENTER_Y,3,3);

	//GAME_ENGINE->SetTransformMatrix(matTransform);

	//GAME_ENGINE->SetColor(COLOR(0,255,0,180));
	//GAME_ENGINE->DrawLine(m_PointRay1,m_PointRay1+m_VectorRay1);
	//GAME_ENGINE->DrawLine(m_PointRay2,m_PointRay2+m_VectorRay2);

	//GAME_ENGINE->SetColor(COLOR(0,255,0,128));
	//GAME_ENGINE->DrawLine(m_PointRay3,m_PointRay3+m_VectorRay3);	
	//GAME_ENGINE->DrawLine(m_PointRay4,m_PointRay4+m_VectorRay4);

	//GAME_ENGINE->FillHitRegion(m_HitRegionPtr);
}


void Helicopter::HandleCollision(HitRegion* hitRegionPtr)
{
	HIT hitArr[1];

	//Ray1

	m_PointRay1= DOUBLE2(m_Pos.x-5,m_Pos.y-CENTER_Y);
	m_VectorRay1 = DOUBLE2(0,HEIGHT);

	RaycastFullCollision(hitRegionPtr, m_PointRay1,m_VectorRay1);

	//Ray2

	m_PointRay2= DOUBLE2(m_Pos.x+5,m_Pos.y-CENTER_Y);
	m_VectorRay2 = DOUBLE2(0,HEIGHT);

	RaycastFullCollision(hitRegionPtr, m_PointRay2,m_VectorRay2);

	//Ray3

	m_PointRay3= DOUBLE2(m_Pos.x-WIDTH/2,m_Pos.y-5);
	m_VectorRay3 = DOUBLE2(WIDTH,0);

	RaycastFullCollision(hitRegionPtr, m_PointRay3,m_VectorRay3);

	//Ray4

	m_PointRay4= DOUBLE2(m_Pos.x-WIDTH/2,m_Pos.y+5);
	m_VectorRay4 = DOUBLE2(WIDTH,0);

	RaycastFullCollision(hitRegionPtr, m_PointRay4,m_VectorRay4);	
	
	m_PointRay1= DOUBLE2(m_Pos.x-5,m_Pos.y-CENTER_Y);	
	m_PointRay2= DOUBLE2(m_Pos.x+5,m_Pos.y-CENTER_Y);	
	m_PointRay3= DOUBLE2(m_Pos.x-WIDTH/2,m_Pos.y-5);
	m_PointRay4= DOUBLE2(m_Pos.x-WIDTH/2,m_Pos.y+5);
}

void Helicopter::RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector)
{
	HIT hitArr[1];

	if (hitRegionPtr->Raycast(point, vector, hitArr, 1, -1) == 1)
	{
		if (hitArr[0].lambda>0.8)
		{
			//Vertical rays
			if (vector.x==0)
			{
				m_Pos.y-= vector.Length()*(1-hitArr[0].lambda);
				m_Velocity=DOUBLE2(0,0);
				
				if (m_CurrentState==STATE_CRASHING)
				{
					m_AudioExplosionPtr->Play();
					m_CurrentState=STATE_EXPLODING;
					m_TickCount=0;
				}				

			}//Horizontal rays right
			else
			{
				m_Pos.x-= m_VectorRay3.Length()*(1-hitArr[0].lambda);
			}

		}		
	}

	//Horizontal rays left
	if (vector.y==0)
	{
		if (hitRegionPtr->Raycast(point, vector, hitArr, 1, 1) == 1)
		{

			m_Pos.x+= m_VectorRay3.Length()*(hitArr[0].lambda);

		}
	}

}

void Helicopter::HandleSemiCollision(HitRegion* hitRegionPtr)
{
	//No semi collision
}


void Helicopter::SetPos(int x, int y)
{
	m_Pos.x=x;
	m_Pos.y=y;
}

void Helicopter::SetPos(DOUBLE2 pos)
{
	m_Pos.x=pos.x;
	m_Pos.y=pos.y;
}

void Helicopter::SetXPos(int x)
{
	m_Pos.x=x;
}

void Helicopter::SetYPos(int y)
{
	m_Pos.y=y;
}

DOUBLE2 Helicopter::GetPos()
{
	return m_Pos;
}

void Helicopter::SetVelocity(int x, int y)
{
	m_Velocity.x=x;
	m_Velocity.y=y;
}

void Helicopter::SetVelocity(DOUBLE2 velocity)
{
	m_Velocity.x=velocity.x;
	m_Velocity.y=velocity.y;
}

void Helicopter::SetXVelocity(int x)
{
	m_Velocity.x=x;
}

void Helicopter::SetYVelocity(int y)
{
	m_Velocity.y=y;
}


DOUBLE2 Helicopter::GetVelocity()
{
	return m_Velocity;
}

HitRegion* Helicopter::GetHitRegion()
{
	return m_HitRegionPtr;
}

void Helicopter::SetDirection(bool isLeft)
{
	if (isLeft==true)
	{
		m_CurrentDirection=DIRECTION_LEFT;
	}
	else
	{
		m_CurrentDirection=DIRECTION_RIGHT;
	}

}

int Helicopter::GetDirection()
{
	return m_CurrentDirection;
}


void Helicopter::SetState(int state)
{

	if ( !(state != STATE_FLYING && state != STATE_CRASHING && state != STATE_EXPLODING && state != STATE_DESTROYED))
	{		
		if (m_CurrentState !=state)
		{
			m_CurrentState=state;

			m_TickCount=0;
		}
	}			
}

void Helicopter::AddGravity(DOUBLE2 force)
{
	// divided by 2 because gravity pulls down the heli slower due to the rotors that are still working
	if (m_CurrentState==STATE_CRASHING)
	m_Velocity+=force/2;
}

int Helicopter::GivePoints()
{
	if (m_PointsGiven==false && m_CurrentState==STATE_CRASHING)
	{
		m_PointsGiven=true;
		return POINTS;
	}

	return 0;
}


bool Helicopter::IsAlive()
{

	return (m_CurrentState!=STATE_CRASHING && m_CurrentState!=STATE_EXPLODING && m_CurrentState!=STATE_DESTROYED);
	
}

bool Helicopter::IsReadyToBeDeleted()
{
	return m_CurrentState==STATE_DESTROYED;
}

bool Helicopter::SetTarget(DOUBLE2 targetPos)
{
	if ( (targetPos-m_Pos).Length()<=500)
	{
		m_HasTarget=true;
		m_TargetPos=targetPos;
		
		return true;
	}
	else
		m_HasTarget=false;
	
	return false;
}

bool Helicopter::IsHostile()
{
	return true;
}

bool Helicopter::UsesBombs()
{
	return true;
}

int Helicopter::GetProjectileType()
{
	return Projectile::BOMB;
}


void Helicopter::SubtractHealth(double health)
{
	m_Health-=health;

	if (m_Health<0) m_Health=0;

	if (m_Health==0 && m_CurrentState==STATE_FLYING)
	{		
		m_CurrentState=STATE_CRASHING;
		m_TickCount=0;
	}

}

bool Helicopter::ExecuteAttack()
{
	if (m_BombsTimer>1 && m_AboveTarget==true)
	{
		m_BombsTimer=0;
		m_AmountOfBombsReady=3;
		
	}

	//Drop 3 bombs in succesion
	if (m_AmountOfBombsReady>0 && m_HasTarget==true && m_BombsTimer>0.2)
	{
		m_BombsTimer=0;
		--m_AmountOfBombsReady;

		return true;
	}

	return false;
}

bool Helicopter::ExecuteSecondaryAttack(DOUBLE2 targetPos)
{
	//No secondary attack
	return false;
}


void Helicopter::GetCameraBounds(DOUBLE2 pos, int width, int height)
{

}
