//---------------------------
// Includes
//---------------------------
#include "RocketDiver.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* RocketDiver::m_BmpRocketDiverSpriteSheetPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
RocketDiver::RocketDiver(int x, int y) : m_Pos(x,y), m_Velocity(0,0), m_HitRegionPtr(nullptr), m_TickCount(0),
	m_TickCountSplash(0), 
	m_CurrentDirection(DIRECTION_LEFT), m_CurrentState(STATE_HIDDEN), m_Health(1),
	m_SlowDownAnimations(0),m_SlowDownAnimationsSplash(0),
	m_OffsetX(0), m_OffsetY(0),
	m_PointRay1(0,0), m_VectorRay1(0,0), m_PointRay2(0,0), m_VectorRay2(0,0), m_PointRay3(0,0), m_VectorRay3(0,0),
	m_PointRay4(0,0), m_VectorRay4(0,0)
	, m_HasExecutedAttack(false), m_HasTarget(false), m_AnimateSplash(false),
	m_TargetPos(0,0), m_SplashPos(x,y-20), m_PointsGiven(false)
{
	// If not yet done, create bitmaps and set transparency color
	if (RocketDiver::m_BmpRocketDiverSpriteSheetPtr==nullptr)
	{
		RocketDiver::m_BmpRocketDiverSpriteSheetPtr= new Bitmap(IDB_ROCKETDIVER);
		RocketDiver::m_BmpRocketDiverSpriteSheetPtr->SetTransparencyColor(0,106,250);
	}


	m_AudioDeathScreamPtr=new Audio("./resources/Soldier_4.wav");
	m_AudioDeathScreamPtr->SetRepeat(false);		

	

	m_HitRegionPtr = new HitRegion();
	m_HitRegionPtr->CreateFromRect(-CENTER_X+5, -CENTER_Y+5, 
		WIDTH-CENTER_X-5, HEIGHT -CENTER_Y);

}


RocketDiver::~RocketDiver()
{
	delete m_AudioDeathScreamPtr;
	delete m_HitRegionPtr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void RocketDiver::Tick(double deltaTime)
{
	m_AudioDeathScreamPtr->Tick();

	//Key input setting the speed.	

	m_Pos += m_Velocity*deltaTime;
	

	// Positions the hitboxes 

	m_HitRegionPtr->SetPos(m_Pos);


	//has a target
	if (m_HasTarget==true)
	{
		//Jump up when the target is in range
		if (m_CurrentState==STATE_HIDDEN)
		{
			m_CurrentState=STATE_GOINGUP;
			m_Velocity+=DOUBLE2(0,-400);
			m_AnimateSplash=true;
		}

		//Handles throwing
		if (m_CurrentState==STATE_GOINGUP && m_TargetPos.y>m_Pos.y-15 && m_TargetPos.y<m_Pos.y+75 && m_Velocity.y>=0)
			m_CurrentState=STATE_THROWING;

	}	


	
	 		
	


	switch (m_CurrentState) 
	{
		
	case STATE_GOINGUP : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;

			if (m_TickCount<GOINGUP_FRAMES-1)
			{
				m_TickCount+=1;
			}
		}
		m_SlowDownAnimations+=THROWING_ANIMATION_SPEED*deltaTime;

		break;

	case STATE_THROWING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;

			if (m_TickCount<THROWING_FRAMES-1)
			{
				m_TickCount+=1;
			}
		}
		m_SlowDownAnimations+=THROWING_ANIMATION_SPEED*deltaTime;

		break;

	case STATE_DYING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;			

			int fadeOutTicks=8;
			if (m_TickCount<DYING_FRAMES-1+fadeOutTicks)
			{
				m_TickCount+=1;
			}
			else m_CurrentState=STATE_DEAD;
		}
		m_SlowDownAnimations+=DYING_ANIMATION_SPEED*deltaTime;

		break;
		
	}
	
	if (m_AnimateSplash==true)
	{
		if (m_SlowDownAnimationsSplash>0)
		{
			m_SlowDownAnimationsSplash=RESET_SLOW_DOWN;			

			if (m_TickCountSplash<SPLASH_FRAMES-1)
			{
				m_TickCountSplash+=1;
			}
			else
			{
				m_AnimateSplash=false;
				m_TickCountSplash=0;
			}
			
		}
		m_SlowDownAnimationsSplash+=SPLASH_ANIMATION_SPEED*deltaTime;
	}


}

