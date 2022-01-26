/**
 * CWB API (Plain C WebBrowser Control mini API) - Implementation file
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software in
 *     a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must not
 *     be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 */
#include <wchar.h>
#include <windows.h>

#include "cwb.h"

/**
 * callback function used to intercept navigation
 */
void BeforeNavigate(LPCWSTR lpUrl, SHORT* pCancel)
{
  // Set cancel value to cancel navigation
  *pCancel = 1;

  // Compose message
  wchar_t msg[256];

  swprintf(msg, 256, L"The navigation to '%ls' has been intercepted and "
                      "canceled. Edit BeforeNavigate callback function to "
                      "allow navigation.", lpUrl);

  // Open message box dialog
  MessageBoxW(NULL, msg, L"Web Browser Example - Before Navigate",
              MB_OK|MB_ICONINFORMATION);
}


/**
 * global variable to count running window
 */
static unsigned __nWnd = 0;


/**
 * common Window Procedure function for created windows
 */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
  case WM_CREATE:
    // Increment running window count
    __nWnd++;
    return 0;

  case WM_DESTROY:
    // Release HTML View from window
    WebBrowserRelease(hWnd);
    // Decrease running window count
    __nWnd--;
    // If no window remain, quit application
    if(!__nWnd) PostQuitMessage(0);
    return 1;

  case WM_SIZE:
    // Adjust HTML View to window client area
    WebBrowserResize(hWnd, LOWORD(lParam), HIWORD(lParam));
    return 0;
  }

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


/**
 * program main entry
 */
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hInstNULL, LPSTR lpszCmdLine, int nCmdShow)
{
  MSG	uMsg;
  HWND hWnd;

	// initialize the OLE library.
	OleInitialize(NULL);

  hWnd = WebBrowserCreate(0, WS_OVERLAPPEDWINDOW, NULL, NULL, hInstance, BeforeNavigate);
  if(hWnd) {

    // Set some additional window parameters
    SetWindowText(hWnd, "Web Browser Example - Document Write");
    SetWindowPos(hWnd, NULL, 0, 0, 980, 520, SWP_NOMOVE);

    // HTML data string
    const wchar_t* html = L"<head><style>"
                            "body {border:0; text-align:center; overflow:auto}"
                          "</head></style>"
                          "<body oncontextmenu=\"return false;\">"
                            "<div>"
                              "<h2>Welcome to HTML View Sample</h2>"
                              "<p>This is an example of HTML data written to <b>HTML View</b>. "
                              "You can click on <a href=\"http://www.perdu.com\">this link</a> "
                              "to trigger the callback.</p>"
                            "</div>"
                          "<body>";

    // Write HTML to HTML View document
    WebBrowserDocumentWrite(hWnd, html);

    // Show the window
    ShowWindow(hWnd, nCmdShow);
  }


  hWnd = WebBrowserCreate(0, WS_OVERLAPPEDWINDOW, NULL, NULL, hInstance, NULL);
  if(hWnd) {

    // Set some additional window parameters
    SetWindowText(hWnd, "Web Browser Example - Navitage");
    SetWindowPos(hWnd, NULL, 0, 0, 980, 520, SWP_NOMOVE);

    // Open location in HTML View
    WebBrowserNavigate(hWnd, L"http://www.w3.org/TR/1999/PR-html40-19990824/");

    // Show window
    ShowWindow(hWnd, nCmdShow);
  }

  // Window message loop
  while(GetMessage(&uMsg, 0, 0, 0)) {

    TranslateMessage(&uMsg);
    DispatchMessage(&uMsg);
  }

  // Free the OLE library.
  OleUninitialize();

  return 0;

}
