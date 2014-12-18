//---------------------------
// Includes
//---------------------------
#include "Soldier.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* Soldier::m_BmpSoldierSpriteSheetPtr=nullptr;
//---------------------------
// Constructor & Destructor
//---------------------------
Soldier::Soldier(int x, int y) : m_Pos(x,y), m_Velocity(0,0), m_HitRegionPtr(nullptr), m_TickCount(0), 
	m_CurrentDirection(Soldier::DIRECTION_LEFT), m_CurrentState(0), m_Health(1),
	m_SlowDownAnimations(0),
	m_OffsetX(0), m_OffsetY(0),
	m_PointRay1(0,0), m_VectorRay1(0,0), m_PointRay2(0,0), m_VectorRay2(0,0), m_PointRay3(0,0), m_VectorRay3(0,0),
	m_PointRay4(0,0), m_VectorRay4(0,0)
	, m_TimeWhenLastExecutedAttack(5), m_ExecuteAttackingInterval(5), m_HasExecutedAttack(false), m_HasTarget(false) , 
	m_TargetPos(0,0), m_PointsGiven(false)
{
	// If not yet done, create bitmaps and set transparency color
	if (Soldier::m_BmpSoldierSpriteSheetPtr==nullptr)
	{
		Soldier::m_BmpSoldierSpriteSheetPtr= new Bitmap(IDB_SOLDIER);
	}
		

	//Randomly chooses between 4 different screams
	String path = String("./resources/Soldier_") + (rand()%4+1) + String(".wav");

	m_AudioDeathScreamPtr=new Audio(path);
	m_AudioDeathScreamPtr->SetRepeat(false);		

	m_HitRegionPtr = new HitRegion();
	m_HitRegionPtr->CreateFromRect(-CENTER_X+5, -CENTER_Y+5, 
		WIDTH-CENTER_X-5, HEIGHT -CENTER_Y);

}


Soldier::~Soldier()
{
	delete m_AudioDeathScreamPtr;
	delete m_HitRegionPtr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void Soldier::Tick(double deltaTime)
{
	m_AudioDeathScreamPtr->Tick();

	//Key input setting the speed.	

	if (m_CurrentState!=STATE_DYING && m_CurrentState!=STATE_DEAD)
	{
		m_Pos += m_Velocity*deltaTime;
	}

	// Positions the hitboxes 

	m_HitRegionPtr->SetPos(m_Pos);


	//If the soldier has a target
	if (m_HasTarget==true)
	{
		//Walks closer to target
		if (m_Pos.x-20>m_TargetPos.x) m_Velocity.x=-50;
		else if (m_Pos.x+20<m_TargetPos.x) m_Velocity.x=50;

	}

	//Face towards velocity direction
	if (m_Velocity.x<0) m_CurrentDirection=DIRECTION_LEFT;
	else if (m_Velocity.x>0) m_CurrentDirection=DIRECTION_RIGHT;

	//Can't move while throwing or knifing
	if (m_CurrentState==STATE_THROWING || m_CurrentState==STATE_KNIFING)
		m_Velocity.x=0;

	//If the soldier is moving and is not scared, dead or dying, set the state to running
	if (m_Velocity.x!=0)
	{
		if (m_CurrentState!=STATE_RUNNINGSCARED && IsAlive()==true) m_CurrentState=STATE_RUNNING;
	}

		
 		
	

	//Handles throwing intervals

	m_TimeWhenLastExecutedAttack+=1*deltaTime;

	
	if (m_CurrentState==STATE_THROWING)
	{
		

			
	}/*
	else
	{
		m_TimeWhenLastExecutedAttack=m_ExecuteAttackingInterval+0.05;
	}*/

	switch (m_CurrentState) 
	{

	case STATE_IDLE : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount>=IDLE_FRAMES)
			{
				m_TickCount=0;
			}
		}
		m_SlowDownAnimations+=IDLE_ANIMATION_SPEED*deltaTime;

		break;

	case STATE_RUNNING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;
			m_TickCount+=1;

			if (m_TickCount>=RUNNING_FRAMES)
			{
				m_TickCount=0;
			}
		}
		m_SlowDownAnimations+=RUNNING_ANIMATION_SPEED*deltaTime;

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

	case STATE_RUNNINGSCARED : 
		
        if (m_SlowDownAnimations>0)
        {
            m_SlowDownAnimations=RESET_SLOW_DOWN;			
			m_TickCount+=1;

			if (m_TickCount==RUNNINGSCARED_FRAMES)
			{
				//Restart at frame 3
				m_TickCount=2;
			}	
        }
        m_SlowDownAnimations+=RUNNINGSCARED_ANIMATION_SPEED*deltaTime;


		break;
		
	case STATE_THROWING : 
		
		if (m_SlowDownAnimations>0)
        {
            m_SlowDownAnimations=RESET_SLOW_DOWN;			
			m_TickCount+=1;

			if (m_TickCount==THROWING_FRAMES)
			{
				//ExecuteAttacking is done go back to idle
				m_TickCount=0;
				m_CurrentState=STATE_IDLE;
			}	
        }
        m_SlowDownAnimations+=THROWING_ANIMATION_SPEED*deltaTime;

		break;

	case STATE_KNIFING : 
		
		if (m_SlowDownAnimations>0)
        {
            m_SlowDownAnimations=RESET_SLOW_DOWN;			
			m_TickCount+=1;

			if (m_TickCount==KNIFING_FRAMES)
			{
				//Knifing is done go back to idle
				m_TickCount=0;
				m_CurrentState=STATE_IDLE;
			}	
        }
        m_SlowDownAnimations+=KNIFING_ANIMATION_SPEED*deltaTime;

		break;

	}
	
}

