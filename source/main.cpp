#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <windowsx.h>
#include <math.h>
#include "res.h"

#define ID_TIMER_MUCHA 2018


class Mucha
{
public:
  int iMuchaX, iMuchaY;
  int iMuchaWidth, iMuchaHeight;
  bool bIsMuchaZywa;
  int iMuchaPredkoscX, iMuchaPredkoscY;
  int iMuchaSzybkosc, iMuchaAngle;

};


HWND hwndText;
HWND hwndSecondaryWindow;
CHAR buffer[100];
int klikX, klikY, x, y, iCostam;
HINSTANCE hinst;
//int iMuchaX, iMuchaY;
//int iMuchaWidth, iMuchaHeight;
//bool bIsMuchaZywa;
//int iMuchaPredkoscX, iMuchaPredkoscY;
//int iMuchaSzybkosc, iMuchaAngle;                   
double dt;
HBITMAP hBitmapMuchaZywa, hBitmapMuchaTrup;
HBITMAP hBitmapMuchaZywa2, hBitmapMuchaTrup2;
const int iNumOfMuchas=5;
Mucha *pMucha[iNumOfMuchas];
//Mucha *pMucha1 = new Mucha();




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
        pMucha[i] = new Mucha();
        pMucha[i]->iMuchaX = rand() % 600;
        pMucha[i]->iMuchaY = rand() % 400;
        pMucha[i]->iMuchaHeight = 100;
        pMucha[i]->iMuchaWidth = 100;
        pMucha[i]->bIsMuchaZywa = true;
        pMucha[i]->iMuchaAngle = rand() % 360;
        pMucha[i]->iMuchaSzybkosc = 50;
        pMucha[i]->iMuchaPredkoscX = 50;
        pMucha[i]->iMuchaPredkoscY = 30;
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

      HBITMAP hBitmapBackgrnd;
      hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP1));
      draw(hwndDig, hBitmapBackgrnd, 0, 0, 1500, 1500);
    return TRUE;
                
    
  case WM_COMMAND:
    switch (HIWORD(wParam)) {
    case BN_CLICKED:
      switch (LOWORD(wParam))
      {
        /*
      case IDC_BUTTON1:
        HDC hDC; // uchwyt do kontekstu urz¹dzenia 
        hDC = GetDC(hwndSecondaryWindow); // pobranie uchwytu do kontekstu okna 
                              //BitBlt(hDC, 0, 0, 800, 600, GetDC(0), 0, 0, SRCCOPY);  //GetDC(0) pobranie kontekstu       
                              //ekranu 

        HBITMAP hBitmap;
        hBitmap = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP2));
        HDC hDCBitmap;
        hDCBitmap = CreateCompatibleDC(hDC); //Utworzenie kopatybilengo kontekstu  
        SelectObject(hDCBitmap, hBitmap); //Wybranie bitmapy w kontekscie 
        BitBlt(hDC, 0, 0,900, 400, hDCBitmap, 0, 0, SRCCOPY);
        
        DeleteDC(hDCBitmap); //Usuniecie kontekstu 
        DeleteObject(hBitmap);

        ReleaseDC(hwndSecondaryWindow, hDC); // Zwolnienie kontekstu urz¹dzenia 

        return TRUE;

     

      case IDC_BUTTON3:
       

        hDC = GetDC(hwndSecondaryWindow); // pobranie uchwytu do kontekstu okna 
                              //BitBlt(hDC, 0, 0, 800, 600, GetDC(0), 0, 0, SRCCOPY);  //GetDC(0) pobranie kontekstu       
                              //ekranu 

        
        hBitmap = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP3));
        
        hDCBitmap = CreateCompatibleDC(hDC); //Utworzenie kopatybilengo kontekstu  
        SelectObject(hDCBitmap, hBitmap); //Wybranie bitmapy w kontekscie 
        BitBlt(hDC, 0, 0, 900, 400, hDCBitmap, 0, 0, SRCCOPY);
        
        DeleteDC(hDCBitmap); //Usuniecie kontekstu 
        DeleteObject(hBitmap);

        ReleaseDC(hwndSecondaryWindow, hDC); // Zwolnienie kontekstu urz¹dzenia 

      
        return TRUE;
        */
      case IDC_BUTTONREVIVE:
        for (int i = 0; i < iNumOfMuchas; i++) {

          if(pMucha[i]->bIsMuchaZywa == false){
          pMucha[i]->bIsMuchaZywa = true;
          pMucha[i]->iMuchaX = rand() % 600;
          pMucha[i]->iMuchaY = rand() % 400;
        }
        }
        HBITMAP hBitmapBackgrnd;
        hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP1));
        draw(hwndDig, hBitmapBackgrnd, 0, 0, 1500, 1500);
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

      if (klikX >= pMucha[i]->iMuchaX && klikX < pMucha[i]->iMuchaX + pMucha[i]->iMuchaWidth && klikY >= pMucha[i]->iMuchaY && klikY < pMucha[i]->iMuchaY + pMucha[i]->iMuchaHeight)
        pMucha[i]->bIsMuchaZywa = false;
    }
    RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);


    break;
    return TRUE;

  case WM_PAINT:
    
   

    HBITMAP hBitmapMucha;
   
    for (int i = 0; i < iNumOfMuchas; i++) {

      if (pMucha[i]->bIsMuchaZywa) {
        hBitmapMucha = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP2));
      }
      else hBitmapMucha = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP3));

      draw(hwndDig, hBitmapMucha, pMucha[i]->iMuchaX, pMucha[i]->iMuchaY, pMucha[i]->iMuchaWidth, pMucha[i]->iMuchaHeight); //Wybranie bitmapy w kontekscie 
    }

   
    break;     
    return TRUE;

  case WM_TIMER: {
    switch (wParam) {
    case ID_TIMER_MUCHA:
      for (int i = 0; i < iNumOfMuchas; i++) {

        if (pMucha[i]->bIsMuchaZywa) {
          HBITMAP hBitmapBackgrnd;
          hBitmapBackgrnd = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP5));
          draw(hwndDig, hBitmapBackgrnd, pMucha[i]->iMuchaX, pMucha[i]->iMuchaY, pMucha[i]->iMuchaWidth, pMucha[i]->iMuchaHeight); //Wybranie bitmapy w kontekscie 






          pMucha[i]->iMuchaX += pMucha[i]->iMuchaPredkoscX *dt;
          pMucha[i]->iMuchaY += pMucha[i]->iMuchaPredkoscY *dt;
          if (pMucha[i]->iMuchaX > 800)pMucha[i]->iMuchaPredkoscX = -abs(pMucha[i]->iMuchaPredkoscX);
          if (pMucha[i]->iMuchaX < 0)pMucha[i]->iMuchaPredkoscX = abs(pMucha[i]->iMuchaPredkoscX);
          if (pMucha[i]->iMuchaY > 550)pMucha[i]->iMuchaPredkoscY = -abs(pMucha[i]->iMuchaPredkoscY);
          if (pMucha[i]->iMuchaY < 0)pMucha[i]->iMuchaPredkoscY = abs(pMucha[i]->iMuchaPredkoscY);

          int iRand = rand();
          if (iRand % 3 == 0)  pMucha[i]->iMuchaPredkoscX += iRand % 23 - 11;
          if (iRand % 4 == 0)  pMucha[i]->iMuchaPredkoscY += iRand % 19 - 9;
          if (iRand % 17 == 0)  pMucha[i]->iMuchaPredkoscY += pMucha[i]->iMuchaPredkoscX / 3;
          if (iRand % 19 == 0)  pMucha[i]->iMuchaPredkoscX -= pMucha[i]->iMuchaPredkoscY / 3;
          if (iRand % 31 == 0) {
            if (pMucha[i]->iMuchaPredkoscX < 0)pMucha[i]->iMuchaPredkoscX = (2 * pMucha[i]->iMuchaPredkoscX - 80) / 3;
            else  pMucha[i]->iMuchaPredkoscX = (2 * pMucha[i]->iMuchaPredkoscX + 80) / 3;
          }
          if (iRand % 37 == 0) {
            if (pMucha[i]->iMuchaPredkoscY < 0)pMucha[i]->iMuchaPredkoscY = (2 * pMucha[i]->iMuchaPredkoscY - 80) / 3;
            else  pMucha[i]->iMuchaPredkoscY = (2 * pMucha[i]->iMuchaPredkoscY + 80) / 3;
          }
          // if (iRand % 91 == 0) iMuchaPredkoscX /= 2;
          // if (iRand % 103 == 0) iMuchaPredkoscY /= 2;
        }





        //  while (iMuchaX > 850 - iMuchaWidth || iMuchaX < 5 || iMuchaY > 530 - iMuchaHeight || iMuchaY < 5) {
          //  iMuchaAngle = (iMuchaAngle + 90) % 360; }
               /*
            iMuchaX += iMuchaPredkosc * cos(iMuchaAngle * 3.1416 / 180)*dt;
          iMuchaY += iMuchaPredkosc * sin(iMuchaAngle * 3.1416 / 180)*dt;


          if (iMuchaX > 850 - iMuchaWidth)iMuchaAngle = (180+ iMuchaAngle) % 360;
          if (iMuchaX < 5 )iMuchaAngle = (180+ iMuchaAngle) % 360;
          if (iMuchaY > 530 - iMuchaHeight)iMuchaAngle = (180 + iMuchaAngle) % 360;
          if (iMuchaY < 5)iMuchaAngle = (180 + iMuchaAngle) % 360;
              */
         }
        RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);
      
      break;
    }}

    return TRUE;


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
  //hwndText = GetDlgItem(hwndMainWindow, IDC_BUTTON4);

  SetTimer(hwndMainWindow, ID_TIMER_MUCHA, dt*100, NULL);

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
