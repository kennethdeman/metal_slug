//---------------------------
// Includes
//---------------------------
#include "Prisoner.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* Prisoner::m_BmpPrisonerSpriteSheetPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
Prisoner::Prisoner(int x, int y) : m_Pos(x,y), m_Velocity(0,0), m_HitRegionPtr(nullptr), 
	 m_TickCount(0),
	m_CurrentDirection(Prisoner::DIRECTION_LEFT), m_CurrentState(0),
	m_SlowDownAnimations(0),m_Health(1),
	m_OffsetX(0), m_OffsetY(0),
	m_PointRay1(0,0), m_VectorRay1(0,0), m_PointRay2(0,0), m_VectorRay2(0,0), m_PointRay3(0,0), m_VectorRay3(0,0),
	m_PointRay4(0,0), m_VectorRay4(0,0), 

	 m_PointsGiven(false)
{
	// If not yet done, create bitmaps and set transparency color
	if (Prisoner::m_BmpPrisonerSpriteSheetPtr==nullptr)
	{
		Prisoner::m_BmpPrisonerSpriteSheetPtr= new Bitmap(IDB_PRISONER);
	}
	

	m_HitRegionPtr = new HitRegion();
	m_HitRegionPtr->CreateFromRect(-CENTER_X+10, -CENTER_Y, 
		WIDTH-CENTER_X-10, HEIGHT -CENTER_Y);

}

Prisoner::~Prisoner()
{
	delete m_HitRegionPtr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void Prisoner::Tick(double deltaTime)
{
	//Key input setting the speed.	

	m_Pos.x += m_Velocity.x*deltaTime;
	  
	m_Pos.y += m_Velocity.y*deltaTime;

	//Positions the hitboxes

	m_HitRegionPtr->SetPos(m_Pos);	

	switch (m_CurrentState) 
	{

	case STATE_BOUND : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount==BOUND_FRAMES)
			{
				m_TickCount=0;

				//Makes the prisoner randomly wave to the player
				int urgeToWave=rand()%5;
				if (urgeToWave==0)
				{
					m_CurrentState = STATE_WAVING;
				}
			}

			
		}		

		m_SlowDownAnimations+=BOUND_ANIMATION_SPEED*deltaTime;		

		break;

	case STATE_WAVING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount==WAVING_FRAMES)
			{
				m_TickCount=0;
				m_CurrentState = STATE_BOUND;
			}
		}
		m_SlowDownAnimations+=WAVING_ANIMATION_SPEED*deltaTime;	

		break;

	case STATE_ESCAPING : 
		
		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount==ESCAPING_FRAMES)
			{
				m_TickCount=0;
				
				m_CurrentState=STATE_WALKING;
			}			
			

		}
		m_SlowDownAnimations+=ESCAPING_ANIMATION_SPEED*deltaTime;	
        
		break;
		
	case STATE_WALKING : 
		
		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			SetXVelocity(-45);
			m_TickCount+=1;

			if (m_TickCount>=WALKING_FRAMES+1)
			{
				m_TickCount=0;
			}			
			
			//Code to show reward animations
			int rewardPlayer=rand()%5;
			if (rewardPlayer==0)
			{
				m_TickCount=0;
				m_CurrentState = STATE_REWARDING;
			}

		}
		m_SlowDownAnimations+=WALKING_ANIMATION_SPEED*deltaTime;	
	
		break;

	case STATE_REWARDING : 
		
		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			SetXVelocity(0);
			m_TickCount+=1;

			if (m_TickCount==REWARDING_FRAMES)
			{
				m_TickCount=0;
				m_CurrentState = STATE_RUNNING;
			}			
			
		}
		m_SlowDownAnimations+= REWARDING_ANIMATION_SPEED*deltaTime;	
	
		break;

	case STATE_RUNNING : 
		
		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			SetXVelocity(-150);
			m_TickCount+=1;

			if (m_TickCount==RUNNING_FRAMES)
			{
				m_TickCount=0;
			}			
			
		}
		m_SlowDownAnimations+=RUNNING_ANIMATION_SPEED*deltaTime;	
	
		break;

	}


}

