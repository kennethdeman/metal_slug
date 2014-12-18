#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Character.h"

//-----------------------------------------------------
// Helicopter Class									
//-----------------------------------------------------
class Helicopter : public Character
{
public:
	Helicopter(int x, int y);				// Constructor
	virtual ~Helicopter();		// Destructor

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

	static const int WIDTH=95, HEIGHT=75;
	static const int CENTER_X = WIDTH/2-5;
	static const int CENTER_Y = HEIGHT/2;

	static const int STATE_FLYING=0; 
	static const int STATE_CRASHING=1;
	static const int STATE_EXPLODING=2;
	static const int STATE_DESTROYED=3;

	static const int DIRECTION_LEFT=0; 
	static const int DIRECTION_RIGHT=1; 
	

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	void RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector);	

	static Bitmap* m_BmpHelicopterSpriteSheetPtr, *m_BmpHeliExplosionSpriteSheetPtr;

	Audio* m_AudioExplosionPtr;

	HitRegion *m_HitRegionPtr;
	DOUBLE2 m_Pos, m_Velocity, m_TargetPos;
	
	int m_TickCount, m_TickCountAlt, m_SlowDownAnimations, m_SlowDownAnimationsAlt,
		m_CurrentDirection, m_CurrentState;

	// Variables which are used to alter where the bitmaps have to be drawn without altering the hitboxes
	int m_OffsetX, m_OffsetY, m_AmountOfBombsReady;

	bool m_HasTarget,m_AboveTarget;

	double m_BombsTimer, m_Health;

	DOUBLE2 m_PointRay1, m_VectorRay1;
	DOUBLE2 m_PointRay2, m_VectorRay2;
	DOUBLE2 m_PointRay3, m_VectorRay3;
	DOUBLE2 m_PointRay4, m_VectorRay4;

	static const int POINTS=800;
	bool m_PointsGiven;

	//This constant value is used to reset the doubles that take care of slowing down the animations
	//according to deltaTime
	static const int RESET_SLOW_DOWN=-10;

	static const int FLYING_WIDTH = 95;
	static const int FLYING_TOP = 0;
	static const int FLYING_BOTTOM = 75;
	static const int FLYING_FRAMES = 11;
	static const int FLYING_ANIMATION_SPEED = 100;

	static const int CRASHING_WIDTH = 95;
	static const int CRASHING_TOP = 185;
	static const int CRASHING_BOTTOM = 254;
	static const int CRASHING_FRAMES = 7;
	static const int CRASHING_ANIMATION_SPEED = 100;

	static const int EXPLODING_WIDTH = 75;
	static const int EXPLODING_TOP = 0;
	static const int EXPLODING_BOTTOM = 179;
	static const int EXPLODING_FRAMES = 24;
	static const int EXPLODING_ANIMATION_SPEED = 200;
	static const int EXPLODING_OFFSET_Y=-80;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Helicopter(const Helicopter& yRef);									
	Helicopter& operator=(const Helicopter& yRef);	
};

 