void Soldier::Paint(MATRIX3X2 matTransform)
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

	int tickCount=0;
	switch (m_CurrentState) 
	{

	case STATE_IDLE : 

		width=IDLE_WIDTH;
		heightStart=IDLE_TOP;
		heightEnd=IDLE_BOTTOM;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;
		
		break;

	case STATE_RUNNING : 

		width=RUNNING_WIDTH;
		heightStart=RUNNING_TOP;
		heightEnd=RUNNING_BOTTOM;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

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

	case STATE_RUNNINGSCARED : 
		
		width=RUNNINGSCARED_WIDTH;
		heightStart=RUNNINGSCARED_TOP;
		heightEnd=RUNNINGSCARED_BOTTOM;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;

		m_OffsetY=RUNNINGSCARED_OFFSET_Y;
		
		break;

	case STATE_THROWING : 
		
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
		
	case STATE_KNIFING : 
					
		width=KNIFING_WIDTH;
		heightStart=KNIFING_TOP;
		heightEnd=KNIFING_BOTTOM;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightEnd;
		
		break;

	}


	MATRIX3X2 matCenter,matScale,matTranslate;

	matCenter.SetAsTranslate(-CENTER_X+m_OffsetX,-CENTER_Y+m_OffsetY);

	matScale.SetAsScale(1,1);	

	//Animations pointing to the right
	if ( (m_CurrentState==STATE_KNIFING || m_CurrentState==STATE_THROWING) && m_CurrentDirection==DIRECTION_LEFT) 
		matScale.SetAsScale(-1,1);
	else if ( (m_CurrentState==STATE_KNIFING || m_CurrentState==STATE_THROWING) && m_CurrentDirection==DIRECTION_RIGHT) 
		matScale.SetAsScale(1,1);
	
	//Animations pointing to the left
	if ( (m_CurrentState==STATE_IDLE || m_CurrentState==STATE_DYING || m_CurrentState==STATE_RUNNINGSCARED) 
		&& m_CurrentDirection==DIRECTION_LEFT) 
		matScale.SetAsScale(1,1);
	else if ( (m_CurrentState==STATE_IDLE || m_CurrentState==STATE_DYING || m_CurrentState==STATE_RUNNINGSCARED || 
		m_CurrentState==STATE_RUNNING) && m_CurrentDirection==DIRECTION_RIGHT) 
		matScale.SetAsScale(-1,1);

	
	matTranslate.SetAsTranslate(m_Pos);

	GAME_ENGINE->SetTransformMatrix(matCenter*matScale*matTranslate*matTransform);
	
	GAME_ENGINE->DrawBitmap(m_BmpSoldierSpriteSheetPtr,0,0,frame);	

 //   GAME_ENGINE->SetColor(COLOR(255,0,255,127));
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

void Soldier::HandleCollision(HitRegion* hitRegionPtr)
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

void Soldier::RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector)
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

void Soldier::HandleSemiCollision(HitRegion* hitRegionPtr)
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

