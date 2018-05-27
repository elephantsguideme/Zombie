#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <windowsx.h>
#include <math.h>
#include "res.h"

#define ID_TIMER_MAIN 2018
#define ID_TIMER_REBIRTH 2019
#define ID_TIMER_ZOMBIE 2000




HWND hwndText;
HWND hwndSecondaryWindow;
HWND hwndMainWindow;
CHAR buffer[100];
int klikX, klikY, x, y, iCostam;
int iWindowWidth, iWindowHeight;
HINSTANCE hinst;
                 
double dt;
HBITMAP hBitmapMuchaZywa, hBitmapMuchaTrup;
HBITMAP hBitmapMuchaZywa2, hBitmapMuchaTrup2;
const int iNumOfMuchas=5;
double dTimeRebirth;

int iTimeIncs;
bool bAllMuchasDead;



void draw(HWND handle, HBITMAP hBitmap, int iX, int iY, int iWidth, int iHeight ) {
  HDC hDC; // uchwyt do kontekstu urz¹dzenia 
  hDC = GetDC(handle); 

  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hDC); //Utworzenie kopatybilengo kontekstu  
  
    SelectObject(hDCBitmap, hBitmap); //Wybranie bitmapy w kontekscie 
  
  BitBlt(hDC, iX, iY, iWidth, iHeight, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap); //Usuniecie kontekstu 
  DeleteObject(hBitmap);
 
  ReleaseDC(handle, hDC); // Zwolnienie kontekstu urz¹dz

  return ;
}

class sPlayer
{
public:
  int iPositionX, iPositionY;		//Pozycja
  int iWidth, iHeight;				//Rozmiar
 
  int iHealthPoints;				//Punkty życia
  int iVelocityX, iVelocityY;


  void playerMove(int dirX, int dirY) {          //{-1, 0, 1}
    this->iPositionX += this->iVelocityX *dt*dirX;
    this->iPositionY += this->iVelocityY *dt*dirY;
  }


  bool bIsAlive()  {
    return iHealthPoints;
  }
  void correct() {
    if (this->iPositionX > iWindowWidth - this->iWidth)this->iPositionX = iWindowWidth - this->iWidth;
      if (this->iPositionX < 0)this->iPositionX = 0;
    if (this->iPositionY > iWindowHeight - this->iHeight)this->iPositionY = iWindowHeight - this->iHeight;
    if (this->iPositionY < 0)this->iPositionY = 0;
  }

};
sPlayer* pPlayer;

class sZombie {
public:
	int iPositionX, iPositionY;		//Pozycja
	int iWidth, iHeight;				//Rozmiar

	int iHealthPoints;				//Punkty życia
	int iZombieSpeed;

	sZombie(int posX, int posY, int width, int height, int health, int speed) : iPositionX(posX), iPositionY(posY), iWidth(width), iHeight(height), iHealthPoints(health), iZombieSpeed(speed) {}
	bool isAlive() {
		if (iHealthPoints <= 0)return false;
	}
	void ZombieMove(int iplayerPositionX, int iplayerPositionY) {

		int X = iplayerPositionX - this->iPositionX;
		int Y = iplayerPositionY - this->iPositionY;
		double dir = sqrt((double)(X*X) + (double)(Y*Y));
		if (dir == 0)dir = 1;
		double cosineMove = X / dir;
		double sineMove = Y / dir;
		this->iPositionX += this->iZombieSpeed * cosineMove;
		this->iPositionY += this->iZombieSpeed * sineMove;

	}

	void Execute() {
		while (this->isAlive())
			this->ZombieMove(pPlayer->iPositionX, pPlayer->iPositionY);
		delete this;
	}
};




class sMucha
{
public:
  int iMuchaX, iMuchaY;
  int iMuchaWidth, iMuchaHeight;
  bool bIsMuchaZywa;
  int iMuchaPredkoscX, iMuchaPredkoscY;
  int iMuchaSzybkosc, iMuchaAngle;

