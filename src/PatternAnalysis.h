#ifndef __PATTERN_ANALYSIS_H__
#define __PATTERN_ANALYSIS_H__

namespace IW
{

    template <int PatternID, typename Pattern, typename F>
    void doIfPatternExists(IUIAutomationElement *element, F &&f)
    {
        Pattern *pattern;
        HRESULT hr = element->GetCurrentPatternAs(PatternID, __uuidof(Pattern), (void **)&pattern);

        if (SUCCEEDED(hr) && pattern)
        {
            f(pattern);
            pattern->Release();
        }
    }

} // namespace IW

#endif // !__PATTERN_ANALYSIS_H__
