#include <Windows.h>
#include <stdio.h>
#include "res.h"

HWND hwndText;
HWND hwndSecondaryWindow;
CHAR buffer[100];
int x, y, iCostam;
HINSTANCE hinst;

INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{





  switch (uMsg)
  {
    
    case WM_CREATE:
   
    return TRUE;
                 
  case WM_COMMAND:
    switch (HIWORD(wParam)) {
    case BN_CLICKED:
      switch (LOWORD(wParam))
      {
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
      }

    }       
       
  case WM_LBUTTONDOWN:
    /*
    CHAR szText[100];
    wsprintf(szText, "Kliknales punkt x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwndDig, szText, TEXT("Klikniecie"), MB_OK); 
    */
    return TRUE;

  case WM_PAINT:
    //Rysowanie okna
  
    //SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));    //Pobranie i aktywacja                   
    //czcionki 
    //SetTextColor(hDC, RGB(255, 0, 0));                   //Ustawienie koloru 
    //TextOut(hDC, 0, 0, TEXT("Text output to client area."), 27);


    break;

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
  hwndText = GetDlgItem(hwndMainWindow, IDC_BUTTON4);
  ShowWindow(hwndMainWindow, iCmdShow);
  hinst = histance;

  HWND hwndWindow = CreateDialog(histance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  hwndSecondaryWindow = hwndWindow;
  ShowWindow(hwndWindow, iCmdShow);


  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    }


 

  return 0;
}
