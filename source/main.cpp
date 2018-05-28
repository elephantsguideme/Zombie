#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <windowsx.h>
#include <math.h>
#include "res.h"

#define ID_TIMER_MAIN 2018
#define ID_TIMER_REBIRTH 2019




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
const int iNumOfMuchas = 5;
const int iNumOfZombies = 5;

double dTimeRebirth;

int iTimeIncs;
bool bAllMuchasDead;



void draw(HWND handle, HBITMAP hBitmap, int iX, int iY, int iWidth, int iHeight) {
  HDC hDC; // uchwyt do kontekstu urz¹dzenia 
  hDC = GetDC(handle);

  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hDC); //Utworzenie kopatybilengo kontekstu  

  SelectObject(hDCBitmap, hBitmap); //Wybranie bitmapy w kontekscie 

  BitBlt(hDC, iX, iY, iWidth, iHeight, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap); //Usuniecie kontekstu 
  DeleteObject(hBitmap);

  ReleaseDC(handle, hDC); // Zwolnienie kontekstu urz¹dz

  return;
}

class sBullet {
  int iPositionX, iPositionY;
  int iWidth, iHeight;

  int iVelocityX, iVelocityY;
  int dirX, dirY;         //{-1, 0, 1}



public:
  void bulletMove() {
    this->iPositionX += this->iVelocityX *dt*dirX;
    this->iPositionY += this->iVelocityY *dt*dirY;
  }

  void bulletDraw() {
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BULLET_3));
    draw(hwndMainWindow, hBitmapBackgrnd, this->iPositionX, this->iPositionY, this->iWidth, this->iHeight);
  }
  void bulletBackgrndDraw() {
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WHITE_3));
    draw(hwndMainWindow, hBitmapBackgrnd, this->iPositionX, this->iPositionY, this->iWidth, this->iHeight);
  }

  sBullet(int iX, int iY, int idX, int idY) :
    iPositionX(iX), iPositionY(iY), dirX(idX), dirY(idY) {
    iVelocityX = 100;
    iVelocityY = 100;
    iWidth = 3;
    iHeight = 3;
  };


};
sBullet* pBullet;     //TODO: many bullets (vector? list?)

class sPlayer
{
public:
  int iPositionX, iPositionY;
  int iWidth, iHeight;

  int iHealthPoints;
  int iVelocityX, iVelocityY;
  int dirX, dirY;         //{-1, 0, 1}

  void playerMove() {
    this->iPositionX += this->iVelocityX *dt*dirX;
    this->iPositionY += this->iVelocityY *dt*dirY;
  }

