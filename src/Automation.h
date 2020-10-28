#ifndef __AUTOMATION_H__
#define __AUTOMATION_H__

#include <Windows.h>
#include <UIAutomation.h>

namespace IW
{
    BOOL initializeAutomation(IUIAutomation **ppAutomation);

    void releaseAutomation(IUIAutomation *pAutomation);
} // namespace IW

#endif // !__AUTOMATION_H__
