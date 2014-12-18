#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Character.h"

//-----------------------------------------------------
// Prisoner Class									
//-----------------------------------------------------
class Prisoner : public Character
{
public:
	Prisoner(int x, int y);				// Constructor
	virtual ~Prisoner();		// Destructor

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

	bool SetTarget(DOUBLE2 targetPos);

	bool IsHostile();
	bool UsesBombs();
	int GetProjectileType();
	
	void SubtractHealth(double health);
	
	bool ExecuteAttack();
	bool ExecuteSecondaryAttack(DOUBLE2 targetPos);

	void GetCameraBounds(DOUBLE2 pos, int width, int height);

	static const int WIDTH=48, HEIGHT=40;
	static const int CENTER_X = WIDTH/2;
	static const int CENTER_Y = HEIGHT/2;

	static const int STATE_BOUND=0; 
	static const int STATE_WAVING=1; 
	static const int STATE_ESCAPING=2;
	static const int STATE_WALKING=3; 
	static const int STATE_REWARDING=4; 
	static const int STATE_RUNNING=5;	
	static const int STATE_OUTOFCAMERA=6;

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	void RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector);	
	void RaycastSemiCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector);

	static Bitmap* m_BmpPrisonerSpriteSheetPtr;
	HitRegion *m_HitRegionPtr;
	DOUBLE2 m_Pos, m_Velocity;
	
	int m_TickCount, m_CurrentDirection, m_CurrentState;

	double m_SlowDownAnimations, m_Health;
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

	static const int BOUND_WIDTH = 48;
	static const int BOUND_TOP = 37;
	static const int BOUND_BOTTOM = 85;
	static const int BOUND_FRAMES = 9;
	static const int BOUND_ANIMATION_SPEED = 50;
	static const int BOUND_OFFSET_Y=-8;

	static const int WAVING_WIDTH = 48;
	static const int WAVING_TOP = 88;
	static const int WAVING_BOTTOM = 136;
	static const int WAVING_FRAMES = 7;
	static const int WAVING_ANIMATION_SPEED = 50;
	static const int WAVING_OFFSET_Y=-8;

	static const int ESCAPING_WIDTH = 48;
	static const int ESCAPING_TOP = 88;
	static const int ESCAPING_BOTTOM = 136;
	static const int ESCAPING_FRAMES = 10;
	static const int ESCAPING_ANIMATION_SPEED = 100;
	static const int ESCAPING_OFFSET_Y=-8;

	static const int WALKING_WIDTH = 36;
	static const int WALKING_TOP = 145;
	static const int WALKING_BOTTOM = 184;
	static const int WALKING_FRAMES = 12;
	static const int WALKING_ANIMATION_SPEED = 100;
	static const int WALKING_OFFSET_Y=1;

	static const int REWARDING_WIDTH = 50;
	static const int REWARDING_TOP = 192;
	static const int REWARDING_BOTTOM = 230;
	static const int REWARDING_FRAMES = 11;
	static const int REWARDING_ANIMATION_SPEED = 100;
	static const int REWARDING_OFFSET_Y=2;

	static const int RUNNING_WIDTH = 50;
	static const int RUNNING_TOP = 232;
	static const int RUNNING_BOTTOM = 274;
	static const int RUNNING_FRAMES = 8;
	static const int RUNNING_ANIMATION_SPEED = 100;
	static const int RUNNING_OFFSET_Y=-1;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Prisoner(const Prisoner& yRef);									
	Prisoner& operator=(const Prisoner& yRef);	
};

 
