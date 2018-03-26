#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <windowsx.h>
#include <math.h>
#include "res.h"

#define ID_TIMER_MUCHA 2018
#define ID_TIMER_REBIRTH 2019




HWND hwndText;
HWND hwndSecondaryWindow;
CHAR buffer[100];
int klikX, klikY, x, y, iCostam;
int iWindowWidth, iWindowHeight;
HINSTANCE hinst;
//int iMuchaX, iMuchaY;
//int iMuchaWidth, iMuchaHeight;
//bool bIsMuchaZywa;
//int iMuchaPredkoscX, iMuchaPredkoscY;
//int iMuchaSzybkosc, iMuchaAngle;                   
double dt;
HBITMAP hBitmapMuchaZywa, hBitmapMuchaTrup;
HBITMAP hBitmapMuchaZywa2, hBitmapMuchaTrup2;
const int iNumOfMuchas=10;
double dTimeRebirth;
//Mucha *pMucha1 = new Mucha();
int iTimeIncs;
bool bAllMuchasDead;

class sMucha
{
public:
	int iMuchaX, iMuchaY;
	int iMuchaWidth, iMuchaHeight;
	bool bIsMuchaZywa;
	int iMuchaPredkoscX, iMuchaPredkoscY;
	int iMuchaSzybkosc, iMuchaAngle;

};
sMucha *pmchMucha[iNumOfMuchas];

void draw(HWND handle, HBITMAP hBitmap, int iX, int iY, int iWidth, int iHeight ) {
  HDC hDC; // uchwyt do kontekstu urz¹dzenia 
  hDC = GetDC(handle); // pobranie uchwytu do kontekstu okna 
                        //BitBlt(hDC, 0, 0, 800, 600, GetDC(0), 0, 0, SRCCOPY);  //GetDC(0) pobranie kontekstu       
                        //ekranu 


  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hDC); //Utworzenie kopatybilengo kontekstu  
  
    SelectObject(hDCBitmap, hBitmap); //Wybranie bitmapy w kontekscie 
  
  BitBlt(hDC, iX, iY, iWidth, iHeight, hDCBitmap, 0, 0, SRCCOPY);



  DeleteDC(hDCBitmap); //Usuniecie kontekstu 
  DeleteObject(hBitmap);
 
  ReleaseDC(handle, hDC); // Zwolnienie kontekstu urz¹dz

  return ;
}


INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{





  switch (uMsg)
  {
    
    case WM_INITDIALOG:
      
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
      /*
      pMucha1->iMuchaX = rand() % 600;
      pMucha1->iMuchaY = rand() %400;
      pMucha1->iMuchaHeight = 100;
      pMucha1->iMuchaWidth = 100;
      pMucha1->bIsMuchaZywa = true;
      pMucha1->iMuchaAngle = rand() % 360;
      pMucha1->iMuchaSzybkosc = 50;
      pMucha1->iMuchaPredkoscX = 50;
      pMucha1->iMuchaPredkoscY = 30;       */
      dt = 0.1;
	  dTimeRebirth = 10;
      iTimeIncs = 0;
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
        for (int i = 0; i < iNumOfMuchas; i++) {

          if(pmchMucha[i]->bIsMuchaZywa == false){
          pmchMucha[i]->bIsMuchaZywa = true;
          pmchMucha[i]->iMuchaX = rand() % 600;
          pmchMucha[i]->iMuchaY = rand() % 400;
        }
        }
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
    for (int i = 0; i < iNumOfMuchas; i++) {

      if (klikX >= pmchMucha[i]->iMuchaX && klikX < pmchMucha[i]->iMuchaX + pmchMucha[i]->iMuchaWidth && klikY >= pmchMucha[i]->iMuchaY && klikY < pmchMucha[i]->iMuchaY + pmchMucha[i]->iMuchaHeight)
        pmchMucha[i]->bIsMuchaZywa = false;
    }
	
    RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);
	HBITMAP hBitmapDziura;
	hBitmapDziura = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP4));
	draw(hwndDig, hBitmapDziura, klikX-10, klikY-10, 20, 20);
    break;
    return TRUE;

  case WM_PAINT:
    
   

    HBITMAP hBitmapMucha;
   
    for (int i = 0; i < iNumOfMuchas; i++) {

      if (pmchMucha[i]->bIsMuchaZywa) {
        hBitmapMucha = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP2));
      }
      else hBitmapMucha = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP3));

      draw(hwndDig, hBitmapMucha, pmchMucha[i]->iMuchaX, pmchMucha[i]->iMuchaY, pmchMucha[i]->iMuchaWidth, pmchMucha[i]->iMuchaHeight); //Wybranie bitmapy w kontekscie 
    }

   
   // break;     
    return FALSE;

  case WM_TIMER: {
    switch (wParam) {
	case ID_TIMER_MUCHA: {
		bAllMuchasDead = true;
		for (int i = 0; i < iNumOfMuchas; i++) {

			if (pmchMucha[i]->bIsMuchaZywa) {
				bAllMuchasDead = false;
				HBITMAP hBitmapBackgrnd;
				hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP5));
				draw(hwndDig, hBitmapBackgrnd, pmchMucha[i]->iMuchaX, pmchMucha[i]->iMuchaY, pmchMucha[i]->iMuchaWidth, pmchMucha[i]->iMuchaHeight); //Wybranie bitmapy w kontekscie 

				int iRand = rand();
				if (iRand % 3 == 0)  pmchMucha[i]->iMuchaPredkoscX += iRand % 23 - 11;
				if (iRand % 4 == 0)  pmchMucha[i]->iMuchaPredkoscY += iRand % 19 - 9;
				if (iRand % 17 == 0)  pmchMucha[i]->iMuchaPredkoscY += pmchMucha[i]->iMuchaPredkoscX / 3;
				if (iRand % 19 == 0)  pmchMucha[i]->iMuchaPredkoscX -= pmchMucha[i]->iMuchaPredkoscY / 3;
				if (iRand % 31 == 0) {
					if (pmchMucha[i]->iMuchaPredkoscX < 0)pmchMucha[i]->iMuchaPredkoscX = (2 * pmchMucha[i]->iMuchaPredkoscX - 80) / 3;
					else  pmchMucha[i]->iMuchaPredkoscX = (2 * pmchMucha[i]->iMuchaPredkoscX + 80) / 3;
				}
				if (iRand % 37 == 0) {
					if (pmchMucha[i]->iMuchaPredkoscY < 0)pmchMucha[i]->iMuchaPredkoscY = (2 * pmchMucha[i]->iMuchaPredkoscY - 80) / 3;
					else  pmchMucha[i]->iMuchaPredkoscY = (2 * pmchMucha[i]->iMuchaPredkoscY + 80) / 3;
				}

				pmchMucha[i]->iMuchaX += pmchMucha[i]->iMuchaPredkoscX *dt;
				pmchMucha[i]->iMuchaY += pmchMucha[i]->iMuchaPredkoscY *dt;

				if (pmchMucha[i]->iMuchaX > iWindowWidth - pmchMucha[i]->iMuchaWidth)pmchMucha[i]->iMuchaPredkoscX = -abs(pmchMucha[i]->iMuchaPredkoscX);
				if (pmchMucha[i]->iMuchaX < 0)pmchMucha[i]->iMuchaPredkoscX = abs(pmchMucha[i]->iMuchaPredkoscX);
				if (pmchMucha[i]->iMuchaY > iWindowHeight - pmchMucha[i]->iMuchaHeight)pmchMucha[i]->iMuchaPredkoscY = -abs(pmchMucha[i]->iMuchaPredkoscY);
				if (pmchMucha[i]->iMuchaY < 0)pmchMucha[i]->iMuchaPredkoscY = abs(pmchMucha[i]->iMuchaPredkoscY);



			}
		}




		iTimeIncs++;
		if (bAllMuchasDead == false) {
			wsprintf(buffer, "%d:%d.%d", iTimeIncs / 6000, (iTimeIncs / 100) % 60, iTimeIncs % 100);
			SetWindowText(hwndText, buffer);

			RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);
		}

		break;
	}

	case ID_TIMER_REBIRTH:{
		for (int i = 0; i < iNumOfMuchas; i++) {
			if (pmchMucha[i]->bIsMuchaZywa == false) {
				HBITMAP hBitmapBackgrnd;
				hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP5));
				draw(hwndDig, hBitmapBackgrnd, pmchMucha[i]->iMuchaX, pmchMucha[i]->iMuchaY, pmchMucha[i]->iMuchaWidth, pmchMucha[i]->iMuchaHeight);
				pmchMucha[i]->iMuchaX = rand() % (iWindowWidth - pmchMucha[i]->iMuchaWidth);
				pmchMucha[i]->iMuchaY= rand() % (iWindowHeight- pmchMucha[i]->iMuchaHeight);

				pmchMucha[i]->iMuchaPredkoscX = rand() % 80;
				pmchMucha[i]->iMuchaPredkoscY = rand() % 80;
				pmchMucha[i]->bIsMuchaZywa = true;
			}
		}
		break;
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
  HWND hwndMainWindow = CreateDialog(histance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  hwndText = GetDlgItem(hwndMainWindow, IDC_BUTTONREVIVE);

  SetTimer(hwndMainWindow, ID_TIMER_MUCHA, dt*100, NULL);
  SetTimer(hwndMainWindow, ID_TIMER_REBIRTH, dTimeRebirth * 1000, NULL);

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
