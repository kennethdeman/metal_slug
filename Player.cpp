//---------------------------
// Includes
//---------------------------
#include "Player.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* Player::m_BmpPlayerBodySpriteSheetPtr=nullptr, *Player::m_BmpPlayerLegsSpriteSheetPtr;


//---------------------------
// Constructor & Destructor
//---------------------------
Player::Player(int x, int y) : m_Pos(x,y), m_Velocity(0,0), m_CameraPos(0,0),
	m_HitRegionPtr(nullptr), m_TickCount(0),
	m_TickCountAlt(0), m_CurrentDirection(DIRECTION_RIGHT), 
	m_CurrentMovement(0), 	m_CurrentState(0), m_CurrentAim(0), m_CurrentAction(0), 
	m_SlowDownAnimations(0), m_SlowDownAnimationsAlt(0),
	m_BodyOffsetX(0), m_BodyOffsetY(0), m_LegsOffsetX(0), m_LegsOffsetY(0), m_IsCrouchingAnimationDone(false),
	m_PointRay1(0,0), m_VectorRay1(0,0), m_PointRay2(0,0), m_VectorRay2(0,0), m_PointRay3(0,0), m_VectorRay3(0,0),
	m_PointRay4(0,0), m_VectorRay4(0,0), m_Lives(3),   m_AimingAngle(0),
	m_TimeWhenLastShot(1.3), m_TimeWhenLastThrown(0.8), m_InvulnerabilityTimer(0), 
	m_ShootingInterval(1.4), m_ThrowingInterval(0.9),
	m_IsInvulnerable(false),m_TickShots(0),m_AmountOfBombs(10),m_Godmode(false)

{										 									 
	// If not yet done, create bitmaps and set transparency color
	if (Player::m_BmpPlayerBodySpriteSheetPtr==nullptr)
	{
		Player::m_BmpPlayerBodySpriteSheetPtr= new Bitmap(IDB_MARCOBODY);
	}
	if (Player::m_BmpPlayerLegsSpriteSheetPtr==nullptr)
	{					 
		Player::m_BmpPlayerLegsSpriteSheetPtr= new Bitmap(IDB_MARCOLEGS);
	}

	
	m_AudioDeathScreamPtr=new Audio("./resources/Marco_1.wav");
	m_AudioDeathScreamPtr->SetRepeat(false);
	

	for (int count=0; count<MAX_SHOTS; count++)
	{
		m_AudioPistolShotPtrArr[count]=new Audio("./resources/PistolShot.wav");
	}


	m_HitRegionPtr = new HitRegion();
	m_HitRegionPtr->CreateFromRect(-CENTER_X, -CENTER_Y, 
		WIDTH-10-CENTER_X, HEIGHT -CENTER_Y);

	//m_AudioDeathScreamPtr= new Audio("./resources/Marco_1.wav");	
	//m_AudioDeathScreamPtr->SetRepeat(false);
	//m_AudioDeathScreamPtr->Play();
}

