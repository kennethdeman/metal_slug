//-----------------------------------------------------------------
// Game File
// C++ Source - MetalSlugSV.cpp - version v2_12 jan 2013 
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "MetalSlugSV.h"																				

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//-----------------------------------------------------------------
// MetalSlugSV methods																				
//-----------------------------------------------------------------

MetalSlugSV::MetalSlugSV(): m_BmpLevelPtr(nullptr)	
	,m_HitLevelPtr(nullptr), m_BmpSubmarinePtr(nullptr), m_Gravity(0,0), 
	m_PlayerPtr(nullptr), m_BmpMainHudPtr(nullptr), m_BmpLargeHudNumbersPtr(nullptr), m_BmpScoreNumbersPtr(nullptr),
	 m_Time(60), m_Score(0), m_PowerUpListPtr(nullptr),m_ProjectilesListPtr(nullptr),
	  m_HitSemiCollisionPtr(nullptr), m_Stage(STAGE_ONE), m_BmpMissionCompletePtr(nullptr),

	 m_AudioSoundtrackPtr(nullptr),m_AudioEndingPtr(nullptr), m_CameraPos(0,0), m_CameraScale(1.5)


{
	
	for (int count=0; count<MAX_CHARACTERS; ++count)
	{
		m_CharacterPtrArr[count]=nullptr;
	}

}

MetalSlugSV::~MetalSlugSV()																						
{
	// nothing to destroy
}

void MetalSlugSV::GameInitialize(HINSTANCE hInstance)			
{
	// Set the required values
	AbstractGame::GameInitialize(hInstance);
	GAME_ENGINE->SetTitle("MetalSlugSV - De Man Kenneth - 1DAE2");					
	GAME_ENGINE->RunGameLoop(true);
	
	// Set the optional values
	//GAME_ENGINE->SetWidth(1800);
	GAME_ENGINE->SetWidth(560);
	GAME_ENGINE->SetHeight(336);
	//GAME_ENGINE->SetHeight(224);
 	GAME_ENGINE->SetKeyList(String("WASDZQFL") + (TCHAR) VK_SPACE + (TCHAR) VK_SHIFT + (TCHAR) VK_CONTROL  + (TCHAR) VK_HOME);
}