void Prisoner::Paint(MATRIX3X2 matTransform)
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

	int tickCount;

	switch (m_CurrentState) 
	{

	case STATE_BOUND : 

		width=BOUND_WIDTH;
		heightStart=BOUND_TOP;
		heightEnd=BOUND_BOTTOM;

		tickCount=m_TickCount;

		//Makes the frames go from 1 to 5 and back from 4 to 1.
		if (m_TickCount>4)
			tickCount=9-m_TickCount;

		frame.left=width*tickCount;
		frame.right=width+width*tickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetY=BOUND_OFFSET_Y;		

		break;

	case STATE_WAVING : 

		width=WAVING_WIDTH;
		heightStart=WAVING_TOP;
		heightEnd=WAVING_BOTTOM;
		
		tickCount=m_TickCount;

		//Makes the frames go from 1 to 4 and back from 3 to 1.
		if (m_TickCount>3)
			tickCount=7-m_TickCount;

		frame.left=width*tickCount;
		frame.right=width+width*tickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetY=WAVING_OFFSET_Y;		
	
		break;

	case STATE_ESCAPING : 
		
		width=ESCAPING_WIDTH;
		heightStart=ESCAPING_TOP;
		heightEnd=ESCAPING_BOTTOM;

		//Animations starts at the 5th image in the row -> m_TickCount+4
		tickCount=m_TickCount+4;

		frame.left=width*tickCount;
		frame.right=width+width*tickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetY=ESCAPING_OFFSET_Y;
		
		

		break;
		
	case STATE_WALKING : 
		
		width=WALKING_WIDTH;
		heightStart=WALKING_TOP;
		heightEnd=WALKING_BOTTOM;		

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetX=0;
		m_OffsetY=WALKING_OFFSET_Y;		
	
		break;

	case STATE_REWARDING : 
		
		width=REWARDING_WIDTH;
		heightStart=REWARDING_TOP;
		heightEnd=REWARDING_BOTTOM;		

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetX=0;
		m_OffsetY=REWARDING_OFFSET_Y;

		break;

	case STATE_RUNNING : 
		
		width=RUNNING_WIDTH;
		heightStart=RUNNING_TOP;
		heightEnd=RUNNING_BOTTOM;		

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetX=0;
		m_OffsetY=RUNNING_OFFSET_Y;		
	
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
	
	GAME_ENGINE->DrawBitmap(m_BmpPrisonerSpriteSheetPtr,0,0,frame);	

	if (m_TickCount>=3 && m_CurrentState==STATE_ESCAPING)
	{			

		frame.left=0;
		frame.right=50;		
		frame.top=192;
		frame.bottom=230;

		m_OffsetX=-5;
		m_OffsetY=2;

		matCenter.SetAsTranslate(-CENTER_X+m_OffsetX,-CENTER_Y+m_OffsetY);

		GAME_ENGINE->SetTransformMatrix(matCenter*matScale*matTranslate*matTransform);

		GAME_ENGINE->DrawBitmap(m_BmpPrisonerSpriteSheetPtr,0,0,frame);	
		
	}

	
	//GAME_ENGINE->SetColor(COLOR(255,0,255,127));
	//GAME_ENGINE->FillEllipse(CENTER_X,CENTER_Y,3,3);

	//GAME_ENGINE->SetTransformMatrix(matTransform);

	//GAME_ENGINE->SetColor(COLOR(0,255,0,180));
	//GAME_ENGINE->DrawLine(m_PointRay1,m_PointRay1+m_VectorRay1);
	//GAME_ENGINE->DrawLine(m_PointRay2,m_PointRay2+m_VectorRay2);

	//GAME_ENGINE->SetColor(COLOR(0,255,0,128));
	//GAME_ENGINE->DrawLine(m_PointRay3,m_PointRay3+m_VectorRay3);	
	//GAME_ENGINE->DrawLine(m_PointRay4,m_PointRay4+m_VectorRay4);

	//GAME_ENGINE->SetTransformMatrix(matTransform);

	//GAME_ENGINE->FillHitRegion(m_HitRegionPtr);
}

void Prisoner::HandleCollision(HitRegion* hitRegionPtr)
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

void Prisoner::RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector)
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
				m_Velocity.y=0;
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