Player::~Player()
{
	delete m_AudioDeathScreamPtr;

	for (int count=0; count<MAX_SHOTS; count++)
	{
		delete m_AudioPistolShotPtrArr[count];
	}

	delete m_HitRegionPtr;

}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void Player::Tick(double deltaTime)
{

	m_AudioDeathScreamPtr->Tick();
	for (int count=0; count<MAX_SHOTS; count++)
	{
		m_AudioPistolShotPtrArr[count]->Tick();
	}

	//Key input setting the speed.	

	m_Pos += m_Velocity*deltaTime;

	//Positions the hitboxes

	m_HitRegionPtr->SetPos(m_Pos);	



	//Handles shooting intervals

	if (m_CurrentAction==ACTION_SHOOTING)
	{
		m_TimeWhenLastShot+=1*deltaTime;

		if (m_TimeWhenLastShot>=m_ShootingInterval+0.1)
			m_TimeWhenLastShot=0;		
			//Action is put back on nothing during shooting animate
	}
	else
	{
		m_TimeWhenLastShot=m_ShootingInterval+0.05 ;
	}

	//Determines aiming angle

	if ( m_CurrentAim==AIM_UP)
	{
		if (m_AimingAngle==M_PI)
		{
			m_AimingAngle= m_AimingAngle*-1;
		}

		if (m_AimingAngle<-M_PI/2)
			m_AimingAngle+=2*deltaTime; 

		else if (m_AimingAngle>-M_PI/2)
			m_AimingAngle-=2*deltaTime;

		//else if (abs(m_AimingAngle-M_PI/2)<0.1)		
			m_AimingAngle=-M_PI/2;
		
	}
	else if ( m_CurrentAim==AIM_DOWN && m_CurrentState==STATE_INTHEAIR)
	{
		m_AimingAngle=-M_PI-M_PI/2;
	}
	else if ( (m_CurrentAim==AIM_NORMAL || m_CurrentAim==AIM_DOWN) && m_CurrentDirection==DIRECTION_LEFT)
	{
		m_AimingAngle=M_PI;
	}
	else if ( (m_CurrentAim==AIM_NORMAL || m_CurrentAim==AIM_DOWN) && m_CurrentDirection==DIRECTION_RIGHT)
	{
		m_AimingAngle=0;
	}

	//Handles throwing intervals

	if (m_CurrentAction==ACTION_THROWING)
	{
		m_TimeWhenLastThrown+=1*deltaTime;

		if (m_TimeWhenLastThrown>=m_ThrowingInterval+0.1)
		{
			m_TimeWhenLastThrown=0;
			//Action is put back on nothing during throwing animate
		}

			
	}
	else
	{
		m_TimeWhenLastThrown=m_ThrowingInterval+0.05;
	}

	// Checks if the player is still crouching
	if (m_CurrentAim!=AIM_DOWN)
	{
		m_IsCrouchingAnimationDone=false;
	}


	//Sets correct velocity
	if (m_CurrentMovement==MOVEMENT_MOVING)
	{
		int direction=1;
		if (m_CurrentDirection==DIRECTION_LEFT) direction=-1;

		if (m_CurrentAim==AIM_DOWN && m_CurrentAction==ACTION_SHOOTING && m_CurrentState==STATE_ONTHEGROUND)
		{
			m_Velocity.x=0;
		}
		else if (m_CurrentAim==AIM_DOWN && m_CurrentState==STATE_ONTHEGROUND)
		{
			m_Velocity.x=75*direction;
		}
		else m_Velocity.x=150*direction;

	}	
	else if (m_CurrentMovement==MOVEMENT_STAYING)
		m_Velocity.x=0;


	//Dying
	if (m_CurrentAction==ACTION_DYING)
	{
		AnimateDying(deltaTime);
	}
	//Crouching on the ground
	else if (m_CurrentAim==AIM_DOWN && m_CurrentState!=STATE_INTHEAIR)
	{
		if (m_CurrentAction == ACTION_NOTHING)
		{

			if (m_CurrentMovement==MOVEMENT_STAYING)
			{
				if (m_IsCrouchingAnimationDone==false)
				{
					AnimateUprightToCrouch(deltaTime);
				}
				else
				{
					AnimateCrouchIdle(deltaTime);
				}
			}
			else
			{
				AnimateCrouchWalking(deltaTime);
			}
			

		}
		else if(m_CurrentAction == ACTION_SHOOTING)
		{
			AnimateCrouchShooting(deltaTime);
		}

	}
	else //If not crouching on the ground or dying
	{
		
		//Body
		if(m_CurrentAction == ACTION_NOTHING)
			AnimateBodyIdle(deltaTime);
		else if(m_CurrentAction == ACTION_SHOOTING)
		{
			// Aim can be down when in the air			
			if(m_CurrentAim == AIM_DOWN)
				AnimateBodyShootingDownward(deltaTime);
			else if(m_CurrentAim == AIM_UP)
				AnimateBodyShootingUpward(deltaTime);
			else if(m_CurrentAim == AIM_NORMAL)
				AnimateBodyShootingForward(deltaTime);
		}
		else if(m_CurrentAction == ACTION_KNIFING)
		{
			AnimateBodyKnifing(deltaTime);
		}
		else if(m_CurrentAction == ACTION_THROWING)
		{
			AnimateBodyThrowing(deltaTime);
		}

		//Legs
		if( m_CurrentState==STATE_INTHEAIR)
			AnimateLegsJumping(deltaTime);
		else if(m_CurrentMovement == MOVEMENT_MOVING)
			AnimateLegsRunning(deltaTime);
		
	}
	
	if (m_InvulnerabilityTimer<=0) m_IsInvulnerable=m_Godmode;
	else m_InvulnerabilityTimer-=1*deltaTime;

}