void MetalSlugSV::GameStart()
{
	m_BmpLevelPtr = new Bitmap(IDB_BACKGROUND);
	m_BmpSubmarinePtr = new Bitmap(IDB_SUBMARINE);
	m_BmpMainHudPtr = new Bitmap(IDB_MAINHUD);
	m_BmpMainHudPtr->SetTransparencyColor(255,0,255);
	m_BmpLargeHudNumbersPtr = new Bitmap(IDB_LARGEHUDNUMBERS); 
	m_BmpScoreNumbersPtr = new Bitmap(IDB_SMALLHUDNUMBERS);
	m_BmpLiveNumbersPtr = new Bitmap(IDB_LIVENUMBERS);
	m_BmpMissionCompletePtr = new Bitmap(IDB_MISSIONCOMPLETE);

	m_HitLevelPtr = new HitRegion();
	m_HitLevelPtr->CreateFromSVG("./resources/FullCollision.svg");

	m_HitSemiCollisionPtr = new HitRegion();
	m_HitSemiCollisionPtr->CreateFromSVG("./resources/SemiCollision.svg");
	
	m_PlayerPtr= new Player(0,135);

	//Batch 1 
	m_CharacterPtrArr[0]=new Soldier(300,135);
	m_CharacterPtrArr[1]=new Soldier(525,135);
	m_CharacterPtrArr[2]=new Soldier(575,135);
	m_CharacterPtrArr[3]=new Soldier(625,135);
										
	m_CharacterPtrArr[4]=new Soldier(750,160);
	m_CharacterPtrArr[5]=new Soldier(900,160);
	m_CharacterPtrArr[6]=new Soldier(1050,160);

	m_CharacterPtrArr[7]=new Soldier(1300,160);
	m_CharacterPtrArr[8]=new Soldier(1350,100);
	m_CharacterPtrArr[9]=new Soldier(1400,100);
	m_CharacterPtrArr[10]=new Soldier(1450,160);
	m_CharacterPtrArr[11]=new Soldier(1500,100);
	m_CharacterPtrArr[12]=new Soldier(1550,160);
	
	//Batch 2
	m_CharacterPtrArr[13]=new Soldier(2200,160);

	m_CharacterPtrArr[14]=new Soldier(2100,88);
	m_CharacterPtrArr[15]=new Soldier(2275,88);

	m_CharacterPtrArr[16]=new Soldier(2300,160);
	m_CharacterPtrArr[17]=new Soldier(2350,160);

	m_CharacterPtrArr[18]=new Soldier(2400,42);
	m_CharacterPtrArr[19]=new Soldier(2450,42);

	m_CharacterPtrArr[20]=new Soldier(2600,160);

	m_CharacterPtrArr[21]=new Soldier(2700,42);

	m_CharacterPtrArr[22]=new Soldier(2900,86);
	m_CharacterPtrArr[23]=new Soldier(2920,86);



	m_CharacterPtrArr[24]=new Prisoner(1000,80);	
	m_CharacterPtrArr[25]=new Prisoner(1120,118);	
	m_CharacterPtrArr[26]=new Prisoner(1530,118);	

	m_CharacterPtrArr[27]=new Prisoner(3160,56);	

	m_CharacterPtrArr[29]=new RocketDiver(2060,220);
	m_CharacterPtrArr[30]=new RocketDiver(2240,220);
	m_CharacterPtrArr[31]=new RocketDiver(2400,220);
	//m_CharacterPtrArr[32]=new RocketDiver(350,220);

	m_CharacterPtrArr[HELICOPTER1]=new Helicopter(1900,50);
	m_CharacterPtrArr[HELICOPTER2]=new Helicopter(3200,50);

	m_ProjectilesListPtr = new ProjectileList();

	m_PowerUpListPtr = new PowerUpList();

	PowerUp* tempPtr;

	tempPtr= new PowerUpBombs(1450,90);

	m_PowerUpListPtr->Add(tempPtr);

	tempPtr= new PowerUpBombs(2420,50);

	m_PowerUpListPtr->Add(tempPtr);

	tempPtr= new PowerUpPig(575,145);

	m_PowerUpListPtr->Add(tempPtr);

	tempPtr= new PowerUpLetter(2000,180);

	m_PowerUpListPtr->Add(tempPtr);

	//Sounds
	m_AudioSoundtrackPtr = new Audio("./resources/Soundtrack.mp3");
	m_AudioSoundtrackPtr->SetRepeat(true);
	m_AudioSoundtrackPtr->Play();
	
	m_AudioEndingPtr = new Audio("./resources/MissionComplete.mp3");
	m_AudioSoundtrackPtr->SetRepeat(false);
}

void MetalSlugSV::GameEnd()
{
	delete m_BmpLevelPtr;
	m_BmpLevelPtr = nullptr;

	delete m_BmpSubmarinePtr;
	m_BmpSubmarinePtr = nullptr;

	delete m_BmpMainHudPtr;
	m_BmpMainHudPtr=nullptr;

	delete m_HitLevelPtr;
	m_HitLevelPtr=nullptr;	

	delete m_BmpLargeHudNumbersPtr;
	m_BmpLargeHudNumbersPtr=nullptr;

	delete m_BmpScoreNumbersPtr; 	
	m_BmpScoreNumbersPtr=nullptr;	

	delete m_BmpLiveNumbersPtr;
	m_BmpLiveNumbersPtr=nullptr;	

	delete m_BmpMissionCompletePtr;
	m_BmpMissionCompletePtr=nullptr;	


	delete m_PlayerPtr;
	m_PlayerPtr=nullptr;

	for (int count=0; count<MAX_CHARACTERS; ++count)
	{
		if (m_CharacterPtrArr[count]!=nullptr)
		{
			delete m_CharacterPtrArr[count];
			m_CharacterPtrArr[count]=nullptr;
		}		
	}

	delete m_ProjectilesListPtr;

	delete m_PowerUpListPtr;

	//Sounds
	delete m_AudioSoundtrackPtr;

	delete m_AudioEndingPtr;
}

