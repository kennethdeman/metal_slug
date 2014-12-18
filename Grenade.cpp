//---------------------------
// Includes
//---------------------------
#include "Grenade.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* Grenade::m_BmpGrenadePtr=nullptr;
Bitmap* Grenade::m_BmpExplosionPtr=nullptr;


//---------------------------
// Constructor & Destructor
//---------------------------
Grenade::Grenade(int x, int y, bool directionLeft) : m_Pos(x,y), m_Angle(0), 
	 m_IsDirectionLeft(directionLeft), m_AmountOfBounces(0), m_CurrentState(STATE_NORMAL)
	 , m_TickCount(0), m_SlowDownAnimations(-10), m_ExplosionTimer(0),
	 m_IsFromEnemy(false)
{

	if (Grenade::m_BmpGrenadePtr==nullptr)
	{
		Grenade::m_BmpGrenadePtr= new Bitmap(IDB_BULLETGRENADE);
	}

	if (Grenade::m_BmpExplosionPtr==nullptr)
	{
		Grenade::m_BmpExplosionPtr= new Bitmap(IDB_EXPLOSION);
	}

	//Set correct starting velocity
	if (m_IsDirectionLeft==true) m_Velocity=DOUBLE2(-250,-150);
	else  m_Velocity=DOUBLE2(250,-150);

	m_HitGrenadePtr=new HitRegion();
	m_HitGrenadePtr->CreateFromEllipse(0,0, CENTER_Y,CENTER_Y);

	m_HitExplosionPtr=new HitRegion();
	m_HitExplosionPtr->CreateFromRect(-CENTER_X-12, -CENTER_Y-50, CENTER_X+12, 0);
}

Grenade::Grenade(int x, int y, bool directionLeft, bool isEnemyGrenade) : m_Pos(x,y), m_Angle(0), 
	 m_IsDirectionLeft(directionLeft), m_AmountOfBounces(0), m_CurrentState(STATE_NORMAL)
	 , m_TickCount(0), m_SlowDownAnimations(-10), m_ExplosionTimer(0),
	 m_IsFromEnemy(isEnemyGrenade)
{

	if (Grenade::m_BmpGrenadePtr==nullptr)
	{
		Grenade::m_BmpGrenadePtr= new Bitmap(IDB_BULLETGRENADE);
	}

	if (Grenade::m_BmpExplosionPtr==nullptr)
	{
		Grenade::m_BmpExplosionPtr= new Bitmap(IDB_EXPLOSION);
	}

	//Set correct starting velocity
	if (m_IsDirectionLeft==true) m_Velocity=DOUBLE2(-250,-150);
	else  m_Velocity=DOUBLE2(250,-150);

	m_HitGrenadePtr=new HitRegion();
	m_HitGrenadePtr->CreateFromEllipse(0, 0, CENTER_Y,CENTER_Y);

	m_HitExplosionPtr=new HitRegion();
	m_HitExplosionPtr->CreateFromRect(-CENTER_X-12, -CENTER_Y-50, CENTER_X+12, 0);
}

Grenade::~Grenade()
{
	delete m_HitGrenadePtr;
	delete m_HitExplosionPtr;
}

//---------------------------
// Own methods
//---------------------------

// add your own methods here.

void Grenade::Tick(double deltaTime)
{
	m_HitGrenadePtr->SetPos(m_Pos);
	m_HitExplosionPtr->SetPos(m_Pos);


	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 		

		//Key input setting the speed.	
		m_Pos += m_Velocity*deltaTime;

		if (m_IsDirectionLeft==false)
			m_Angle += 5*deltaTime;
		else
			m_Angle -= 5*deltaTime;

		break;

	case STATE_EXPLODING : 

		m_Angle=0;

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

		m_Angle=0;

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

	
	if (m_AmountOfBounces>=1)
	{
		m_ExplosionTimer+=1*deltaTime;
	}

	//If the timer becomes 1 the grenade explodes
	if (m_ExplosionTimer >= 1 && m_CurrentState==STATE_NORMAL)
	{		
		m_CurrentState=STATE_EXPLODING;
	}		

	//After 2 bounces the grenade explodes
	if (m_AmountOfBounces>=2 && m_CurrentState==STATE_NORMAL) m_CurrentState=STATE_EXPLODING;
}