void Player::Paint(MATRIX3X2 matTransform)
{	
	RECT frameBody;
	frameBody.left=0;
	frameBody.right=WIDTH;
	frameBody.top=165;
	frameBody.bottom=165+HEIGHT;

	RECT frameLegs;
	frameLegs.left=0;
	frameLegs.right=0;
	frameLegs.top=0;
	frameLegs.bottom=0;

	m_BodyOffsetX=0;
	m_BodyOffsetY=0;
	m_LegsOffsetX=0;
	m_LegsOffsetY=0;

	//Dying
	if (m_CurrentAction==ACTION_DYING)
	{
		frameBody=GetFrameDying();
	}
	//Crouching on the ground
	else if (m_CurrentAim==AIM_DOWN && m_CurrentState!=STATE_INTHEAIR)
	{
		if (m_CurrentAction == ACTION_NOTHING)
		{

			if (m_CurrentMovement==MOVEMENT_STAYING)
			{
				if (m_IsCrouchingAnimationDone==false)
				{
					frameBody=GetFrameUprightToCrouch();
					
				}
				else
				{
					frameBody=GetFrameCrouchIdle();
				}
			}
			else
			{
				frameBody=GetFrameCrouchWalking();	
			}
			

		}
		else if(m_CurrentAction == ACTION_SHOOTING)
		{
			frameBody=GetFrameCrouchShooting();
		}

	}
	else //If not crouching on the ground or dying
	{
		
		//Body
		if(m_CurrentAction == ACTION_NOTHING)
			frameBody=GetFrameBodyIdle();
		else if(m_CurrentAction == ACTION_SHOOTING)
		{
			// Aim can be down when in the air
			if(m_CurrentAim == AIM_DOWN)
				frameBody=GetFrameBodyShootingDownward();
			else if(m_CurrentAim == AIM_UP)
			{
				frameBody=GetFrameBodyShootingUpward();					
				m_BodyOffsetY=-42;

			}
			else if(m_CurrentAim == AIM_NORMAL)
			{
				frameBody=GetFrameBodyShootingForward();
				m_BodyOffsetY=1;
			}	
		}
		else if(m_CurrentAction == ACTION_KNIFING)
		{
			frameBody=GetFrameBodyKnifing();
		}
		else if(m_CurrentAction == ACTION_THROWING)
		{
			frameBody=GetFrameBodyThrowing();
		}

		//Legs
		if( m_CurrentState==STATE_INTHEAIR)
		{

			//Body is lifted 2 pixels when jumping
			if (m_CurrentAim!=AIM_UP)
			{
				
				m_BodyOffsetY=-2;
			}
			else
				m_BodyOffsetY-=2;

			frameLegs=GetFrameLegsJumping();
		}
		else if(m_CurrentMovement == MOVEMENT_MOVING)
		{

			//Body is lifted 2 pixels when running
			if (m_CurrentAim!=AIM_UP)
			{				
				m_BodyOffsetY=-2;
			}
			else
				m_BodyOffsetY-=2;

			frameLegs=GetFrameLegsRunning();
		}			
		else if (m_CurrentState==STATE_ONTHEGROUND)
			frameLegs=GetFrameLegsIdle();
		
	}


	//Suggestion teacher

	/*MATRIX3X2 matBody;
	matBody.SetAsTranslate(-(frameBody.right - frameBody.left)/2, -frameBody.top - frameBody.bottom)
	GAME_ENGINE->SetTransformMatrix(matView*/

	MATRIX3X2 matCenter,matScale,matTranslate, matBodyCenter, matLegsCenter;

	matCenter.SetAsTranslate(-CENTER_X,-CENTER_Y);	
	matBodyCenter.SetAsTranslate(m_BodyOffsetX,m_BodyOffsetY);
	matLegsCenter.SetAsTranslate(m_LegsOffsetX,m_LegsOffsetY);

	if (m_CurrentDirection==DIRECTION_LEFT)
	{
		matScale.SetAsScale(-1,1);
	}
	else
	{
		matScale.SetAsScale(1,1);
	}
	
	matTranslate.SetAsTranslate(m_Pos);

	GAME_ENGINE->SetTransformMatrix(matCenter*matLegsCenter*matScale*matTranslate*matTransform);
	GAME_ENGINE->DrawBitmap(m_BmpPlayerLegsSpriteSheetPtr,0,0,frameLegs);

	GAME_ENGINE->SetTransformMatrix(matCenter*matBodyCenter*matScale*matTranslate*matTransform);
	GAME_ENGINE->DrawBitmap(m_BmpPlayerBodySpriteSheetPtr,0,0,frameBody);	

	/*GAME_ENGINE->SetColor(COLOR(255,0,255,127));
	GAME_ENGINE->FillEllipse(CENTER_X,CENTER_Y,3,3);

	GAME_ENGINE->SetTransformMatrix(matTransform);

	GAME_ENGINE->SetColor(COLOR(0,255,0,180));
	GAME_ENGINE->DrawLine(m_PointRay1,m_PointRay1+m_VectorRay1);
	GAME_ENGINE->DrawLine(m_PointRay2,m_PointRay2+m_VectorRay2);

	GAME_ENGINE->SetColor(COLOR(0,255,0,128));
	GAME_ENGINE->DrawLine(m_PointRay3,m_PointRay3+m_VectorRay3);	
	GAME_ENGINE->DrawLine(m_PointRay4,m_PointRay4+m_VectorRay4);*/

	//GAME_ENGINE->FillHitRegion(m_HitRegionPtr);	

}