void MetalSlugSV::CheckKeyboard()
{	
	if (m_Stage!=FINISH)
	{
		
		if (GAME_ENGINE->IsKeyDown('Z') || GAME_ENGINE->IsKeyDown('W') || GAME_ENGINE->IsKeyDown(VK_UP))
		{		
			m_PlayerPtr->SetAim(Player::AIM_UP);

		}else if (GAME_ENGINE->IsKeyDown('S') || GAME_ENGINE->IsKeyDown(VK_DOWN))
		{
			
			m_PlayerPtr->SetAim(Player::AIM_DOWN);

		}
		else
		{
			m_PlayerPtr->SetAim(Player::AIM_NORMAL);
		}

		
		if (GAME_ENGINE->IsKeyDown('Q') || GAME_ENGINE->IsKeyDown('A') || GAME_ENGINE->IsKeyDown(VK_LEFT))
		{
			m_PlayerPtr->MoveLeft();

		}
		else if (GAME_ENGINE->IsKeyDown('D') || GAME_ENGINE->IsKeyDown(VK_RIGHT))
		{
			m_PlayerPtr->MoveRight();

		}	
		else
		{
			m_PlayerPtr->StopMoving();

		}
		
		if (GAME_ENGINE->IsKeyDown(VK_SPACE) && m_PlayerPtr->GetVelocity().y==0)
		{
			m_PlayerPtr->Jump();
		}

		if (GAME_ENGINE->IsKeyDown(VK_SHIFT))
		{

			//Handles Knifing

			for (int count=0; count<MAX_CHARACTERS; ++count)
			{
				if (m_CharacterPtrArr[count]!=nullptr && m_CharacterPtrArr[count]->IsAlive()==true)
				{
					
					//Vehicles like helicopters drop bombs -> Can't knife vehicles
					if (m_CharacterPtrArr[count]->UsesBombs()==false)
					{
						if (m_PlayerPtr->Knife(m_CharacterPtrArr[count]->GetPos())==true)
						{
							//Subtracting health from prisoners frees them
							m_CharacterPtrArr[count]->SubtractHealth(1);
						}
					}				

				}		
			}
			

			//Makes the player shoot if he isn't knifing (if he is knifing Shoot() will return false instantly)
			
			if (m_PlayerPtr->Shoot()==true)
			{
				Projectile* tempPtr;
				tempPtr=new Bullet(m_PlayerPtr->GetPos().x, m_PlayerPtr->GetPos().y, 
					m_PlayerPtr->GetAimingAngle());
				m_ProjectilesListPtr->Add(tempPtr);

			}	
			

		}

		if (GAME_ENGINE->IsKeyDown(VK_CONTROL))
		{

			//Makes the player throw
			if (m_PlayerPtr->Throw()==true)
			{
				Projectile* tempPtr;
				tempPtr=new Grenade(m_PlayerPtr->GetPos().x, m_PlayerPtr->GetPos().y, 
						m_PlayerPtr->IsDirectionLeft());
				m_ProjectilesListPtr->Add(tempPtr);

			}	

		}

	}
}

void MetalSlugSV::KeyPressed(TCHAR cKey)
{		
	switch (cKey)
	{
	case VK_HOME:

		m_PlayerPtr->ToggleGodmode();

		break;
		
	}

}