void RocketDiver::Paint(MATRIX3X2 matTransform)
{	
	RECT frame;
	frame.left=0;
	frame.right=0;
	frame.top=0;
	frame.bottom=0;

	int width=0;
	int heightStart=0;
	int heightEnd=0;

	m_OffsetX=0;
	m_OffsetY=0;

	int tickCount=0;
	switch (m_CurrentState) 
	{

	case STATE_THROWING : case STATE_GOINGUP :

		width=THROWING_WIDTH;
		heightStart=THROWING_TOP;
		heightEnd=THROWING_BOTTOM;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;
		
		m_OffsetX=THROWING_OFFSET_X;
		m_OffsetY=THROWING_OFFSET_Y;
		break;

	case STATE_DYING : 

		width=DYING_WIDTH;
		heightStart=DYING_TOP;
		heightEnd=DYING_BOTTOM;

		tickCount=m_TickCount;

		//Fade out effect
		if (m_TickCount>=DYING_FRAMES)
		{
			if (m_TickCount%2==0)
			{
				tickCount=DYING_FRAMES;
			}
			else tickCount=DYING_FRAMES-1;

		}

		frame.left=width*tickCount;
		frame.right=width+width*tickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetX=DYING_OFFSET_X;
		m_OffsetY=DYING_OFFSET_Y;

		break;

	}


	MATRIX3X2 matCenter,matScale,matTranslate;

	matCenter.SetAsTranslate(-CENTER_X+m_OffsetX,-CENTER_Y+m_OffsetY);

	matScale.SetAsScale(1,1);	
	
	matTranslate.SetAsTranslate(m_Pos);

	GAME_ENGINE->SetTransformMatrix(matCenter*matScale*matTranslate*matTransform);
	
	GAME_ENGINE->DrawBitmap(m_BmpRocketDiverSpriteSheetPtr,0,0,frame);	

	if (m_AnimateSplash==true)
	{
		width=SPLASH_WIDTH;
		heightStart=SPLASH_TOP;
		heightEnd=SPLASH_BOTTOM;

		frame.left=width*m_TickCountSplash;
		frame.right=width+width*m_TickCountSplash;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		matCenter.SetAsTranslate(-SPLASH_WIDTH/2+SPLASH_OFFSET_X,-75);

		matScale.SetAsScale(1,1);	
	
		matTranslate.SetAsTranslate(m_SplashPos);

		GAME_ENGINE->SetTransformMatrix(matCenter*matScale*matTranslate*matTransform);
		
 		GAME_ENGINE->DrawBitmap(m_BmpRocketDiverSpriteSheetPtr,0,0,frame);	
	}

	GAME_ENGINE->SetTransformMatrix(matTransform);

	//GAME_ENGINE->SetColor(COLOR(255,0,255,127));
	//GAME_ENGINE->FillEllipse(m_SplashPos,3,3);

	//GAME_ENGINE->SetColor(COLOR(255,0,255,127));
	//GAME_ENGINE->FillEllipse(m_Pos,3,3);

	//GAME_ENGINE->SetColor(COLOR(0,255,0,180));
	//GAME_ENGINE->DrawLine(m_PointRay1,m_PointRay1+m_VectorRay1);
	//GAME_ENGINE->DrawLine(m_PointRay2,m_PointRay2+m_VectorRay2);

	//GAME_ENGINE->SetColor(COLOR(0,255,0,128));
	//GAME_ENGINE->DrawLine(m_PointRay3,m_PointRay3+m_VectorRay3);	
	//GAME_ENGINE->DrawLine(m_PointRay4,m_PointRay4+m_VectorRay4);

	//GAME_ENGINE->SetTransformMatrix(matTransform);

	//GAME_ENGINE->FillHitRegion(m_HitRegionPtr);
}