void Player::HandleCollision(HitRegion* hitRegionPtr)
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

void Player::RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector)
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
				SetState(STATE_ONTHEGROUND);
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

void Player::HandleSemiCollision(HitRegion* hitRegionPtr)
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

void Player::RaycastSemiCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector)
{
	HIT hitArr[1];

	if (hitRegionPtr->Raycast(point, vector, hitArr, 1, -1) == 1)
	{
		if (m_Velocity.y >= 0 && hitArr[0].lambda>0.8)
		{
			
			m_Pos.y-= vector.Length()*(1-hitArr[0].lambda);
			m_Velocity.y=0;
			SetState(STATE_ONTHEGROUND);

		}		
	}
}


void Player::HandleBoundaryCollision(int x,int width)
{
	//Prevent the character from leaving the level
	if (m_Pos.x < x + CENTER_X){
		m_Pos.x=x + CENTER_X;
	}
	if (m_Pos.x  > width - CENTER_X)
	{
		m_Pos.x= width - CENTER_X;
	}
}

void Player::MoveLeft()
{
	if (m_CurrentAction!=ACTION_DYING)
	{
		m_CurrentDirection=DIRECTION_LEFT;
		m_CurrentMovement=MOVEMENT_MOVING;
	}	
	else m_CurrentMovement=MOVEMENT_STAYING;
}

void Player::MoveRight()
{
	if (m_CurrentAction!=ACTION_DYING)
	{
		m_CurrentDirection=DIRECTION_RIGHT;
		m_CurrentMovement=MOVEMENT_MOVING;
	}	
	else m_CurrentMovement=MOVEMENT_STAYING;
}

void Player::StopMoving()
{
	m_CurrentMovement=MOVEMENT_STAYING;
}


void Player::Jump()
{
	if (m_CurrentState==STATE_ONTHEGROUND && m_CurrentAction!=ACTION_DYING)
	{
		m_Velocity.y=-250;
		m_TickCount=0;
		m_TickCountAlt=0;
		m_CurrentState=STATE_INTHEAIR;
	}	
}

void Player::AddAmountOfBombs(int amount)
{
	if (amount>0) m_AmountOfBombs+=amount;
}


int Player::GetAmountOfBombs()
{
	return m_AmountOfBombs;
}


void Player::ToggleGodmode()
{
	m_Godmode=!m_Godmode;

	if (m_Godmode==true) 
	{
		m_CurrentAction=ACTION_NOTHING;
		m_Lives=3;
	}
	
}


void Player::SetPos(int x, int y)
{
	m_Pos.x=x;
	m_Pos.y=y;
}

void Player::SetPos(DOUBLE2 pos)
{
	m_Pos.x=pos.x;
	m_Pos.y=pos.y;
}

void Player::SetXPos(int x)
{
	m_Pos.x=x;
}

void Player::SetYPos(int y)
{
	m_Pos.y=y;
}

DOUBLE2 Player::GetPos()
{
	return m_Pos;
}

void Player::SetVelocity(int x, int y)
{
	m_Velocity.x=x;
	m_Velocity.y=y;
}

void Player::SetVelocity(DOUBLE2 velocity)
{
	m_Velocity.x=velocity.x;
	m_Velocity.y=velocity.y;
}

void Player::SetXVelocity(int x)
{
	m_Velocity.x=x;
}

void Player::SetYVelocity(int y)
{
	m_Velocity.y=y;
}


DOUBLE2 Player::GetVelocity()
{
	return m_Velocity;
}