void MetalSlugSV::GameTick(double deltaTime)
{

	m_AudioSoundtrackPtr->Tick();
	m_AudioEndingPtr->Tick();

	//Time speed
	deltaTime*=1;

	//Timer
	m_Time-=0.33*deltaTime;

	if (m_Time<=0)
	{
		m_PlayerPtr->Kill(m_CameraPos);
		m_Time=60;
	}



	//Player
	
	m_PlayerPtr->Tick(deltaTime);
	m_PlayerPtr->AddGravity(m_Gravity*deltaTime);
	m_PlayerPtr->HandleCollision(m_HitLevelPtr);
	m_PlayerPtr->HandleSemiCollision(m_HitSemiCollisionPtr);
	m_PlayerPtr->HandleBoundaryCollision(m_CameraPos.x,m_BmpLevelPtr->GetWidth());
	m_PlayerPtr->AddAmountOfBombs(m_PowerUpListPtr->AmmunitionGainedByPowerUps(m_PlayerPtr->GetHitRegion()));

	if (m_ProjectilesListPtr->CheckHit(m_PlayerPtr->GetHitRegion(), false)==true)
	{
		m_Time=60;
		m_PlayerPtr->Kill(m_CameraPos);	
	}

	//Characters
	for (int count=0; count<MAX_CHARACTERS; ++count)
	{
		if (m_CharacterPtrArr[count]!=nullptr)
		{
			m_CharacterPtrArr[count]->Tick(deltaTime);
			m_CharacterPtrArr[count]->AddGravity(m_Gravity*deltaTime);
			m_CharacterPtrArr[count]->HandleCollision(m_HitLevelPtr);
			m_CharacterPtrArr[count]->HandleSemiCollision(m_HitSemiCollisionPtr);

			if (m_CharacterPtrArr[count]->IsAlive()==true)
			{
				if (m_ProjectilesListPtr->CheckHit(m_CharacterPtrArr[count]->GetHitRegion(), true)==true)
				{
					m_CharacterPtrArr[count]->SubtractHealth(1);	
					m_Score+=m_CharacterPtrArr[count]->GivePoints();
				}
			}


			// If the characters are soldiers/helicopters and they are alive + if they have a target + if the player is alive
			if (m_CharacterPtrArr[count]->IsHostile()==true && m_CharacterPtrArr[count]->IsAlive()==true
				&& m_CharacterPtrArr[count]->SetTarget(m_PlayerPtr->GetPos())==true &&
			m_PlayerPtr->GetLives()>0)
			{
				//If they can attack
				if (m_CharacterPtrArr[count]->ExecuteAttack()==true)
				{

					if (m_CharacterPtrArr[count]->GetProjectileType()==Projectile::GRENADE )
					{
						//Soldiers											

						Grenade* tempPtr;
						tempPtr=new Grenade(m_CharacterPtrArr[count]->GetPos().x, 
									m_CharacterPtrArr[count]->GetPos().y, 
									m_CharacterPtrArr[count]->GetDirection(), true);
						m_ProjectilesListPtr->Add(tempPtr);

					}
					else if (m_CharacterPtrArr[count]->GetProjectileType()==Projectile::BOMB )
					{
						//Helicopters

						Projectile* tempPtr;
						tempPtr=new Bomb(m_CharacterPtrArr[count]->GetPos().x, 
								m_CharacterPtrArr[count]->GetPos().y+30, true);
						m_ProjectilesListPtr->Add(tempPtr);														
						
					}
					else if (m_CharacterPtrArr[count]->GetProjectileType()==Projectile::ROCKET )
					{
						//RocketDivers

						Projectile* tempPtr;
						tempPtr=new Rocket(m_CharacterPtrArr[count]->GetPos().x, 
								m_CharacterPtrArr[count]->GetPos().y, true);
						m_ProjectilesListPtr->Add(tempPtr);														
						
					}

				}

				//Knifing of soldiers
				if (m_CharacterPtrArr[count]->ExecuteSecondaryAttack(m_PlayerPtr->GetPos())==true)
				{
					m_PlayerPtr->Kill(m_CameraPos);
				}

			}


			if (m_CharacterPtrArr[count]->IsReadyToBeDeleted()==true)
			{
				delete m_CharacterPtrArr[count];
				m_CharacterPtrArr[count]=nullptr;
			}


		}		
	}
	
	//Projectiles
	m_ProjectilesListPtr->Tick(deltaTime);
	m_ProjectilesListPtr->CheckOutOfBounds(m_CameraPos,GAME_ENGINE->GetWidth(),GAME_ENGINE->GetHeight());
	m_ProjectilesListPtr->AddGravity(m_Gravity*deltaTime);
	m_ProjectilesListPtr->HandleCollision(m_HitLevelPtr,deltaTime);
	

	//PowerUps
	m_PowerUpListPtr->Tick(deltaTime);
	m_Score+=m_PowerUpListPtr->ScoreGainedByPowerUps(m_PlayerPtr->GetHitRegion());

	//Stages

	//Stage 1
	if (m_PlayerPtr->GetPos().x>STATE_TWO_START && m_Stage==STAGE_ONE) 
	{
		m_Stage=STAGE_TWO;
	}
	
	//Stage 2
	if (m_Stage==STAGE_TWO) 
	{
		m_PlayerPtr->HandleBoundaryCollision(m_CameraPos.x,1840);
	}
	
	if (m_CharacterPtrArr[HELICOPTER1]==nullptr) m_Stage=STAGE_THREE;

	//Stage 3
	if (m_PlayerPtr->GetPos().x>STAGE_FOUR_START && m_Stage==STAGE_THREE) 
	{
		m_Stage=STAGE_FOUR;
	}

	//Stage 4
	if (m_Stage==STAGE_FOUR) 
	{
		m_PlayerPtr->HandleBoundaryCollision(m_CameraPos.x,m_CameraPos.x+GAME_ENGINE->GetWidth()/m_CameraScale);
	}

	//Stage 5
	if (m_CharacterPtrArr[HELICOPTER2]==nullptr) m_Stage=STAGE_FIVE;

	//Finish
	if (m_PlayerPtr->GetPos().x>m_BmpLevelPtr->GetWidth()-100 && m_Stage!=FINISH)
	{
		m_Stage=FINISH;
		m_PlayerPtr->SetXVelocity(0);
		m_PlayerPtr->StopMoving();

		m_AudioSoundtrackPtr->Stop();
		m_AudioEndingPtr->Play();
	}



	//Set gravity after first tick due to Bug where characters fall through hitregion at the start of the game
	//This seems to fix it
	if (m_Gravity==DOUBLE2(0,0))
		m_Gravity=DOUBLE2(0,981/2);
}

