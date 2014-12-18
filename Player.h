#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------
// Player Class									
//-----------------------------------------------------
class Player
{
public:
	Player(int x, int y);				// Constructor
	virtual ~Player();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion* hitRegionPtr);
	void HandleSemiCollision(HitRegion* hitRegionPtr);
	void HandleBoundaryCollision(int x, int width);
	
	void MoveLeft();
	void MoveRight();
	void StopMoving();
	void Jump();
	
	void AddAmountOfBombs(int amount);
	int GetAmountOfBombs();

	void ToggleGodmode();

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
	void SetMovement(bool isMoving);
	void SetState(bool isInTheAir);
	void SetAim(int aim);

	bool Shoot();

	bool Knife(DOUBLE2 targetPos);

	bool Throw();

	bool IsDirectionLeft();

	double GetAimingAngle();

	void AddGravity(DOUBLE2 force);

	int GetLives();
	void SetLives(int amountOfLives);
	void SubtractLives(int amountOfLives);

	void Kill(DOUBLE2 cameraPos);

	static const int WIDTH=33, HEIGHT=37;
	static const int CENTER_X = WIDTH/2-5;
	static const int CENTER_Y = HEIGHT/2;

	static const int AIM_NORMAL=0; 
	static const int AIM_UP=1;
	static const int AIM_DOWN=2;

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	void RaycastFullCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector);
	void RaycastSemiCollision(HitRegion* hitRegionPtr, DOUBLE2 point, DOUBLE2 vector);

	static Bitmap* m_BmpPlayerBodySpriteSheetPtr, *m_BmpPlayerLegsSpriteSheetPtr;

	Audio* m_AudioDeathScreamPtr;

	int m_TickShots;
	static const int MAX_SHOTS=5;
	Audio *m_AudioPistolShotPtrArr[MAX_SHOTS];

	double m_ShootingInterval, m_ThrowingInterval;

	HitRegion *m_HitRegionPtr;
	DOUBLE2 m_Pos, m_Velocity, m_CameraPos;
	// Alt = Alternative: alternative variables which can be used to animate the legs differently 
	int m_TickCount, m_TickCountAlt,
		m_CurrentDirection, m_CurrentMovement, m_CurrentState, m_CurrentAim, 
		m_CurrentAction, m_Lives, m_AmountOfBombs;

	double m_SlowDownAnimations, m_SlowDownAnimationsAlt;

	double m_TimeWhenLastShot, m_AimingAngle, m_TimeWhenLastThrown, m_InvulnerabilityTimer;

	bool m_IsInvulnerable, m_Godmode;

	DOUBLE2 m_PointRay1, m_VectorRay1;
	DOUBLE2 m_PointRay2, m_VectorRay2;
	DOUBLE2 m_PointRay3, m_VectorRay3;
	DOUBLE2 m_PointRay4, m_VectorRay4;

	// Variables which are used to alter where the bitmaps have to be drawn without altering the hitboxes
	int m_BodyOffsetX, m_BodyOffsetY, m_LegsOffsetX, m_LegsOffsetY;

	static const int DIRECTION_RIGHT=1;
	static const int DIRECTION_LEFT=0;
	static const int MOVEMENT_STAYING=0; // Horizontal movement: stays on the spot (vertical movement handled by state)
	static const int MOVEMENT_MOVING=1; // Moves horizontally
	static const int STATE_ONTHEGROUND=0; // + AIM NORMAL or UP -> Upright // + AIM DOWN -> Crouching
	static const int STATE_INTHEAIR=1; // When jumping or falling	
	static const int ACTION_NOTHING=0;
	static const int ACTION_SHOOTING=1;
	static const int ACTION_KNIFING=2;
	static const int ACTION_THROWING=3;
	static const int ACTION_DYING=4;
	
	//This constant value is used to reset the doubles that take care of slowing down the animations
	//according to deltaTime
	static const int RESET_SLOW_DOWN=-10;

	static const int BODY_IDLE_WIDTH = 33;
	static const int BODY_IDLE_TOP = 0;
	static const int BODY_IDLE_BOTTOM = 29;
	static const int BODY_IDLE_FRAMES = 4;
	static const int BODY_IDLE_ANIMATION_SPEED = 100;
	void AnimateBodyIdle(double deltaTime);
	RECT GetFrameBodyIdle();

	static const int BODY_SHOOTING_F_WIDTH = 54;
	static const int BODY_SHOOTING_F_TOP = 31;
	static const int BODY_SHOOTING_F_BOTTOM = 57;
	static const int BODY_SHOOTING_F_FRAMES = 14;
	static const int BODY_SHOOTING_F_ANIMATION_SPEED = 1500;
	void AnimateBodyShootingForward(double deltaTime);
	RECT GetFrameBodyShootingForward();

	static const int BODY_SHOOTING_U_WIDTH = 33;
	static const int BODY_SHOOTING_U_TOP = 80;
	static const int BODY_SHOOTING_U_BOTTOM = 145;
	static const int BODY_SHOOTING_U_FRAMES = 18;
	static const int BODY_SHOOTING_U_ANIMATION_SPEED = 1500;
	void AnimateBodyShootingUpward(double deltaTime);
	RECT GetFrameBodyShootingUpward(); 

	static const int BODY_SHOOTING_D_WIDTH = 25;
	static const int BODY_SHOOTING_D_TOP = 490;
	static const int BODY_SHOOTING_D_BOTTOM = 547;
	static const int BODY_SHOOTING_D_FRAMES = 7;
	static const int BODY_SHOOTING_D_ANIMATION_SPEED = 300;
	void AnimateBodyShootingDownward(double deltaTime);
	RECT GetFrameBodyShootingDownward();

	static const int BODY_KNIFING_WIDTH = 50;
	static const int BODY_KNIFING_TOP = 548;
	static const int BODY_KNIFING_BOTTOM = 585;
	static const int BODY_KNIFING_FRAMES = 6;
	static const int BODY_KNIFING_ANIMATION_SPEED = 100;
	void AnimateBodyKnifing(double deltaTime);
	RECT GetFrameBodyKnifing();

	static const int BODY_THROWING_WIDTH = 36;
	static const int BODY_THROWING_TOP = 452;
	static const int BODY_THROWING_BOTTOM = 482;
	static const int BODY_THROWING_FRAMES = 5;
	static const int BODY_THROWING_ANIMATION_SPEED = 100;
	void AnimateBodyThrowing(double deltaTime);
	RECT GetFrameBodyThrowing();

	static const int LEGS_IDLE_WIDTH = 24;
	static const int LEGS_IDLE_TOP = 0;
	static const int LEGS_IDLE_BOTTOM = 15;
	static const int LEGS_IDLE_OFFSET_Y = 22;
	//No animation
	RECT GetFrameLegsIdle();

	static const int LEGS_RUNNING_WIDTH = 32;
	static const int LEGS_RUNNING_TOP = 25;
	static const int LEGS_RUNNING_BOTTOM = 44;
	static const int LEGS_RUNNING_FRAMES = 18;
	static const int LEGS_RUNNING_OFFSET_X = -6;
	static const int LEGS_RUNNING_OFFSET_Y = 18;
	static const int LEGS_RUNNING_ANIMATION_SPEED = 150;
	void AnimateLegsRunning(double deltaTime);
	RECT GetFrameLegsRunning();

	static const int LEGS_JUMPING_WIDTH = 25;
	static const int LEGS_JUMPING_TOP = 75;
	static const int LEGS_JUMPING_BOTTOM = 100;
	static const int LEGS_JUMPING_FRAMES = 6;
	static const int LEGS_JUMPING_OFFSET_X = 2;
	static const int LEGS_JUMPING_OFFSET_Y = 19;
	static const int LEGS_JUMPING_ANIMATION_SPEED = 200;
	void AnimateLegsJumping(double deltaTime);
	RECT GetFrameLegsJumping();

	
	bool m_IsCrouchingAnimationDone;
	static const int UP_TO_CROUCH_WIDTH = 40;
	static const int UP_TO_CROUCH_TOP = 165;
	static const int UP_TO_CROUCH_BOTTOM = 205;
	static const int UP_TO_CROUCH_FRAMES = 7;
	static const int UP_TO_CROUCH_OFFSET_X = -7;
	static const int UP_TO_CROUCH_OFFSET_Y = -3;
	static const int UP_TO_CROUCH_ANIMATION_SPEED=100;
	void AnimateUprightToCrouch(double deltaTime);
	RECT GetFrameUprightToCrouch();

	static const int CROUCH_IDLE_WIDTH = 40;
	static const int CROUCH_IDLE_TOP = 219;
	static const int CROUCH_IDLE_BOTTOM = 242;
	static const int CROUCH_IDLE_FRAMES = 4;
	static const int CROUCH_IDLE_OFFSET_X = -7;
	static const int CROUCH_IDLE_OFFSET_Y = 14;
	static const int CROUCH_IDLE_ANIMATION_SPEED=100;
	void AnimateCrouchIdle(double deltaTime);
	RECT GetFrameCrouchIdle();

	static const int CROUCH_WALKING_WIDTH = 40;
	static const int CROUCH_WALKING_TOP = 258;
	static const int CROUCH_WALKING_BOTTOM = 280;
	static const int CROUCH_WALKING_FRAMES = 7;
	static const int CROUCH_WALKING_OFFSET_X = -7;
	static const int CROUCH_WALKING_OFFSET_Y = 15;
	static const int CROUCH_WALKING_ANIMATION_SPEED=100;
	void AnimateCrouchWalking(double deltaTime);
	RECT GetFrameCrouchWalking();

	static const int CROUCH_SHOOTING_WIDTH = 60;
	static const int CROUCH_SHOOTING_TOP = 330;
	static const int CROUCH_SHOOTING_BOTTOM = 358;
	static const int CROUCH_SHOOTING_FRAMES = 7;
	static const int CROUCH_SHOOTING_OFFSET_X = -2;
	static const int CROUCH_SHOOTING_OFFSET_Y = 10;
	static const int CROUCH_SHOOTING_ANIMATION_SPEED=200;
	void AnimateCrouchShooting(double deltaTime);
	RECT GetFrameCrouchShooting();

	static const int DYING_WIDTH = 50;
	static const int DYING_TOP = 585;
	static const int DYING_BOTTOM = 632;
	static const int DYING_FRAMES = 19;
	static const int DYING_OFFSET_X = -8;
	static const int DYING_OFFSET_Y = -5;
	static const int DYING_ANIMATION_SPEED=200;
	void AnimateDying(double deltaTime);
	RECT GetFrameDying();

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Player(const Player& yRef);									
	Player& operator=(const Player& yRef);	
};

 