HitRegion* Player::GetHitRegion()
{
	return m_HitRegionPtr;
}

void Player::SetDirection(bool isLeft)
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

void Player::SetMovement(bool isMoving)
{
	if (isMoving==true)
	{
		m_CurrentMovement=MOVEMENT_MOVING;
	}
	else
	{
		m_CurrentMovement=MOVEMENT_STAYING;
	}
}

void Player::SetState(bool isInTheAir)
{
	if (isInTheAir==true)
	{
		m_TickCount=0;
		m_TickCountAlt=0;
		m_CurrentState=STATE_INTHEAIR;
		
	}
	else
	{
		m_CurrentState=STATE_ONTHEGROUND;
	}

}

void Player::SetAim(int aim)
{
	if (aim==AIM_NORMAL || aim==AIM_UP || aim==AIM_DOWN)
	{
		if (m_CurrentAim!=aim && m_CurrentAction!=ACTION_DYING)
		{
			//Can't knife or throw while crouching
			if ( !(aim==AIM_DOWN && m_CurrentAction==ACTION_KNIFING) && !(aim==AIM_DOWN && m_CurrentAction==ACTION_THROWING))
			{
				m_CurrentAim=aim;
				//When aim has been changed TickCount is reset to let the animations start correctly
				m_TickCount=0;
			}


		}
		
	}
}


bool Player::Shoot()
{
	
	if (m_TimeWhenLastShot>m_ShootingInterval && m_CurrentAction==ACTION_NOTHING)
	{
		m_TimeWhenLastShot=0;
		if (m_CurrentAction==ACTION_SHOOTING)
		{
			//If already shooting the animation will skip a certain amount of frames
			if (m_CurrentAim==AIM_NORMAL)
			{
				m_TickCount=3;
			}
			else if (m_CurrentAim==AIM_UP)
			{
				m_TickCount=5;
			}
			else if (m_CurrentAim==AIM_DOWN)
			{
				//Don't skip frames when crouched shooting
				m_TickCount=1;
			}


		}
		else
		{
			m_TickCount=0;
			m_CurrentAction=ACTION_SHOOTING;
		}	

		m_AudioPistolShotPtrArr[m_TickShots]->Stop();
		m_AudioPistolShotPtrArr[m_TickShots]->Play();
		m_TickShots= (m_TickShots+1)%MAX_SHOTS;
		return true;
	}

	return false;
}

bool Player::Knife(DOUBLE2 targetPos)
{
	if (m_CurrentAction!=ACTION_KNIFING && (m_Pos-targetPos).Length()<=40 && m_CurrentAction==ACTION_NOTHING)
	{
		m_CurrentAction=ACTION_KNIFING;
		//Can't crouch and knife
		m_CurrentAim=AIM_NORMAL;
		m_TickCount=0;
		return true;
	}

	return false;	
}


bool Player::Throw()
{

	if (m_TimeWhenLastThrown>m_ThrowingInterval  && m_CurrentAction==ACTION_NOTHING && m_AmountOfBombs>0)
	{
		m_TimeWhenLastThrown=0;
		m_AmountOfBombs-=1;

		m_TickCount=0;
		m_CurrentAction=ACTION_THROWING;
		//Can't crouch and throw
		m_CurrentAim=AIM_NORMAL;
		
		return true;
	}

	return false;
}

bool Player::IsDirectionLeft()
{
	if (m_CurrentDirection==DIRECTION_LEFT) return true;

	return false;
}



double Player::GetAimingAngle()
{
	return m_AimingAngle;
}

void Player::AddGravity(DOUBLE2 force)
{
	m_Velocity+=force;
}



int Player::GetLives()
{
	return m_Lives;
}

void Player::SetLives(int amountOfLives)
{
	if (amountOfLives>=0)
	{
		m_Lives=amountOfLives;
	}

}

void Player::SubtractLives(int amountOfLives)
{
	m_Lives-=amountOfLives;

	if (m_Lives<0) m_Lives=0;
}

void Player::Kill(DOUBLE2 cameraPos)
{
	if (m_CurrentAction!=ACTION_DYING && m_IsInvulnerable==false)
	{
		SubtractLives(1);	
		m_CurrentAction=ACTION_DYING;
		m_TickCount=0;

		m_CameraPos=cameraPos;

		m_AudioDeathScreamPtr->Play();
	}
}




