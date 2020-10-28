#include "Automation.h"

namespace IW
{

    BOOL initializeAutomation(IUIAutomation **ppAutomation)
    {
        CoInitialize(NULL);

        HRESULT hr = CoCreateInstance(__uuidof(CUIAutomation), NULL, CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), reinterpret_cast<void **>(ppAutomation));

        return (SUCCEEDED(hr));
    }

    void releaseAutomation(IUIAutomation *pAutomation)
    {
        pAutomation->Release();

        CoUninitialize();
    }

} // namespace IW
