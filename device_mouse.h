/*Device mouse using Window calls */

#ifndef INC_DEVICE_MOUSE_H_
#define INC_DEVICE_MOUSE_H_

#include "mcv_platform.h"
#include "input/input_device.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

#define MAX_MOUSE_MOVE_VALUE	15 

namespace Input {
  struct TInterface_Mouse_Data;

  class CDevice_Mouse : public IDevice {
  public:
    CDevice_Mouse();
    ~CDevice_Mouse();
    Vector2 global_pos_mouse;

    void updateMouse(TInterface_Mouse_Data& data) override;

    VEC2 n_pos;
    VEC2 position_delta;

  private:

    VEC2 previous_pos = VEC2(0,0);
    void init();
    void free();
    bool onSysMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    LPDIRECTINPUT8 g_pDI = nullptr;
    LPDIRECTINPUTDEVICE8 g_pMouse = nullptr;

#if defined(_DEBUG) | defined(_IMGUI_ACTIVE)
    bool bExclusive = false;
#else
    bool bExclusive = true;
#endif
  };

}

#endif

