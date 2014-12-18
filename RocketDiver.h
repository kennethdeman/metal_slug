#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Character.h"

//-----------------------------------------------------
// RocketDiver Class									
//-----------------------------------------------------
class RocketDiver : public Character
{
public:
	RocketDiver(int x, int y);				// Constructor
	virtual ~RocketDiver();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion* hitRegionPtr);	
	void HandleSemiCollision(HitRegion* hitRegionPtr);
	void AddGravity(DOUBLE2 force);
	int GivePoints();

	void SetPos(int x, int y);
	void SetPos(DOUBLE2 pos);
	void SetXPos(int x);
	void SetYPos(int y);
	DOUBLE2 GetPos();

	void SetVelocity(int x, int y);
	void SetVelocity(DOUBLE2 velocity);
	void SetXVelocity(int x);
	void SetYVelocity(int y);
	DOUBLE2 GetVelocity();

	HitRegion* GetHitRegion();

	void SetDirection(bool isLeft);
	int GetDirection();

	void SetState(int state);	
		
	bool IsAlive();
	bool IsReadyToBeDeleted();

	bool SetTarget(DOUBLE2 playerPos);

	bool IsHostile();
	bool UsesBombs();
	int GetProjectileType();

	void SubtractHealth(double health);	

	bool ExecuteAttack();
	bool ExecuteSecondaryAttack(DOUBLE2 targetPos);

	void GetCameraBounds(DOUBLE2 pos, int width, int height);

	static const int WIDTH=40, HEIGHT=45;
	static const int CENTER_X = WIDTH/2;
	static const int CENTER_Y = HEIGHT/2;
	
	static const int STATE_HIDDEN=0; 
	static const int STATE_GOINGUP=1; 
	static const int STATE_THROWING=2; 
	static const int STATE_HIDDENAGAIN=3;
	static const int STATE_DYING=4;
	static const int STATE_DEAD=5;
	
private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	void RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector);	

	static Bitmap* m_BmpRocketDiverSpriteSheetPtr;

	Audio* m_AudioDeathScreamPtr;

	HitRegion *m_HitRegionPtr;
	DOUBLE2 m_Pos, m_Velocity, m_TargetPos, m_SplashPos;
	
	int m_TickCount, m_TickCountSplash, 
		m_CurrentDirection, m_CurrentState;

	double m_SlowDownAnimations, m_SlowDownAnimationsSplash, m_Health;
	bool m_HasExecutedAttack, m_HasTarget, m_AnimateSplash;

	// Variables which are used to alter where the bitmaps have to be drawn without altering the hitboxes
	int m_OffsetX, m_OffsetY;

	static const int DIRECTION_RIGHT=0;
	static const int DIRECTION_LEFT=1;
	
	DOUBLE2 m_PointRay1, m_VectorRay1;
	DOUBLE2 m_PointRay2, m_VectorRay2;
	DOUBLE2 m_PointRay3, m_VectorRay3;
	DOUBLE2 m_PointRay4, m_VectorRay4;

	static const int POINTS=100;
	bool m_PointsGiven;


	//This constant value is used to reset the doubles that take care of slowing down the animations
	//according to deltaTime
	static const int RESET_SLOW_DOWN=-10;

	static const int GOINGUP_FRAMES = 3;

	static const int THROWING_WIDTH = 50;
	static const int THROWING_TOP = 0;
	static const int THROWING_BOTTOM = 60;
	static const int THROWING_FRAMES = 10;
	static const int THROWING_ANIMATION_SPEED = 200;	
	static const int THROWING_OFFSET_X=-5;
	static const int THROWING_OFFSET_Y=-12;	

	static const int DYING_WIDTH = 50;
	static const int DYING_TOP = 60;
	static const int DYING_BOTTOM = 105;
	static const int DYING_FRAMES = 9;
	static const int DYING_ANIMATION_SPEED = 100;
	static const int DYING_OFFSET_X=0;
	static const int DYING_OFFSET_Y=8;

	static const int SPLASH_WIDTH = 40;
	static const int SPLASH_TOP = 105;
	static const int SPLASH_BOTTOM = 185;
	static const int SPLASH_FRAMES = 27;
	static const int SPLASH_ANIMATION_SPEED = 200;	
	static const int SPLASH_OFFSET_X=0;
	static const int SPLASH_OFFSET_Y=-75;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	RocketDiver(const RocketDiver& yRef);									
	RocketDiver& operator=(const RocketDiver& yRef);	
};

 