void Prisoner::HandleSemiCollision(HitRegion* hitRegionPtr)
{
	HIT hitArr[1];

	//Ray1

	m_PointRay1= DOUBLE2(m_Pos.x-5,m_Pos.y-CENTER_Y);
	m_VectorRay1 = DOUBLE2(0,HEIGHT);

	RaycastSemiCollision(hitRegionPtr, m_PointRay1,m_VectorRay1);

	//Ray2

	m_PointRay2= DOUBLE2(m_Pos.x+5,m_Pos.y-CENTER_Y);
	m_VectorRay2 = DOUBLE2(0,HEIGHT);

	RaycastSemiCollision(hitRegionPtr, m_PointRay2,m_VectorRay2);

	m_PointRay1= DOUBLE2(m_Pos.x-5,m_Pos.y-CENTER_Y);	
	m_PointRay2= DOUBLE2(m_Pos.x+5,m_Pos.y-CENTER_Y);	
}

void Prisoner::RaycastSemiCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector)
{
	HIT hitArr[1];

	if (hitRegionPtr->Raycast(point, vector, hitArr, 1, -1) == 1)
	{
		if (m_Velocity.y >= 0 && hitArr[0].lambda>0.8)
		{
			
			m_Pos.y-= vector.Length()*(1-hitArr[0].lambda);
			m_Velocity.y=0;			

		}		
	}
}

void Prisoner::AddGravity(DOUBLE2 force)
{
	m_Velocity+=force;
}

int Prisoner::GivePoints()
{
	if (m_PointsGiven==false && m_CurrentState!=STATE_BOUND)
	{
		m_PointsGiven=true;
		return POINTS;
	}

	return 0;
}

void Prisoner::SetPos(int x, int y)
{
	m_Pos.x=x;
	m_Pos.y=y;
}

void Prisoner::SetPos(DOUBLE2 pos)
{
	m_Pos.x=pos.x;
	m_Pos.y=pos.y;
}

void Prisoner::SetXPos(int x)
{
	m_Pos.x=x;
}

void Prisoner::SetYPos(int y)
{
	m_Pos.y=y;
}

DOUBLE2 Prisoner::GetPos()
{
	return m_Pos;
}

void Prisoner::SetVelocity(int x, int y)
{
	m_Velocity.x=x;
	m_Velocity.y=y;
}

void Prisoner::SetVelocity(DOUBLE2 velocity)
{
	m_Velocity.x=velocity.x;
	m_Velocity.y=velocity.y;
}

void Prisoner::SetXVelocity(int x)
{
	m_Velocity.x=x;
}

void Prisoner::SetYVelocity(int y)
{
	m_Velocity.y=y;
}


DOUBLE2 Prisoner::GetVelocity()
{
	return m_Velocity;
}

HitRegion* Prisoner::GetHitRegion()
{
	return m_HitRegionPtr;
}


void Prisoner::SetDirection(bool isLeft)
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

int Prisoner::GetDirection()
{
	return m_CurrentDirection;
}


void Prisoner::SetState(int state)
{

	if (state != STATE_BOUND && state != STATE_WAVING && state != STATE_ESCAPING && state != STATE_WALKING && state != STATE_REWARDING && state != STATE_RUNNING )
	{
		m_CurrentState=STATE_BOUND;
	}
	else
	{
		if (m_CurrentState !=state)
		{
			m_CurrentState=state;

			m_TickCount=0;
		}
	}			
}

bool Prisoner::IsAlive()
{	
	if (m_CurrentState==STATE_BOUND)
		return true;

	//False if freed
	return false;
}


bool Prisoner::IsReadyToBeDeleted()
{
	return m_CurrentState==STATE_OUTOFCAMERA;
}

bool Prisoner::SetTarget(DOUBLE2 targetPos)
{
	return false;
}

bool Prisoner::IsHostile()
{
	return false;
}

bool Prisoner::UsesBombs()
{
	return false;
}

int Prisoner::GetProjectileType()
{
	return Projectile::NOTHING;
}

	
void Prisoner::SubtractHealth(double health)
{
	m_Health-=health;

	if (m_Health<0) m_Health=0;

	if (m_Health==0 && IsAlive()==true)
	{		
		m_CurrentState=STATE_ESCAPING;
		m_TickCount=0;
	}
}

	
bool Prisoner::ExecuteAttack()
{
	//No Attack
	return false;
}

bool Prisoner::ExecuteSecondaryAttack(DOUBLE2 targetPos)
{
	//No Secondary Attack
	return false;
}


void Prisoner::GetCameraBounds(DOUBLE2 pos, int width, int height)
{
	//If the prisoner is left of the camera -> set ready to be deleted
	if (m_Pos.x> pos.x) m_CurrentState=STATE_OUTOFCAMERA;
}