  void playerDraw() {
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_PLAYER_80));
    draw(hwndMainWindow, hBitmapBackgrnd, this->iPositionX, this->iPositionY, this->iWidth, this->iHeight);
  }
  void playerBackgrndDraw() {
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WHITE_PLAYER_80));
    draw(hwndMainWindow, hBitmapBackgrnd, this->iPositionX, this->iPositionY, this->iWidth, this->iHeight);
  }

  void playerShoot() {

    pBullet = new sBullet(iPositionX + iWidth / 2, iPositionY + iHeight / 2, dirX, dirY);
  }


  bool bIsAlive() {
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


class sZombie
{
public:
  int iPositionX, iPositionY;
  int iWidth, iHeight;

  int iHealthPoints;
  int iVelocityX, iVelocityY;
  int iSpeed;

  void zombieMove() {
    this->iPositionX += this->iVelocityX *dt;
    this->iPositionY += this->iVelocityY *dt;
  }

  void zombieChangeDir(int iRand) {                                 //zombies should follow the player but
                                                                    //they should also 'feel' clumsy
    int iDiffY = pPlayer->iPositionY - pPlayer->iHeight / 2 - iPositionY + iHeight / 2;
    int iDiffX = pPlayer->iPositionX - pPlayer->iWidth / 2 - iPositionX + iWidth / 2;

    float fBase = 1.0;
    if (iDiffY || iDiffX) fBase = sqrt(iDiffY * iDiffY + iDiffX * iDiffX);


    float fVelocityY = iDiffY / fBase;
    float fVelocityX = iDiffX / fBase;
    iVelocityY = fVelocityY * iSpeed + iVelocityY / 3;
    iVelocityX = fVelocityX * iSpeed + iVelocityX / 3;




  }


  bool bIsAlive() {
    return iHealthPoints;
  }


  void zombieDraw() {
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_ZOMBIE_100));
    draw(hwndMainWindow, hBitmapBackgrnd, this->iPositionX, this->iPositionY, this->iWidth, this->iHeight);
  }
  void zombieBackgrndDraw() {
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WHITE_ZOMBIE_100));
    draw(hwndMainWindow, hBitmapBackgrnd, this->iPositionX, this->iPositionY, this->iWidth, this->iHeight);
  }








  void correct() {
    if (this->iPositionX > iWindowWidth - this->iWidth)this->iPositionX = iWindowWidth - this->iWidth;
    if (this->iPositionX < 0)this->iPositionX = 0;
    if (this->iPositionY > iWindowHeight - this->iHeight)this->iPositionY = iWindowHeight - this->iHeight;
    if (this->iPositionY < 0)this->iPositionY = 0;
  }


};
sZombie* pZombie[iNumOfZombies];           //TODO: more zombies




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

  void muchaMove() {
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
sMucha *pmchMucha[iNumOfMuchas];

void collisionDetectionZ2Z(sZombie* pZombie1, sZombie* pZombie2) {            // is Zombie1 center inside Zombie2?
  int iCenterY = pZombie1->iPositionY + pZombie1->iHeight / 2;                // zombies can overlap "a bit"
  int iCenterX = pZombie1->iPositionX + pZombie1->iWidth / 2;
  if (iCenterY > pZombie2->iPositionY && iCenterY < pZombie2->iPositionY + pZombie2->iHeight &&
    iCenterX > pZombie2->iPositionX && iCenterX < pZombie2->iPositionX + pZombie2->iWidth) {
    pZombie1->iPositionY += (pZombie2->iPositionY + pZombie2->iHeight - iCenterY) / 20;
    pZombie1->iPositionX += (pZombie2->iPositionX + pZombie2->iWidth - iCenterX) / 10;



  }
}


INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{





  switch (uMsg)
  {

  case WM_INITDIALOG:

    pPlayer = new sPlayer();
    pPlayer->iPositionX = rand() % 300;
    pPlayer->iPositionY = rand() % 200;
    pPlayer->iHeight = 80;
    pPlayer->iWidth = 58;
    pPlayer->iHealthPoints = 10;
    pPlayer->iVelocityX = 40;
    pPlayer->iVelocityY = 40;



    for (int i = 0; i < iNumOfZombies; i++) {
      pZombie[i] = new sZombie();
      pZombie[i]->iPositionX = 300 + rand() % 300;
      pZombie[i]->iPositionY = 200 + rand() % 200;
      pZombie[i]->iHeight = 100;
      pZombie[i]->iWidth = 53;
      pZombie[i]->iHealthPoints = 3;
      pZombie[i]->iVelocityX = 10;
      pZombie[i]->iVelocityY = 10;
      pZombie[i]->iSpeed = 15;

    }

    for (int i = 0; i < iNumOfMuchas; i++) {
      pmchMucha[i] = new sMucha();
      pmchMucha[i]->iMuchaX = rand() % 600;
      pmchMucha[i]->iMuchaY = rand() % 400;
      pmchMucha[i]->iMuchaHeight = 100;
      pmchMucha[i]->iMuchaWidth = 100;
      pmchMucha[i]->bIsMuchaZywa = true;
      pmchMucha[i]->iMuchaAngle = rand() % 360;
      pmchMucha[i]->iMuchaSzybkosc = rand() % 80;
      pmchMucha[i]->iMuchaPredkoscX = rand() % 80;
      pmchMucha[i]->iMuchaPredkoscY = rand() % 80;
    }

    dt = 0.1;
    dTimeRebirth = 10;
    iTimeIncs = 0;
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WHITE_BACKGROUND));
    draw(hwndDig, hBitmapBackgrnd, 0, 0, 1500, 1500);
    return TRUE;


  case WM_COMMAND:
    switch (HIWORD(wParam)) {
    case BN_CLICKED:
      switch (LOWORD(wParam))
      {

      case IDC_BUTTONREVIVE:
        for (int i = 0; i < iNumOfMuchas; i++) {

          if (pmchMucha[i]->bIsMuchaZywa == false) {
            pmchMucha[i]->bIsMuchaZywa = true;
            pmchMucha[i]->iMuchaX = rand() % 600;
            pmchMucha[i]->iMuchaY = rand() % 400;
          }
        }
        HBITMAP hBitmapBackgrnd;
        hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WHITE_BACKGROUND));
        draw(hwndDig, hBitmapBackgrnd, 0, 0, iWindowWidth, iWindowHeight);

        iTimeIncs = 0;
        return TRUE;
      }

    }

  case WM_KEYDOWN: {                                  //NOT WORKING
    switch (wParam)
    {
    case VK_SHIFT:
      if (true)pPlayer->playerShoot();


      break;

    default:
      break;
    }

    return TRUE;

  }
                   /*
                   case WM_LBUTTONDOWN:
                   CHAR szText[100];
                   wsprintf(szText, "Kliknales punkt x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
                   MessageBox(hwndDig, szText, TEXT("Klikniecie"), MB_OK);

                   klikX =LOWORD(lParam);
                   klikY = HIWORD(lParam);
                   for (int i = 0; i < iNumOfMuchas; i++) {
                   if (klikX >= pmchMucha[i]->iMuchaX && klikX < pmchMucha[i]->iMuchaX + pmchMucha[i]->iMuchaWidth && klikY >= pmchMucha[i]->iMuchaY && klikY < pmchMucha[i]->iMuchaY + pmchMucha[i]->iMuchaHeight)
                   pmchMucha[i]->bIsMuchaZywa = false;
                   }

                   RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);
                   HBITMAP hBitmapDziura;
                   hBitmapDziura = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP4));
                   draw(hwndDig, hBitmapDziura, klikX-10, klikY-10, 20, 20);
                   break;
                   return TRUE;         */

  case WM_PAINT:

    //HBITMAP hBitmapPlayer;
    //hBitmapPlayer= LoadBitmap(hinst, MAKEINTRESOURCE(IDB_PLAYER_80));
    //draw(hwndDig, hBitmapPlayer, pPlayer->iPositionX, pPlayer->iPositionY, pPlayer->iWidth, pPlayer->iHeight); //Wybranie bitmapy w kontekscie 
    pPlayer->playerDraw();

    for (int i = 0; i < iNumOfZombies; i++) {
      pZombie[i]->zombieDraw();

    }
    if (pBullet)
      pBullet->bulletDraw();

    /*HBITMAP hBitmapMucha;

    for (int i = 0; i < iNumOfMuchas; i++) {
    if (pmchMucha[i]->bIsMuchaZywa) {
    hBitmapMucha = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP2));
    }
    else hBitmapMucha = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP3));
    draw(hwndDig, hBitmapMucha, pmchMucha[i]->iMuchaX, pmchMucha[i]->iMuchaY, pmchMucha[i]->iMuchaWidth, pmchMucha[i]->iMuchaHeight); //Wybranie bitmapy w kontekscie
    }
    */

    // break;     
    return FALSE;

  case WM_TIMER: {
    switch (wParam) {
    case ID_TIMER_MAIN: {

      bool dirY1 = GetKeyState(VK_UP) & 0x8000;                         //checking arrow buttons
      bool dirY2 = GetKeyState(VK_DOWN) & 0x8000;
      bool dirX1 = GetKeyState(VK_LEFT) & 0x8000;
      bool dirX2 = GetKeyState(VK_RIGHT) & 0x8000;

      pPlayer->dirY = dirY2 - dirY1;
      pPlayer->dirX = dirX2 - dirX1;

      pPlayer->playerBackgrndDraw();
      pPlayer->playerMove();
      pPlayer->correct();
      //pPlayer->playerDraw();

      if (pBullet) {
        pBullet->bulletBackgrndDraw();
        pBullet->bulletMove();
      }

      for (int i = 0; i < iNumOfZombies; i++) {
        int iRand = rand();

        pZombie[i]->zombieBackgrndDraw();
        if (iRand % 15 == 0)pZombie[i]->zombieChangeDir(iRand);
        pZombie[i]->zombieMove();

        pZombie[i]->zombieBackgrndDraw();
        for (int j = 0; j < i; j++) {
         // collisionDetectionZ2Z(pZombie[i], pZombie[j]);
        }

        pZombie[i]->correct();
        //pZombie[i]->zombieDraw();

      }
      // pZombie[1]->zombieBackgrndDraw();
      // collisionDetectionZ2Z(pZombie[1], pZombie[2]);
      //pZombie[1]->zombieDraw();

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


    }
    return TRUE;
  }

  case WM_WINDOWPOSCHANGED: {
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WHITE_BACKGROUND));
    draw(hwndDig, hBitmapBackgrnd, 0, 0, iWindowWidth, iWindowHeight);

    return FALSE;
  }

  case WM_SIZE: {
    iWindowWidth = LOWORD(lParam);
    iWindowHeight = HIWORD(lParam);
    HBITMAP hBitmapBackgrnd;
    hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WHITE_BACKGROUND));
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
  //x = 1, y = 1000;
  hwndMainWindow = CreateDialog(histance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  hwndText = GetDlgItem(hwndMainWindow, IDC_BUTTONREVIVE);

  SetTimer(hwndMainWindow, ID_TIMER_MAIN, dt * 100, NULL);
  //SetTimer(hwndMainWindow, ID_TIMER_REBIRTH, dTimeRebirth * 1000, NULL);

  ShowWindow(hwndMainWindow, iCmdShow);
  hinst = histance;

  HBITMAP hBitmapBackgrnd;
  hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WHITE_BACKGROUND));
  draw(hwndMainWindow, hBitmapBackgrnd, 0, 0, iWindowWidth, iWindowHeight);

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