  void muchaDraw() {
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP5));
    draw(hwndMainWindow, hBitmapBackgrnd, this->iMuchaX, this->iMuchaY, this->iMuchaWidth, this->iMuchaHeight);
  }

  void muchaChange(int iRand) {             
      this->iMuchaPredkoscX += iRand % 23 - 11;
      this->iMuchaPredkoscY += iRand % 19 - 9;         
    if (iRand % 5 == 0)  this->iMuchaPredkoscY += this->iMuchaPredkoscX / 3;
    if (iRand % 7 == 0)  this->iMuchaPredkoscX -= this->iMuchaPredkoscY / 3;
    
    if (iRand % 9 == 0) {
      if (this->iMuchaPredkoscX < 0)this->iMuchaPredkoscX = (2 * this->iMuchaPredkoscX - 80) / 3;
      else  this->iMuchaPredkoscX = (2 * this->iMuchaPredkoscX + 80) / 3;
    }
    if (iRand % 11 == 0) {
      if (this->iMuchaPredkoscY < 0)this->iMuchaPredkoscY = (2 * this->iMuchaPredkoscY - 80) / 3;
      else  this->iMuchaPredkoscY = (2 * this->iMuchaPredkoscY + 80) / 3;
    }
      }

  void muchaMove(){
    this->iMuchaX += this->iMuchaPredkoscX *dt;
  this->iMuchaY += this->iMuchaPredkoscY *dt;
  }

  void muchaCorrect() {
    if (this->iMuchaX > iWindowWidth - this->iMuchaWidth)this->iMuchaPredkoscX = -abs(this->iMuchaPredkoscX);
    if (this->iMuchaX < 0)this->iMuchaPredkoscX = abs(this->iMuchaPredkoscX);
    if (this->iMuchaY > iWindowHeight - this->iMuchaHeight)this->iMuchaPredkoscY = -abs(this->iMuchaPredkoscY);
    if (this->iMuchaY < 0)this->iMuchaPredkoscY = abs(this->iMuchaPredkoscY);
  }
};
//sMucha *pmchMucha[iNumOfMuchas];

sZombie *zombies;



INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


  switch (uMsg)
  {
    
    case WM_INITDIALOG:
      
		//Utworzenie gracza
      pPlayer = new sPlayer();
      pPlayer->iPositionX = rand() % 600;
      pPlayer->iPositionY = rand() % 400;
      pPlayer->iHeight = 100;
      pPlayer->iWidth = 100;
      pPlayer->iHealthPoints = 10;
      pPlayer->iVelocityX =  40;
      pPlayer->iVelocityY =  40;

      
      dt = 0.1;
	  dTimeRebirth = 10;
      iTimeIncs = 0;

	  zombies = new sZombie(pPlayer->iPositionX + 50, pPlayer->iPositionY + 50, 100, 100, 10, 2);

      HBITMAP hBitmapBackgrnd;
      hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP1));
      draw(hwndDig, hBitmapBackgrnd, 0, 0, 1500, 1500);
    return TRUE;
                
    
  case WM_COMMAND:
    switch (HIWORD(wParam)) {
    case BN_CLICKED:
      switch (LOWORD(wParam))
      {
        
      case IDC_BUTTONREVIVE:
        /*for (int i = 0; i < iNumOfMuchas; i++) {

          if(pmchMucha[i]->bIsMuchaZywa == false){
          pmchMucha[i]->bIsMuchaZywa = true;
          pmchMucha[i]->iMuchaX = rand() % 600;
          pmchMucha[i]->iMuchaY = rand() % 400;
        }
        }*/
        HBITMAP hBitmapBackgrnd;
        hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP1));
        draw(hwndDig, hBitmapBackgrnd, 0, 0, iWindowWidth, iWindowHeight);

        iTimeIncs = 0;
        return TRUE;
      }
      
    }       
       
  case WM_LBUTTONDOWN:
    /*
    CHAR szText[100];
    wsprintf(szText, "Kliknales punkt x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwndDig, szText, TEXT("Klikniecie"), MB_OK); 
    */
    klikX =LOWORD(lParam);
    klikY = HIWORD(lParam);
    /*for (int i = 0; i < iNumOfMuchas; i++) {

      if (klikX >= pmchMucha[i]->iMuchaX && klikX < pmchMucha[i]->iMuchaX + pmchMucha[i]->iMuchaWidth && klikY >= pmchMucha[i]->iMuchaY && klikY < pmchMucha[i]->iMuchaY + pmchMucha[i]->iMuchaHeight)
        pmchMucha[i]->bIsMuchaZywa = false;
    }*/
	if ((klikX > zombies->iPositionX) && (klikX < zombies->iPositionX + zombies->iWidth)
		&& (klikY >= zombies->iPositionY) && (klikY < zombies->iPositionY + zombies->iHeight))zombies->iHealthPoints -= 5;
	
    RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);
	HBITMAP hBitmapDziura;
	hBitmapDziura = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP4));
	draw(hwndDig, hBitmapDziura, klikX-10, klikY-10, 20, 20);
    break;
    return TRUE;

  case WM_PAINT:
    
    HBITMAP hBitmapPlayer;
    hBitmapPlayer= LoadBitmap(hinst, MAKEINTRESOURCE(IDB_PLAYER_80));
    draw(hwndDig, hBitmapPlayer, pPlayer->iPositionX, pPlayer->iPositionY, pPlayer->iWidth, pPlayer->iHeight); //Wybranie bitmapy w kontekscie 

	HBITMAP hBitmapZombie;
	hBitmapZombie = (HBITMAP)LoadImage(NULL, "..\\source\\zombie_01.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	draw(hwndDig, hBitmapZombie, zombies->iPositionX, zombies->iPositionY, zombies->iWidth, zombies->iHeight);
	

    HBITMAP hBitmapMucha;
   
    /*for (int i = 0; i < iNumOfMuchas; i++) {

      if (pmchMucha[i]->bIsMuchaZywa) {
        hBitmapMucha = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP2));
      }
      else hBitmapMucha = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP3));

      draw(hwndDig, hBitmapMucha, pmchMucha[i]->iMuchaX, pmchMucha[i]->iMuchaY, pmchMucha[i]->iMuchaWidth, pmchMucha[i]->iMuchaHeight); //Wybranie bitmapy w kontekscie 
    }*/

   
   // break;     
    return FALSE;

  case WM_TIMER: {
    switch (wParam) {
    case ID_TIMER_MAIN: {   
      
      bool dirY1 = GetKeyState(VK_UP) & 0x8000;                         //checking arrow buttons
      bool dirY2 = GetKeyState(VK_DOWN) & 0x8000;
      bool dirX1 = GetKeyState(VK_LEFT) & 0x8000;
      bool dirX2 = GetKeyState(VK_RIGHT) & 0x8000;
      
      int dirY = dirY2 - dirY1;
      int dirX = dirX2 - dirX1;

      pPlayer->playerMove(dirX, dirY);
      pPlayer->correct();

     

      RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);


      /*
      bAllMuchasDead = true;
      for (int i = 0; i < iNumOfMuchas; i++) {

        if (pmchMucha[i]->bIsMuchaZywa) {
          bAllMuchasDead = false;
          pmchMucha[i]->muchaDraw();
          
          int iRand = rand();
          if (iRand % 4 == 0)      pmchMucha[i]->muchaChange(iRand);
             
          pmchMucha[i]->muchaMove();
          pmchMucha[i]->muchaCorrect();

        }
      }

      iTimeIncs++;
      if (bAllMuchasDead == false) {
        wsprintf(buffer, "%d:%d.%d", iTimeIncs / 6000, (iTimeIncs / 100) % 60, iTimeIncs % 100);
        SetWindowText(hwndText, buffer);

        RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);
      }

      break;       */

    }             
	case ID_TIMER_ZOMBIE: {
		zombies->ZombieMove(pPlayer->iPositionX, pPlayer->iPositionY);
		if (zombies->iHealthPoints <= 0) {
			zombies->iZombieSpeed = 0;
		}
	}
                         
    }
    return TRUE;
  }

  case WM_WINDOWPOSCHANGED: {
	  HBITMAP hBitmapBackgrnd;
	  hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP1));
	  draw(hwndDig, hBitmapBackgrnd, 0, 0, iWindowWidth, iWindowHeight);

	  return FALSE;
  }

  case WM_SIZE: {
	  iWindowWidth= LOWORD(lParam);
		  iWindowHeight=HIWORD(lParam);
		  HBITMAP hBitmapBackgrnd;
		  hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		  draw(hwndDig, hBitmapBackgrnd, 0, 0, iWindowWidth, iWindowHeight);
	  return FALSE;
  }

  case WM_CLOSE:
    DestroyWindow(hwndDig);
    PostQuitMessage(0);
    return TRUE;
  }
  return FALSE;
}





int WINAPI WinMain(HINSTANCE histance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow)
{
  x = 1, y = 1000;
  hwndMainWindow = CreateDialog(histance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  hwndText = GetDlgItem(hwndMainWindow, IDC_BUTTONREVIVE);

  SetTimer(hwndMainWindow, ID_TIMER_MAIN, dt*100, NULL);
  SetTimer(hwndMainWindow, ID_TIMER_ZOMBIE, dt*100, NULL);

  ShowWindow(hwndMainWindow, iCmdShow);
  hinst = histance;

  //HWND hwndWindow = CreateDialog(histance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, NULL);
 // hwndSecondaryWindow = hwndWindow;
 // ShowWindow(hwndWindow, iCmdShow);


  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    }


 

  return 0;
}
