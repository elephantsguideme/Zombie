#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <windowsx.h>
#include <math.h>
#include "res.h"

#define ID_TIMER_MUCHA 2018

HWND hwndText;
HWND hwndSecondaryWindow;
CHAR buffer[100];
int klikX, klikY, x, y, iCostam;
HINSTANCE hinst;
int iMuchaX, iMuchaY;
int iMuchaWidth, iMuchaHeight;
bool bIsMuchaZywa;
int iMuchaPredkoscX, iMuchaPredkoscY;
int iMuchaPredkosc, iMuchaAngle;
double dt;

INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{





  switch (uMsg)
  {
    
    case WM_INITDIALOG:
      srand(time(NULL)); //losowanie ziarna RNG
      iMuchaX = rand() % 400;
      iMuchaY = rand() %400;
      iMuchaHeight = 400;
      iMuchaWidth = 400;
      bIsMuchaZywa = true;
      iMuchaAngle = rand() % 360;
      iMuchaPredkosc = 50;
      dt = 0.1;
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

        bIsMuchaZywa = true;
        
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
    if (klikX >= iMuchaX && klikX < iMuchaX + iMuchaWidth && klikY >= iMuchaY && klikY < iMuchaY + iMuchaHeight)
      bIsMuchaZywa = false;

    RedrawWindow(hwndDig, NULL, NULL, RDW_INVALIDATE);


    break;
    return TRUE;

  case WM_PAINT:
    
    HDC hDC; // uchwyt do kontekstu urz¹dzenia 
    hDC = GetDC(hwndDig); // pobranie uchwytu do kontekstu okna 
                                      //BitBlt(hDC, 0, 0, 800, 600, GetDC(0), 0, 0, SRCCOPY);  //GetDC(0) pobranie kontekstu       
                                      //ekranu 

    HBITMAP hBitmapMuchaZywa, hBitmapMuchaTrup;
    hBitmapMuchaZywa = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP2));
    hBitmapMuchaTrup = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP3));
    HDC hDCBitmap;
    hDCBitmap = CreateCompatibleDC(hDC); //Utworzenie kopatybilengo kontekstu  
    if (bIsMuchaZywa) {
      SelectObject(hDCBitmap, hBitmapMuchaZywa); //Wybranie bitmapy w kontekscie 
    }
    else SelectObject(hDCBitmap, hBitmapMuchaTrup); //
    BitBlt(hDC, iMuchaX, iMuchaY, iMuchaWidth, iMuchaHeight, hDCBitmap, 0, 0, SRCCOPY);



    DeleteDC(hDCBitmap); //Usuniecie kontekstu 
    DeleteObject(hBitmapMuchaZywa);
    DeleteObject(hBitmapMuchaTrup);
    ReleaseDC(hwndDig, hDC); // Zwolnienie kontekstu urz¹dz
    break;
    return TRUE;

  case WM_TIMER: {
    switch (wParam) {
    case ID_TIMER_MUCHA:
      int iNewX;
      int iNewY;
      

       

      while (iMuchaX > 850 - iMuchaWidth || iMuchaX < 5 || iMuchaY > 530 - iMuchaHeight || iMuchaY < 5) {
        iMuchaAngle = (iMuchaAngle + 90) % 360;
      }

        iMuchaX += iMuchaPredkosc * cos(iMuchaAngle * 3.1416 / 180)*dt;
      iMuchaY += iMuchaPredkosc * sin(iMuchaAngle * 3.1416 / 180)*dt;

      /*
      if (iMuchaX > 850 - iMuchaWidth)iMuchaAngle = (180+ iMuchaAngle) % 360;
      if (iMuchaX < 5 )iMuchaAngle = (180+ iMuchaAngle) % 360;
      if (iMuchaY > 530 - iMuchaHeight)iMuchaAngle = (180 + iMuchaAngle) % 360;
      if (iMuchaY < 5)iMuchaAngle = (180 + iMuchaAngle) % 360;
      */
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

  SetTimer(hwndMainWindow, ID_TIMER_MUCHA, dt*500, NULL);

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
