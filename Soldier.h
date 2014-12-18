#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Character.h"

//-----------------------------------------------------
// Soldier Class									
//-----------------------------------------------------
class Soldier : public Character
{
public:
	Soldier(int x, int y);				// Constructor
	virtual ~Soldier();		// Destructor

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

	static const int WIDTH=34, HEIGHT=40;
	static const int CENTER_X = WIDTH/2;
	static const int CENTER_Y = HEIGHT/2;
	
	static const int STATE_IDLE=0; 
	static const int STATE_RUNNING=1; 
	static const int STATE_RUNNINGSCARED=2;	
	static const int STATE_THROWING=3; 
	static const int STATE_KNIFING=4;
	static const int STATE_DYING=5;
	static const int STATE_DEAD=6;


private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	void RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector);	
	void RaycastSemiCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector);

	static Bitmap* m_BmpSoldierSpriteSheetPtr;

	Audio* m_AudioDeathScreamPtr;

	HitRegion *m_HitRegionPtr;
	DOUBLE2 m_Pos, m_Velocity, m_TargetPos;
	
	int m_TickCount, m_CurrentDirection, m_CurrentState;

	double m_SlowDownAnimations, m_TimeWhenLastExecutedAttack, m_ExecuteAttackingInterval, m_Health;
	bool m_HasExecutedAttack, m_HasTarget;

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


	static const int IDLE_WIDTH = 34;
	static const int IDLE_TOP = 100;
	static const int IDLE_BOTTOM = 140;
	static const int IDLE_FRAMES = 6;
	static const int IDLE_ANIMATION_SPEED = 100;

	static const int RUNNING_WIDTH = 34;
	static const int RUNNING_TOP = 0;
	static const int RUNNING_BOTTOM = 40;
	static const int RUNNING_FRAMES = 12;
	static const int RUNNING_ANIMATION_SPEED = 100;


	static const int DYING_WIDTH = 60;
	static const int DYING_TOP = 313;
	static const int DYING_BOTTOM = 357;
	static const int DYING_FRAMES = 11;
	static const int DYING_ANIMATION_SPEED = 200;
	static const int DYING_OFFSET_X=-14;
	static const int DYING_OFFSET_Y=-4;

	static const int RUNNINGSCARED_WIDTH = 40;
	static const int RUNNINGSCARED_TOP = 250;
	static const int RUNNINGSCARED_BOTTOM = 288;
	static const int RUNNINGSCARED_FRAMES = 14;
	static const int RUNNINGSCARED_ANIMATION_SPEED = 200;
	static const int RUNNINGSCARED_OFFSET_Y=2;

	static const int THROWING_WIDTH = 40;
	static const int THROWING_TOP = 145;
	static const int THROWING_BOTTOM = 191;
	static const int THROWING_FRAMES = 14;
	static const int THROWING_ANIMATION_SPEED = 150;
	static const int THROWING_OFFSET_X=-2;
	static const int THROWING_OFFSET_Y=-6;

	static const int KNIFING_WIDTH = 60;
	static const int KNIFING_TOP = 374;
	static const int KNIFING_BOTTOM = 418;
	static const int KNIFING_FRAMES = 6;
	static const int KNIFING_ANIMATION_SPEED = 150;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Soldier(const Soldier& yRef);									
	Soldier& operator=(const Soldier& yRef);	
};

 
