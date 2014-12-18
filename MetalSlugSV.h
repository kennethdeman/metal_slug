//-----------------------------------------------------------------
// Game File
// C++ Source - MetalSlugSV.h - version v2_12 jan 2013 
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Student data
// Name:
// Group:
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "Resource.h"	
#include "GameEngine.h"
#include "AbstractGame.h"

#include "Player.h"
#include "Soldier.h"
#include "Prisoner.h"
#include "Helicopter.h"
#include "RocketDiver.h"

#include "ProjectileList.h"

#include "PowerUpList.h"

//-----------------------------------------------------------------
// MetalSlugSV Class																
//-----------------------------------------------------------------
class MetalSlugSV : public AbstractGame, public Callable
{
public:				
	//---------------------------
	// Constructor(s)
	//---------------------------
	MetalSlugSV();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~MetalSlugSV();

	//---------------------------
	// General Methods
	//---------------------------

	void GameInitialize(HINSTANCE hInstance);
	void GameStart();				
	void GameEnd();
	void CheckKeyboard();
	void KeyPressed(TCHAR cKey);
	void GameTick(double deltaTime);
	void GamePaint(RECT rect);

	// -------------------------
	// Member functions
	// -------------------------
	
		

private:
	// -------------------------
	// Member functions
	// -------------------------	
	void DrawHudNumbers(int x, int y, int number, bool largeFont, Bitmap *bitmapPtr);

	// -------------------------
	// Datamembers
	// -------------------------
	static const int MAX_CHARACTERS=50;

	Bitmap *m_BmpLevelPtr, *m_BmpSubmarinePtr, *m_BmpMainHudPtr, *m_BmpLargeHudNumbersPtr, 
		*m_BmpScoreNumbersPtr, *m_BmpLiveNumbersPtr, *m_BmpMissionCompletePtr;
	HitRegion *m_HitLevelPtr, *m_HitSemiCollisionPtr;
	DOUBLE2 m_Gravity;
	DOUBLE2 m_CameraPos;
	double m_CameraScale;
	double m_Time;
	int m_Score, m_Stage;

	//First part of the level
	static const int STAGE_ONE=0;

	//Helicopter fight
	static const int STAGE_TWO=1;
	static const int STATE_TWO_START=1550;

	//Second part of the level
	static const int STAGE_THREE=2;

	//Helicopter fight
	static const int STAGE_FOUR=3;
	static const int STAGE_FOUR_START=2820;

	//Last part
	static const int STAGE_FIVE=4;

	static const int FINISH=5;

	static const int HELICOPTER1=48;
	static const int HELICOPTER2=49;

	Player* m_PlayerPtr;

	Character* m_CharacterPtrArr[MAX_CHARACTERS];

	ProjectileList* m_ProjectilesListPtr;

	PowerUpList* m_PowerUpListPtr;

	Audio* m_AudioSoundtrackPtr, *m_AudioEndingPtr;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	MetalSlugSV(const MetalSlugSV& tRef);
	MetalSlugSV& operator=(const MetalSlugSV& tRef);
};