// Private Animation methods
void Player::AnimateBodyIdle(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;
		m_TickCount+=1;

		if (m_TickCount>=BODY_IDLE_FRAMES)
		{
			m_TickCount=0;
		}
	}
	m_SlowDownAnimations+=BODY_IDLE_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameBodyIdle()
{
	// See SpriteSheet for values
	int width=BODY_IDLE_WIDTH;
	int heightStart=BODY_IDLE_TOP;
	int heightEnd=BODY_IDLE_BOTTOM;
	RECT frameBody;

	frameBody.left=width*m_TickCount;
	frameBody.right=width+width*m_TickCount-1;
	frameBody.top=heightStart;
	frameBody.bottom=heightEnd;

	m_LegsOffsetX=0;
	m_LegsOffsetY=0;

	return frameBody;
}


void Player::AnimateBodyShootingForward(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
	m_SlowDownAnimations=RESET_SLOW_DOWN;

		if (m_TickCount<BODY_SHOOTING_F_FRAMES-1)
		{
			m_TickCount+=1;
			
		}
		else
		{
			m_CurrentAction=ACTION_NOTHING;
			m_TickCount=0;
		}

	}
	m_SlowDownAnimations+=BODY_SHOOTING_F_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameBodyShootingForward()
{
	// See SpriteSheet for values
	int width=BODY_SHOOTING_F_WIDTH;
	int heightStart=BODY_SHOOTING_F_TOP;
	int heightEnd=BODY_SHOOTING_F_BOTTOM;
	RECT frameBody;

	frameBody.left=width*m_TickCount;
	frameBody.right=width+width*m_TickCount-1;
	frameBody.top=heightStart;
	frameBody.bottom=heightEnd;

	return frameBody;
}

void Player::AnimateBodyShootingUpward(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;

		if (m_TickCount<BODY_SHOOTING_U_FRAMES-1)
		{
			m_TickCount+=1;
			
		}
		else
		{
			m_CurrentAction=ACTION_NOTHING;
			m_TickCount=0;
		}

	}
	m_SlowDownAnimations+=BODY_SHOOTING_U_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameBodyShootingUpward()
{
	// See SpriteSheet for values
	int width=BODY_SHOOTING_U_WIDTH;
	int heightStart=BODY_SHOOTING_U_TOP;
	int heightEnd=BODY_SHOOTING_U_BOTTOM;
	RECT frameBody;
	
	frameBody.left=width*m_TickCount;
	frameBody.right=width+width*m_TickCount-1;
	frameBody.top=heightStart;
	frameBody.bottom=heightEnd;

	return frameBody;
}

void Player::AnimateBodyShootingDownward(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;

		if (m_TickCount<BODY_SHOOTING_D_FRAMES-1)
		{
			m_TickCount+=1;
			
		}
		else
		{
			m_CurrentAction=ACTION_NOTHING;
			m_TickCount=0;
		}

	}
	m_SlowDownAnimations+=BODY_SHOOTING_D_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameBodyShootingDownward()
{
	// See SpriteSheet for values
	int width=BODY_SHOOTING_D_WIDTH;
	int heightStart=BODY_SHOOTING_D_TOP;
	int heightEnd=BODY_SHOOTING_D_BOTTOM;
	RECT frameBody;
	
	frameBody.left=width*m_TickCount;
	frameBody.right=width+width*m_TickCount-1;
	frameBody.top=heightStart;
	frameBody.bottom=heightEnd;

	return frameBody;
}


void Player::AnimateBodyKnifing(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;

		if (m_TickCount<BODY_KNIFING_FRAMES-1)
		{
			m_TickCount+=1;
			
		}
		else
		{
			m_CurrentAction=ACTION_NOTHING;
			m_TickCount=0;
		}

	}
	m_SlowDownAnimations+=BODY_KNIFING_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameBodyKnifing()
{
	// See SpriteSheet for values
	int width=BODY_KNIFING_WIDTH;
	int heightStart=BODY_KNIFING_TOP;
	int heightEnd=BODY_KNIFING_BOTTOM;
	RECT frameBody;
	
	frameBody.left=width*m_TickCount;
	frameBody.right=width+width*m_TickCount-1;
	frameBody.top=heightStart;
	frameBody.bottom=heightEnd;

	m_BodyOffsetX=-2;
	m_BodyOffsetY=-3;

	return frameBody;
}