void Soldier::RaycastSemiCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector)
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

void Soldier::SetPos(int x, int y)
{
	m_Pos.x=x;
	m_Pos.y=y;
}

void Soldier::SetPos(DOUBLE2 pos)
{
	m_Pos.x=pos.x;
	m_Pos.y=pos.y;
}

void Soldier::SetXPos(int x)
{
	m_Pos.x=x;
}

void Soldier::SetYPos(int y)
{
	m_Pos.y=y;
}

DOUBLE2 Soldier::GetPos()
{
	return m_Pos;
}

void Soldier::SetVelocity(int x, int y)
{
	m_Velocity.x=x;
	m_Velocity.y=y;
}

void Soldier::SetVelocity(DOUBLE2 velocity)
{
	m_Velocity.x=velocity.x;
	m_Velocity.y=velocity.y;
}

void Soldier::SetXVelocity(int x)
{
	m_Velocity.x=x;
}

void Soldier::SetYVelocity(int y)
{
	m_Velocity.y=y;
}


DOUBLE2 Soldier::GetVelocity()
{
	return m_Velocity;
}

HitRegion* Soldier::GetHitRegion()
{
	return m_HitRegionPtr;
}


void Soldier::SetDirection(bool isLeft)
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

int Soldier::GetDirection()
{
	return m_CurrentDirection;
}


void Soldier::SetState(int state)
{
	if ( !(state != STATE_IDLE && state != STATE_RUNNING && state != STATE_RUNNINGSCARED && state != STATE_THROWING && state != STATE_DYING))
	{
		if (m_CurrentState !=STATE_DYING || m_CurrentState != STATE_DEAD)
		{
			m_CurrentState=state;

			m_TickCount=0;
		}
	}			
}

void Soldier::AddGravity(DOUBLE2 force)
{
	m_Velocity+=force;
}

int Soldier::GivePoints()
{
	if (m_PointsGiven==false && m_CurrentState==STATE_DYING)
	{
		m_PointsGiven=true;
		return POINTS;
	}

	return 0;
}

bool Soldier::IsAlive()
{
	if (m_CurrentState==STATE_DYING || m_CurrentState==STATE_DEAD)
	{
		return false;
	}
	
	return true;
}


bool Soldier::IsReadyToBeDeleted()
{
	if (m_CurrentState==STATE_DEAD)
	{
		return true;
	}
	
	return false;

}

bool Soldier::SetTarget(DOUBLE2 playerPos)
{
	if ( (m_Pos-playerPos).Length() <= 300)
	{
		

		m_TargetPos=playerPos;
		m_HasTarget=true;
		return true;
	}

	m_HasTarget=false;
	return false;
}

bool Soldier::IsHostile()
{
	return true;
}


bool Soldier::UsesBombs()
{
	//Uses grenades
	return false;
}

int Soldier::GetProjectileType()
{
	return Projectile::GRENADE;
}

void Soldier::SubtractHealth(double health)
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


bool Soldier::ExecuteAttack()
{

	if (m_TimeWhenLastExecutedAttack>m_ExecuteAttackingInterval)
	{
		m_TimeWhenLastExecutedAttack=0;
		m_HasExecutedAttack=false;

		m_TickCount=0;
		m_CurrentState=STATE_THROWING;
		
	}

	if (m_CurrentState==STATE_THROWING && m_TickCount==9 &&  m_HasExecutedAttack==false)
	{
		m_HasExecutedAttack=true;
		return true;
	}	

	return false;
}


bool Soldier::ExecuteSecondaryAttack(DOUBLE2 targetPos)
{
	if (m_CurrentState!=STATE_DYING && m_CurrentState!=STATE_DEAD)
	{

		DOUBLE2 length = targetPos-m_Pos;

		if (length.Length() <= 40)
		{
			//Reset tickcount at the start of the knifing process
			if (m_CurrentState!=STATE_KNIFING) m_TickCount=0;

			if (targetPos.x < m_Pos.x) m_CurrentDirection=DIRECTION_LEFT;
			else m_CurrentDirection=DIRECTION_RIGHT;

			m_CurrentState=STATE_KNIFING;
			
			if (m_CurrentState==STATE_KNIFING && m_TickCount == KNIFING_FRAMES -1)
				return true;	
		}

		
	}
	return false;

}

void Soldier::GetCameraBounds(DOUBLE2 pos, int width, int height)
{

}