void MetalSlugSV::GamePaint(RECT rect)
{
	MATRIX3X2 matCamera, matScale, matCameraTransform;
	matScale.SetAsScale(m_CameraScale,m_CameraScale);

	int h_sw = GAME_ENGINE->GetWidth()/2-120*m_CameraScale;
	int bitmap_w = m_BmpLevelPtr->GetWidth();
	
	DOUBLE2 oldCameraPos=m_CameraPos;

	if ( m_PlayerPtr->GetPos().x < h_sw)
	{
		m_CameraPos=DOUBLE2(0,0);
	}
	else
	{	
		m_CameraPos=DOUBLE2(m_PlayerPtr->GetPos().x  - h_sw,0);
	}     

	if (m_CameraPos.x+336> bitmap_w-36) 
	{
		m_CameraPos.x=oldCameraPos.x;
	}
	

	//Prevent the camera from going back
	if (oldCameraPos.x > m_CameraPos.x) m_CameraPos.x=oldCameraPos.x;

	//Fixed camera in stage 2
	if (m_Stage==STAGE_TWO) m_CameraPos.x=STATE_TWO_START - h_sw;

	//Fixed camera in stage 4
	if (m_Stage==STAGE_FOUR) m_CameraPos.x=STAGE_FOUR_START - h_sw;

	matCamera.SetAsTranslate(m_CameraPos);

	//matCamera.SetAsTranslate(m_PlayerPtr->GetPos().x - GAME_ENGINE->GetWidth()/3,0);
	matCameraTransform=matCamera*matScale;
	MATRIX3X2 matView=matCamera.Inverse();

	//Paint Level
	GAME_ENGINE->SetTransformMatrix(matView*matScale);
	GAME_ENGINE->DrawBitmap(m_BmpLevelPtr,0,0);

	GAME_ENGINE->SetColor(COLOR(255,0,0,127));
	//GAME_ENGINE->FillHitRegion(m_HitLevelPtr);

	//GAME_ENGINE->FillHitRegion(m_HitSemiCollisionPtr);
	
	//Paint Player
	m_PlayerPtr->Paint(matView*matScale);

	//Paint Characters
	for (int count=0; count<MAX_CHARACTERS; ++count)
	{
		if (m_CharacterPtrArr[count]!=nullptr)
		{
			m_CharacterPtrArr[count]->Paint(matView*matScale);
		}		
	}

	//Paint Projectiles
	m_ProjectilesListPtr->Paint(matView*matScale);

	//Paint PowerUps

	m_PowerUpListPtr->Paint(matView*matScale);

	//Paint Submarine

	MATRIX3X2 matSubmarine;

	matSubmarine.SetAsTranslate(1352,118);		

	GAME_ENGINE->SetTransformMatrix(matSubmarine*matView*matScale);
	GAME_ENGINE->DrawBitmap(m_BmpSubmarinePtr,0,0);

	

	if (m_Stage==FINISH)
	{		
		GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());	
		GAME_ENGINE->DrawBitmap(m_BmpMissionCompletePtr,0,0);
	}


	//Paint Hud

	MATRIX3X2 matLivesNumberScale;

	matLivesNumberScale.SetAsScale(0.7,0.7);

	GAME_ENGINE->SetTransformMatrix(matLivesNumberScale);
	

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());	

	GAME_ENGINE->DrawBitmap(m_BmpMainHudPtr,0,0);

	GAME_ENGINE->SetTransformMatrix(matScale);

	//Time	
	DrawHudNumbers(195,0,m_Time,true, m_BmpLargeHudNumbersPtr);

	//Score
	DrawHudNumbers(10,8,m_Score,false, m_BmpScoreNumbersPtr);

	//Lives
	DrawHudNumbers(62,27,m_PlayerPtr->GetLives(),false,m_BmpLiveNumbersPtr);

	//Amount Of Bombs
	DrawHudNumbers(162,18,m_PlayerPtr->GetAmountOfBombs(),false,m_BmpLiveNumbersPtr);
	
}