void Player::AnimateBodyThrowing(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;

		if (m_TickCount<BODY_THROWING_FRAMES-1)
		{
			m_TickCount+=1;
			
		}
		else
		{
			m_CurrentAction=ACTION_NOTHING;
			m_TickCount=0;
		}

	}
	m_SlowDownAnimations+=BODY_THROWING_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameBodyThrowing()
{
	// See SpriteSheet for values
	int width=BODY_THROWING_WIDTH;
	int heightStart=BODY_THROWING_TOP;
	int heightEnd=BODY_THROWING_BOTTOM;
	RECT frameBody;
	
	frameBody.left=width*m_TickCount;
	frameBody.right=width+width*m_TickCount-1;
	frameBody.top=heightStart;
	frameBody.bottom=heightEnd;

	return frameBody;
}


//Legs
RECT Player::GetFrameLegsIdle()
{
	// See SpriteSheet for values
	int width=LEGS_IDLE_WIDTH;
	int heightStart=LEGS_IDLE_TOP;
	int heightEnd=LEGS_IDLE_BOTTOM;
	RECT frameLegs;

	frameLegs.left=0;
	frameLegs.right=width-1;
	frameLegs.top=heightStart;
	frameLegs.bottom=heightEnd;
	
	m_LegsOffsetX=0;
	m_LegsOffsetY=LEGS_IDLE_OFFSET_Y;

	return frameLegs;
}