void RocketDiver::HandleCollision(HitRegion* hitRegionPtr)
{
	HIT hitArr[1];

	//Ray1

	if (m_CurrentState==STATE_GOINGUP || m_CurrentState==STATE_THROWING)
		m_PointRay1= DOUBLE2(m_Pos.x-5,m_Pos.y-HEIGHT);
	else
		m_PointRay1= DOUBLE2(m_Pos.x-5,m_Pos.y-CENTER_Y);
	m_VectorRay1 = DOUBLE2(0,HEIGHT);

	RaycastFullCollision(hitRegionPtr, m_PointRay1,m_VectorRay1);


	//Ray2

	if (m_CurrentState==STATE_GOINGUP || m_CurrentState==STATE_THROWING)
		m_PointRay2= DOUBLE2(m_Pos.x+5,m_Pos.y-HEIGHT);
	else
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

void RocketDiver::RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector)
{
	HIT hitArr[1];

	if (hitRegionPtr->Raycast(point, vector, hitArr, 1, -1) == 1)
	{
		if (hitArr[0].lambda>0.8)
		{
			//Vertical rays
			if (vector.x==0)
			{
				if (m_CurrentState==STATE_DYING || m_CurrentState==STATE_DEAD)
				{
					m_Pos.y-= vector.Length()*(1-hitArr[0].lambda);
					m_Velocity.y=0;
				}
				else
				{
					
					if (m_Velocity.y>=0) 						
					{
						m_CurrentState=STATE_HIDDENAGAIN;
						m_AnimateSplash=true;
						m_TickCountSplash=0;
					}
					
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

void RocketDiver::HandleSemiCollision(HitRegion* hitRegionPtr)
{
	//Do nothing
}

void RocketDiver::SetPos(int x, int y)
{
	m_Pos.x=x;
	m_Pos.y=y;
}

void RocketDiver::SetPos(DOUBLE2 pos)
{
	m_Pos.x=pos.x;
	m_Pos.y=pos.y;
}

void RocketDiver::SetXPos(int x)
{
	m_Pos.x=x;
}

void RocketDiver::SetYPos(int y)
{
	m_Pos.y=y;
}

DOUBLE2 RocketDiver::GetPos()
{
	return m_Pos;
}

void RocketDiver::SetVelocity(int x, int y)
{
	m_Velocity.x=x;
	m_Velocity.y=y;
}

void RocketDiver::SetVelocity(DOUBLE2 velocity)
{
	m_Velocity.x=velocity.x;
	m_Velocity.y=velocity.y;
}

void RocketDiver::SetXVelocity(int x)
{
	m_Velocity.x=x;
}

void RocketDiver::SetYVelocity(int y)
{
	m_Velocity.y=y;
}


DOUBLE2 RocketDiver::GetVelocity()
{
	return m_Velocity;
}

HitRegion* RocketDiver::GetHitRegion()
{
	return m_HitRegionPtr;
}


void RocketDiver::SetDirection(bool isLeft)
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

int RocketDiver::GetDirection()
{
	return m_CurrentDirection;
}


void RocketDiver::SetState(int state)
{
	if ( !(state != STATE_THROWING && state != STATE_DYING))
	{
		if (m_CurrentState !=STATE_DYING || m_CurrentState != STATE_DEAD)
		{
			m_CurrentState=state;

			m_TickCount=0;
		}
	}			
}

void RocketDiver::AddGravity(DOUBLE2 force)
{
	if (m_CurrentState!=STATE_HIDDEN) m_Velocity+=force;
}

int RocketDiver::GivePoints()
{
	if (m_PointsGiven==false && m_CurrentState==STATE_DYING)
	{
		m_PointsGiven=true;
		return POINTS;
	}

	return 0;
}

bool RocketDiver::IsAlive()
{
	if (m_CurrentState==STATE_DYING || m_CurrentState==STATE_DEAD)
	{
		return false;
	}
	
	return true;
}


bool RocketDiver::IsReadyToBeDeleted()
{
	if (m_CurrentState==STATE_DEAD)
	{
		return true;
	}
	
	return false;

}

bool RocketDiver::SetTarget(DOUBLE2 playerPos)
{
	if ( (m_Pos-playerPos).Length() <= 250)
	{
		

		m_TargetPos=playerPos;
		m_HasTarget=true;
		return true;
	}

	m_HasTarget=false;
	return false;
}

bool RocketDiver::IsHostile()
{
	return true;
}


bool RocketDiver::UsesBombs()
{
	//Uses grenades
	return false;
}


int RocketDiver::GetProjectileType()
{
	return Projectile::ROCKET;
}


void RocketDiver::SubtractHealth(double health)
{
	m_Health-=health;

	if (m_Health<0) m_Health=0;

	if (m_Health==0 && IsAlive()==true)
	{		
		m_AudioDeathScreamPtr->Play();
		m_CurrentState=STATE_DYING;
		m_TickCount=0;
	}
	
}


bool RocketDiver::ExecuteAttack()
{

	if (m_CurrentState==STATE_THROWING && m_TickCount==7 &&  m_HasExecutedAttack==false)
	{
		m_HasExecutedAttack=true;
		return true;
	}	

	return false;
}


bool RocketDiver::ExecuteSecondaryAttack(DOUBLE2 targetPos)
{
	//Do nothing
	return false;
}

void RocketDiver::GetCameraBounds(DOUBLE2 pos, int width, int height)
{
	//Do nothing
}