void Grenade::Paint(MATRIX3X2 matTransform)
{
	RECT frame;

	int width=0;
	int heightStart=0;
	int heightEnd=0;

	MATRIX3X2 matCenter,matRotate,matScale,matTranslate;

	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 		

		frame.top= BULLET_FRAME_Y;
		frame.bottom= BULLET_FRAME_Y+BULLET_HEIGHT;
		frame.left= BULLET_FRAME_X;
		frame.right= BULLET_FRAME_X+BULLET_WIDTH;		

		matCenter.SetAsTranslate(-CENTER_X,-CENTER_Y);

		matRotate.SetAsRotate(m_Angle);

		matScale.SetAsScale(1,1);
		
		matTranslate.SetAsTranslate(m_Pos);

		GAME_ENGINE->SetTransformMatrix(matCenter*matRotate*matScale*matTranslate*matTransform);

		GAME_ENGINE->DrawBitmap(m_BmpGrenadePtr,0,0,frame);

		break;

	case STATE_EXPLODING : 

		width=35;
		heightStart=0;
		heightEnd=64;

		frame.left=width*m_TickCount;
		frame.right=width+width*m_TickCount;		
		frame.top=heightStart;
		frame.bottom=heightStart+heightEnd;

		matCenter.SetAsTranslate(-17,-heightEnd);

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

	//GAME_ENGINE->FillHitRegion(m_HitGrenadePtr);
	//GAME_ENGINE->FillHitRegion(m_HitExplosionPtr);

	//DOUBLE2 extraRaycastPoint= m_Pos;
	//extraRaycastPoint.y -=CENTER_Y;
	//DOUBLE2 extraRaycastVector = DOUBLE2(0,CENTER_Y*2-5);

	//GAME_ENGINE->SetColor(COLOR(255,0,0));
	//GAME_ENGINE->DrawLine(extraRaycastPoint,extraRaycastPoint+extraRaycastVector);


	//GAME_ENGINE->SetColor(COLOR(0,0,0));
	//GAME_ENGINE->DrawString(String("") + m_ExplosionTimer, extraRaycastPoint);


}

void Grenade::HandleCollision(HitRegion* hitRegionPtr, double deltaTime)
{
	HIT hitArr[1];
	DOUBLE2 projected, reflected, newPoint, remainingVector;

	double remainingTime=deltaTime;

	newPoint=m_Pos;
	newPoint.y+=CENTER_Y;	

	remainingVector=m_Velocity*deltaTime;



	while (remainingTime>0)
	{
		if (hitRegionPtr->Raycast(newPoint, remainingVector, hitArr, 1, -1)>=1)
		{	
			++m_AmountOfBounces;	
			
			DOUBLE2 normal = DOUBLE2(hitArr[0].normal);

			projected=normal*normal.DotProduct(remainingVector);  
			reflected=remainingVector-2*projected;
			reflected=reflected.Normalized();

			reflected *= remainingVector.Length()* (1 - hitArr[0].lambda);
		

			newPoint=hitArr[0].point;
			remainingVector=reflected;

			remainingTime= deltaTime * (1 - hitArr[0].lambda);

			m_Pos=hitArr[0].point+reflected;			

			m_Velocity=reflected.Normalized()*m_Velocity.Length()/2;
			

		}
		else
		{
			remainingTime=0;
		}
	}

	//Extra raycast if the reflection raycast fails (which can happen on the second bounce)

	DOUBLE2 extraRaycastPoint= m_Pos;
	extraRaycastPoint.y -=CENTER_Y;
	DOUBLE2 extraRaycastVector = DOUBLE2(0,CENTER_Y*2-10);
	if (hitRegionPtr->Raycast(extraRaycastPoint, extraRaycastVector, hitArr, 1, -1)>=1)
	{		
		++m_AmountOfBounces;
	}
}

//Used for gravity
void Grenade::AddGravity(DOUBLE2 force)
{
	m_Velocity+=force;
}

bool Grenade::IsDestroyed()
{
	return m_CurrentState==STATE_DESTROYED;
}

DOUBLE2 Grenade::GetPos()
{
	return m_Pos;
}


bool Grenade::CheckHit(HitRegion* hitTargetPtr)
{
	

	//Only if the grenade is in the beginning stage of exploding (tickcount->0)
	if (m_CurrentState==STATE_EXPLODING && m_TickCount==0)
	{
		RECT2 collision = m_HitExplosionPtr->CollisionTest(hitTargetPtr);

		// if Collision values != 0,0,0,0
		if ( (collision.bottom - collision.top) > 0.1 || (collision.right - collision.left) > 0.1)
		{
			return true;
		}
	}
	else
	{
		RECT2 collision = m_HitGrenadePtr->CollisionTest(hitTargetPtr);

		// if Collision values != 0,0,0,0
		if ( (collision.bottom - collision.top) > 0.1 || (collision.right - collision.left) > 0.1)
		{

			//Set the position of the explosion on the target
			RECT2 playerBounds=hitTargetPtr->GetBounds();
			m_Pos.x=playerBounds.right - (playerBounds.right - playerBounds.left)/2;

			m_CurrentState=STATE_EXPLODING2;
			return true;
		}
	}


	return false;
}


bool Grenade::IsFromEnemy()
{
	return m_IsFromEnemy;
}