void Player::AnimateLegsRunning(double deltaTime)
{
	if (m_SlowDownAnimationsAlt>0)
	{
		m_SlowDownAnimationsAlt=RESET_SLOW_DOWN;
		m_TickCountAlt+=1;

		if (m_TickCountAlt>=LEGS_RUNNING_FRAMES)
		{
			m_TickCountAlt=0;	
		}
	
	}
	m_SlowDownAnimationsAlt+=LEGS_RUNNING_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameLegsRunning()
{
	// See SpriteSheet for values
	int width=LEGS_RUNNING_WIDTH;
	int heightStart=LEGS_RUNNING_TOP;
	int heightEnd=LEGS_RUNNING_BOTTOM;
	RECT frameLegs;


	frameLegs.left=width*m_TickCountAlt;
	frameLegs.right=width+width*m_TickCountAlt-1;
	frameLegs.top=heightStart;
	frameLegs.bottom=heightEnd;

	
	m_LegsOffsetX=LEGS_RUNNING_OFFSET_X;
	m_LegsOffsetY=LEGS_RUNNING_OFFSET_Y;

	return frameLegs;
}


void Player::AnimateLegsJumping(double deltaTime)
{
	if (m_SlowDownAnimationsAlt>0)
	{
		m_SlowDownAnimationsAlt=RESET_SLOW_DOWN;
		
		if (m_TickCountAlt<LEGS_JUMPING_FRAMES-1)
		{
			m_TickCountAlt+=1;
		}
	}
	m_SlowDownAnimationsAlt+=LEGS_RUNNING_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameLegsJumping()
{
	// See SpriteSheet for values
	int width=LEGS_JUMPING_WIDTH;
	int heightStart=LEGS_JUMPING_TOP;
	int heightEnd=LEGS_JUMPING_BOTTOM;
	RECT frameLegs;


	frameLegs.left=width*m_TickCountAlt;
	frameLegs.right=width+width*m_TickCountAlt;
	frameLegs.top=heightStart;
	frameLegs.bottom=heightEnd;

	
	m_LegsOffsetX=LEGS_JUMPING_OFFSET_X-1;
	m_LegsOffsetY=LEGS_JUMPING_OFFSET_Y;

	return frameLegs;
}

void Player::AnimateUprightToCrouch(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;

		if (m_TickCount<UP_TO_CROUCH_FRAMES-1)
		{
			m_TickCount+=1;
			
		}
		else
		{
			m_IsCrouchingAnimationDone=true;
			m_TickCount=0;
			m_TickCountAlt=0;
		}

	}
	m_SlowDownAnimations+=UP_TO_CROUCH_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameUprightToCrouch()
{
	// See SpriteSheet for values
	int width=UP_TO_CROUCH_WIDTH;
	int heightStart=UP_TO_CROUCH_TOP;
	int heightEnd=UP_TO_CROUCH_BOTTOM;

	RECT frame;
	

	frame.left=width*m_TickCount;
	frame.right=width+width*m_TickCount-1;
	frame.top=heightStart;
	frame.bottom=heightEnd;

	m_BodyOffsetX=UP_TO_CROUCH_OFFSET_X;
	m_BodyOffsetY=UP_TO_CROUCH_OFFSET_Y;

	return frame;
}


void Player::AnimateCrouchIdle(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;
		m_TickCount+=1;

		if (m_TickCount>=CROUCH_IDLE_FRAMES-1)
		{
			m_TickCount=0;
		}
	}
	m_SlowDownAnimations+=CROUCH_IDLE_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameCrouchIdle()
{
	// See SpriteSheet for values
	int width=CROUCH_IDLE_WIDTH;
	int heightStart=CROUCH_IDLE_TOP;
	int heightEnd=CROUCH_IDLE_BOTTOM;

	RECT frame;

	frame.left=width*m_TickCount;
	frame.right=width+width*m_TickCount-1;
	frame.top=heightStart;
	frame.bottom=heightEnd;

	m_BodyOffsetX=CROUCH_IDLE_OFFSET_X;
	m_BodyOffsetY=CROUCH_IDLE_OFFSET_Y;

	return frame;
}

void Player::AnimateCrouchWalking(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;
		m_TickCount+=1;

		if (m_TickCount>=CROUCH_WALKING_FRAMES-1)
		{
			m_TickCount=0;
		}
	}
	m_SlowDownAnimations+=CROUCH_WALKING_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameCrouchWalking()
{
	// See SpriteSheet for values
	int width=CROUCH_WALKING_WIDTH;
	int heightStart=CROUCH_WALKING_TOP;
	int heightEnd=CROUCH_WALKING_BOTTOM;

	RECT frame;

	frame.left=width*m_TickCount;
	frame.right=width+width*m_TickCount-1;
	frame.top=heightStart;
	frame.bottom=heightEnd;

	m_BodyOffsetX=CROUCH_WALKING_OFFSET_X;
	m_BodyOffsetY=CROUCH_WALKING_OFFSET_Y;

	return frame;
}

void Player::AnimateCrouchShooting(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;

		if (m_TickCount<CROUCH_SHOOTING_FRAMES-1)
		{
			m_TickCount+=1;
			//Instantly puts the up to crouch animation to done
			m_IsCrouchingAnimationDone=true;
		}
		else
		{
			m_CurrentAction=ACTION_NOTHING;
			m_TickCount=0;
		}

	}
	m_SlowDownAnimations+=CROUCH_SHOOTING_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameCrouchShooting()
{
	// See SpriteSheet for values
	int width=CROUCH_SHOOTING_WIDTH;
	int heightStart=CROUCH_SHOOTING_TOP;
	int heightEnd=CROUCH_SHOOTING_BOTTOM;

	RECT frame;

	frame.left=width*m_TickCount+1;
	frame.right=width+width*m_TickCount;
	frame.top=heightStart;
	frame.bottom=heightEnd;

	m_BodyOffsetX=CROUCH_SHOOTING_OFFSET_X;
	m_BodyOffsetY=CROUCH_SHOOTING_OFFSET_Y;

	return frame;
}

void Player::AnimateDying(double deltaTime)
{
	if (m_SlowDownAnimations>0)
	{
		m_SlowDownAnimations=RESET_SLOW_DOWN;

		int fadeOutTicks=8;
		if (m_TickCount<DYING_FRAMES-1+fadeOutTicks)
		{
			m_TickCount+=1;

			if (m_Lives==0 &&  m_TickCount>=DYING_FRAMES) m_TickCount=DYING_FRAMES-1;
		}
		else
		{
			m_CurrentAction=ACTION_NOTHING;
			SetVelocity(0,0);
			SetPos(m_CameraPos.x+20,120);

			m_InvulnerabilityTimer=1;
			m_IsInvulnerable=true;
		}

	}
	m_SlowDownAnimations+=DYING_ANIMATION_SPEED*deltaTime;
}

RECT Player::GetFrameDying()
{
	// See SpriteSheet for values
	int width=DYING_WIDTH;
	int heightStart=DYING_TOP;
	int heightEnd=DYING_BOTTOM;

	

	int tickCount=m_TickCount;

	//Fade out effect
	if (m_TickCount>=DYING_FRAMES)
	{
		if (m_TickCount%2==0)
		{
			tickCount=DYING_FRAMES;
		}
		else tickCount=DYING_FRAMES-1;

	}

	RECT frame;

	frame.left=width*tickCount+1;
	frame.right=width+width*tickCount;
	frame.top=heightStart;
	frame.bottom=heightEnd;

	m_BodyOffsetX=DYING_OFFSET_X;
	m_BodyOffsetY=DYING_OFFSET_Y;

	return frame;
}
