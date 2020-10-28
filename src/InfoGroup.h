#ifndef __INFO_GROUP_H__
#define __INFO_GROUP_H__

#include <Windows.h>

namespace IW
{
    struct TargetElement
    {
        BSTR name{nullptr};
        BSTR controleType{nullptr};

        RECT boundingRectangle{0, 0, 0, 0};

        BOOL isContent{false};
        BOOL isControle{false};
    };

    struct TargetElementList
    {
        TargetElement *elements{nullptr};
        int length{0};
    };

    struct Annotation
    {
        BSTR name{nullptr};
        BSTR author{nullptr};
        BSTR dateTime{nullptr};

        TargetElement target;
    };

    struct Dock
    {
        BSTR position{nullptr};
    };

    struct ExpandCollapse
    {
        BSTR state{nullptr};
    };

    struct LegacyAccessibility
    {
        BSTR action{nullptr};
        BSTR description{nullptr};
        BSTR name{nullptr};
        BSTR value{nullptr};
    };

    struct RangeValue
    {
        BOOL isReadOnly{false};

        double bigChange{0.0};
        double smallChange{0.0};
        double currentValue{0.0};
        double maximumValue{0.0};
        double minimumValue{0.0};
    };

    struct ScrollData
    {
        BOOL isScrollable{false};

        double scrollPercent{0.0f};
        double viewSize{0.0f};
    };

    struct Scroll
    {
        ScrollData horizontalScroll;
        ScrollData verticallScroll;
    };

    struct Selection
    {
        BOOL isRequired;
        BOOL isMultipleSelectionAllowed;

        TargetElementList targets;
    };

    struct SelectionItem
    {
        BOOL isSelected;
    };

    struct Style
    {
        BSTR name{nullptr};
        BSTR extendedProperties{nullptr};
        BSTR shape{nullptr};

        int fillColor{0};
        int fillPatternColor{0};
        int styleId{0};
    };

    struct Text
    {
        BSTR text{nullptr};
    };

    struct Toggle
    {
        BSTR state{nullptr};
    };

    struct Transform
    {
        BOOL canMove{false};
        BOOL canResize{false};
        BOOL canRotate{false};
    };

    struct Value
    {
        BOOL isReadOnly{false};
        BSTR value{nullptr};
    };

    struct Window
    {
        BOOL canMaximize{false};
        BOOL canMinimize{false};
        BOOL isModal{false};
        BOOL isTopMost{false};

        BSTR interactionState{nullptr};
        BSTR visualState{nullptr};
    };

    struct InfoGroup
    {
        BSTR name{nullptr};
        BSTR controleType{nullptr};

        RECT boundingRectangle{0, 0, 0, 0};

        BOOL isContent{false};
        BOOL isControle{false};

        Annotation *annotation{nullptr};
        Dock *dock{nullptr};
        ExpandCollapse *expandCollapse{nullptr};
        LegacyAccessibility *legacyAccessibility{nullptr};
        RangeValue *rangeValue{nullptr};
        Scroll *scroll{nullptr};
        Selection *selection{nullptr};
        SelectionItem *selectionItem{nullptr};
        Style *style{nullptr};
        Text *text{nullptr};
        Toggle *toggle{nullptr};
        Transform *transform{nullptr};
        Value *value{nullptr};
        Window *window{nullptr};
    };

    InfoGroup getInfoGroupFromPoint(long x, long y);
    void releaseInfoGroup(InfoGroup &ig);

} // namespace IW

#endif // !__INFO_GROUP_H__
