#include "mcv_platform.h"
#include "device_mouse.h"
#include "input/interfaces/interface_mouse.h"
#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <cassert>
#include <stdio.h>
#include "windows/app.h"

using namespace Input;
bool left_button = false;
bool right_button = false;


#define SAMPLE_BUFFER_SIZE  16      // arbitrary number of buffer elements
int cont = 0;
CDevice_Mouse::CDevice_Mouse() {
}

CDevice_Mouse::~CDevice_Mouse() {
  free();
}

void CDevice_Mouse::init() {
  HRESULT hr;
  HWND hWnd = CApp::get().getHWnd();

  // Cleanup any previous call first
  KillTimer(hWnd, 0);
  free();

  // Obtain an interface to the system mouse device.
  if (FAILED(hr = g_pDI->CreateDevice(GUID_SysMouse, &g_pMouse, NULL))) {
    fatal("Could not create mouse device from Direct Input\n");
    return;
  }
}
bool CDevice_Mouse::onSysMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_LBUTTONDOWN:
    //left_button = true;

    break;
  }
  return false;
}
void CDevice_Mouse::free() {
  // Unacquire the device one last time just in case 
  // the app tried to exit while the device is still acquired.
  if (g_pMouse)
    g_pMouse->Unacquire();

  // Release any DirectInput objects.
  SAFE_RELEASE(g_pMouse);
}

void CDevice_Mouse::updateMouse(TInterface_Mouse_Data& data) {

  if (CApp::get().isActive() == true) {

    CApp::get().getWindowsRectangle();
    POINT p;

    GetCursorPos(&p);
    global_pos_mouse.x = (float)p.x;
    global_pos_mouse.y = (float)p.y;

    CApp::WindowsSize windows = CApp::get().getWindowsRectangle();

    n_pos.x = getAbsoluteRatioFromValue((float)p.x, (float)windows.right, (float)windows.left);
    n_pos.y = getAbsoluteRatioFromValue((float)p.y, (float)windows.bottom, (float)windows.top);

    data.position_delta.x = (previous_pos.x - global_pos_mouse.x);   //clenaed deltas
    data.position_delta.y = (previous_pos.y - global_pos_mouse.y);
    position_delta.x = (previous_pos.x - global_pos_mouse.x);
    position_delta.y = (previous_pos.y - global_pos_mouse.y);
    //cont++;
    if (CApp::getSwitchMouse() == false && CApp::getMouseGameplay() == true && CApp::get().isActive()) {
      global_pos_mouse = VEC2(std::round(windows.left + (windows.right - windows.left) * 0.5f), std::round(windows.top + (windows.bottom - windows.top) * 0.5f));
      SetCursorPos((int)global_pos_mouse.x, (int)global_pos_mouse.y);
    }

    previous_pos = global_pos_mouse;
    data.buttons[Input::EMouseButton::LEFT] = GetAsyncKeyState(VK_LBUTTON) ? true : false;
    data.buttons[Input::EMouseButton::RIGHT] = GetAsyncKeyState(VK_RBUTTON) ? true : false;
    const int MIDDLE_MOUSE_BUTTON = 4;
    data.buttons[Input::EMouseButton::MIDDLE] = GetAsyncKeyState(MIDDLE_MOUSE_BUTTON) ? true : false;
  }
}