void MetalSlugSV::DrawHudNumbers(int x, int y, int number, bool largeFont, Bitmap *bitmapPtr)
{
	String numberString;
	numberString= String("") + number;

	RECT frame;

	frame.top=0;
	if (largeFont==true) frame.bottom = m_BmpLargeHudNumbersPtr->GetHeight();
	else frame.bottom = m_BmpScoreNumbersPtr->GetHeight();

	for (int counter=0; counter<numberString.GetLength(); ++counter)
	{
		int numberSymbol=numberString.SubString(counter,1).ToInteger();

		if (largeFont==true) 
		{
		
			frame.left = 32 * (numberSymbol-1);
			frame.right = 32 * numberSymbol;

			if (numberSymbol == 0)
			{
				frame.left = 32 * 9;
				frame.right = 32 * 10;
			}


			GAME_ENGINE->DrawBitmap(m_BmpLargeHudNumbersPtr, x + 32 * counter, y,frame);
		}			
		else 
		{
			frame.left = bitmapPtr->GetWidth()/10 * (numberSymbol-1);
			frame.right = bitmapPtr->GetWidth()/10 * numberSymbol;

			if (numberSymbol == 0)
			{
				frame.left = bitmapPtr->GetWidth()/10 * 9;
				frame.right = bitmapPtr->GetWidth()/10 * 10;
			}

			GAME_ENGINE->DrawBitmap(bitmapPtr, x + bitmapPtr->GetWidth()/10 * counter, y,frame);
		}			
				

	}

}




