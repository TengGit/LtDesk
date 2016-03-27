/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "tgui.h"
#include "MyWindow.h"
#include "MyDef.h"

#pragma data_seg("LtDesk")
int volatile LtDesk_InstanceCount = 0;
#pragma data_seg()
#pragma comment(linker,"/section:LtDesk,RWS")

using namespace tl;

int APIENTRY Main(int) {
    if (LtDesk_InstanceCount != 0) {
        MessageBox(NULL, msgInstance, NULL, MB_ICONINFORMATION);
        return 0;
    }
    LtDesk_InstanceCount++;
    MyWindow wnd;
    int result;
    CoInitialize(NULL);
    wnd.Run();
    result = Application.RunApplication();
	CoUninitialize();
    return result;
}
