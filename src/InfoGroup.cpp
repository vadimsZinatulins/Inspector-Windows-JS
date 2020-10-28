#include "InfoGroup.h"
#include "Automation.h"
#include "PatternAnalysis.h"

#include <iostream>

namespace IW
{
    void getInfoFromElement(IUIAutomationElement *element, TargetInfo &targetInfo);

    void getInfoFromElement(IUIAutomationElement *element, TargetInfo &targetInfo)
    {
        element->get_CurrentName(&targetInfo.name);
        element->get_CurrentLocalizedControlType(&targetInfo.controleType);
        element->get_CurrentBoundingRectangle(&targetInfo.boundingRectangle);
        element->get_CurrentIsContentElement(&targetInfo.isContent);
        element->get_CurrentIsControlElement(&targetInfo.isControle);

        doIfPatternExists<UIA_AnnotationPatternId, IUIAutomationAnnotationPattern>(element, [&](IUIAutomationAnnotationPattern *pattern) {
            Annotation *annotation = new Annotation();

            pattern->get_CurrentAnnotationTypeName(&annotation->name);
            pattern->get_CurrentAuthor(&annotation->author);
            pattern->get_CurrentDateTime(&annotation->dateTime);
            pattern->get_CurrentAnnotationTypeName(&annotation->name);

            IUIAutomationElement *annotaionTargetElement;
            HRESULT hr = pattern->get_CurrentTarget(&annotaionTargetElement);
            if (SUCCEEDED(hr) && annotaionTargetElement)
            {
                getInfoFromElement(annotaionTargetElement, annotation->target);

                annotaionTargetElement->Release();
            }

            targetInfo.annotation = annotation;
        });

        doIfPatternExists<UIA_DockPatternId, IUIAutomationDockPattern>(element, [&](IUIAutomationDockPattern *pattern) {
            BSTR positions[] = {
                (BSTR)L"DockPosition_Top",
                (BSTR)L"DockPosition_Left",
                (BSTR)L"DockPosition_Bottom",
                (BSTR)L"DockPosition_Right",
                (BSTR)L"DockPosition_Fill",
                (BSTR)L"DockPosition_None",
            };

            DockPosition posIndex;
            HRESULT hr = pattern->get_CurrentDockPosition(&posIndex);

            if (SUCCEEDED(hr))
            {
                Dock *dock = new Dock();
                dock->position = positions[posIndex];

                targetInfo.dock = dock;
            }
        });

        doIfPatternExists<UIA_ExpandCollapsePatternId, IUIAutomationExpandCollapsePattern>(element, [&](IUIAutomationExpandCollapsePattern *pattern) {
            BSTR states[] = {
                (BSTR)L"ExpandCollapseState_Collapsed",
                (BSTR)L"ExpandCollapseState_Expanded",
                (BSTR)L"ExpandCollapseState_PartiallyExpanded",
                (BSTR)L"ExpandCollapseState_LeafNode"};

            ExpandCollapseState stateIndex;
            HRESULT hr = pattern->get_CurrentExpandCollapseState(&stateIndex);

            if (SUCCEEDED(hr))
            {
                ExpandCollapse *expandCollapse = new ExpandCollapse();
                expandCollapse->state = states[stateIndex];

                targetInfo.expandCollapse = expandCollapse;
            }
        });

        doIfPatternExists<UIA_LegacyIAccessiblePatternId, IUIAutomationLegacyIAccessiblePattern>(element, [&](IUIAutomationLegacyIAccessiblePattern *pattern) {
            LegacyAccessibility *legacyAccessibility = new LegacyAccessibility;

            pattern->get_CurrentDefaultAction(&legacyAccessibility->action);
            pattern->get_CurrentDescription(&legacyAccessibility->description);
            pattern->get_CurrentName(&legacyAccessibility->name);
            pattern->get_CurrentValue(&legacyAccessibility->value);

            targetInfo.legacyAccessibility = legacyAccessibility;
        });

        doIfPatternExists<UIA_RangeValuePatternId, IUIAutomationRangeValuePattern>(element, [&](IUIAutomationRangeValuePattern *pattern) {
            RangeValue *rangeValue = new RangeValue();

            pattern->get_CurrentIsReadOnly(&rangeValue->isReadOnly);
            pattern->get_CurrentLargeChange(&rangeValue->bigChange);
            pattern->get_CurrentSmallChange(&rangeValue->smallChange);
            pattern->get_CurrentValue(&rangeValue->currentValue);
            pattern->get_CurrentMaximum(&rangeValue->maximumValue);
            pattern->get_CurrentMinimum(&rangeValue->minimumValue);

            targetInfo.rangeValue = rangeValue;
        });

        doIfPatternExists<UIA_ScrollPatternId, IUIAutomationScrollPattern>(element, [&](IUIAutomationScrollPattern *pattern) {
            Scroll *scroll = new Scroll();

            pattern->get_CurrentHorizontallyScrollable(&scroll->horizontalScroll.isScrollable);
            pattern->get_CurrentHorizontalScrollPercent(&scroll->horizontalScroll.scrollPercent);
            pattern->get_CurrentHorizontalViewSize(&scroll->horizontalScroll.viewSize);

            pattern->get_CurrentVerticallyScrollable(&scroll->verticallScroll.isScrollable);
            pattern->get_CurrentVerticalScrollPercent(&scroll->verticallScroll.scrollPercent);
            pattern->get_CurrentVerticalViewSize(&scroll->verticallScroll.viewSize);

            targetInfo.scroll = scroll;
        });

        doIfPatternExists<UIA_SelectionPatternId, IUIAutomationSelectionPattern>(element, [&](IUIAutomationSelectionPattern *pattern) {
            Selection *selection = new Selection();

            pattern->get_CurrentIsSelectionRequired(&selection->isRequired);
            pattern->get_CurrentCanSelectMultiple(&selection->isMultipleSelectionAllowed);

            IUIAutomationElementArray *targetsArray;
            HRESULT hr = pattern->GetCurrentSelection(&targetsArray);
            if (SUCCEEDED(hr) && targetsArray)
            {
                targetsArray->get_Length(&selection->targets.length);

                selection->targets.elements = new TargetInfo[selection->targets.length];

                for (int i = 0; i < selection->targets.length; i++)
                {
                    IUIAutomationElement *target;
                    HRESULT hr = targetsArray->GetElement(i, &target);

                    if (SUCCEEDED(hr) && target)
                    {
                        getInfoFromElement(target, selection->targets.elements[i]);

                        target->Release();
                    }
                }

                targetsArray->Release();
            }

            targetInfo.selection = selection;
        });

        doIfPatternExists<UIA_SelectionItemPatternId, IUIAutomationSelectionItemPattern>(element, [&](IUIAutomationSelectionItemPattern *pattern) {
            SelectionItem *selectionItem = new SelectionItem();

            pattern->get_CurrentIsSelected(&selectionItem->isSelected);

            targetInfo.selectionItem = selectionItem;
        });

        doIfPatternExists<UIA_StylesPatternId, IUIAutomationStylesPattern>(element, [&](IUIAutomationStylesPattern *pattern) {
            Style *style = new Style();

            pattern->get_CurrentStyleName(&style->name);
            pattern->get_CurrentExtendedProperties(&style->extendedProperties);
            pattern->get_CurrentShape(&style->shape);
            pattern->get_CurrentFillColor(&style->fillColor);
            pattern->get_CurrentFillPatternColor(&style->fillPatternColor);
            pattern->get_CurrentStyleId(&style->styleId);

            targetInfo.style = style;
        });

        doIfPatternExists<UIA_TextPatternId, IUIAutomationTextPattern>(element, [&](IUIAutomationTextPattern *pattern) {
            Text *text = new Text();

            IUIAutomationTextRange *range;
            HRESULT hr = pattern->get_DocumentRange(&range);
            if (SUCCEEDED(hr))
            {
                range->GetText(-1, &text->text);

                range->Release();
            }

            targetInfo.text = text;
        });

        doIfPatternExists<UIA_TogglePatternId, IUIAutomationTogglePattern>(element, [&](IUIAutomationTogglePattern *pattern) {
            BSTR states[] = {
                (BSTR)L"ToggleState_Off",
                (BSTR)L"ToggleState_On",
                (BSTR)L"ToggleState_Indeterminate"};

            ToggleState stateIndex;
            HRESULT hr = pattern->get_CurrentToggleState(&stateIndex);
            if (SUCCEEDED(hr))
            {
                Toggle *toggle = new Toggle();
                toggle->state = states[stateIndex];

                targetInfo.toggle = toggle;
            }
        });

        doIfPatternExists<UIA_TransformPatternId, IUIAutomationTransformPattern>(element, [&](IUIAutomationTransformPattern *pattern) {
            Transform *transform = new Transform;

            pattern->get_CurrentCanMove(&transform->canMove);
            pattern->get_CurrentCanResize(&transform->canResize);
            pattern->get_CurrentCanRotate(&transform->canRotate);

            targetInfo.transform = transform;
        });

        doIfPatternExists<UIA_ValuePatternId, IUIAutomationValuePattern>(element, [&](IUIAutomationValuePattern *pattern) {
            Value *value = new Value();

            pattern->get_CurrentIsReadOnly(&value->isReadOnly);
            pattern->get_CurrentValue(&value->value);

            targetInfo.value = value;
        });

        doIfPatternExists<UIA_WindowPatternId, IUIAutomationWindowPattern>(element, [&](IUIAutomationWindowPattern *pattern) {
            BSTR interactionStates[] = {
                (BSTR)L"WindowInteractionState_Running",
                (BSTR)L"WindowInteractionState_Closing",
                (BSTR)L"WindowInteractionState_ReadyForUserInteraction",
                (BSTR)L"WindowInteractionState_BlockedByModalWindow",
                (BSTR)L"WindowInteractionState_NotResponding"};

            BSTR visualStates[] = {
                (BSTR)L"WindowVisualState_Normal",
                (BSTR)L"WindowVisualState_Maximized",
                (BSTR)L"WindowVisualState_Minimized"};

            Window *window = new Window();

            pattern->get_CurrentCanMaximize(&window->canMaximize);
            pattern->get_CurrentCanMinimize(&window->canMinimize);
            pattern->get_CurrentIsModal(&window->isModal);
            pattern->get_CurrentIsTopmost(&window->isTopMost);

            WindowInteractionState interationStateIndex;
            pattern->get_CurrentWindowInteractionState(&interationStateIndex);
            window->interactionState = interactionStates[interationStateIndex];

            WindowVisualState visualStateIndex;
            pattern->get_CurrentWindowVisualState(&visualStateIndex);
            window->visualState = visualStates[visualStateIndex];

            targetInfo.window = window;
        });
    }

    TargetInfo getTargetInfoFromPoint(long x, long y)
    {
        IUIAutomation *pAutomation;

        initializeAutomation(&pAutomation);

        IUIAutomationElement *pElementAtMouse;
        HRESULT hr = pAutomation->ElementFromPoint({x, y}, &pElementAtMouse);

        TargetInfo ti;

        if (SUCCEEDED(hr) && pElementAtMouse)
        {
            getInfoFromElement(pElementAtMouse, ti);

            pElementAtMouse->Release();
        }

        releaseAutomation(pAutomation);

        return ti;
    }

    void releaseTargetInfo(TargetInfo &ti)
    {
        // Clear name string
        if (ti.name)
        {
            SysFreeString(ti.name);

            ti.name = nullptr;
        }

        // Clear controle type string
        if (ti.controleType)
        {
            SysFreeString(ti.controleType);

            ti.controleType = nullptr;
        }

        // Clear annotation
        if (ti.annotation)
        {
            // Clear annotation name string
            if (ti.annotation->name)
            {
                SysFreeString(ti.annotation->name);

                ti.annotation->name = nullptr;
            }

            // Clear annotation author string
            if (ti.annotation->author)
            {
                SysFreeString(ti.annotation->author);

                ti.annotation->author = nullptr;
            }

            // Clear annotation date time string
            if (ti.annotation->dateTime)
            {
                SysFreeString(ti.annotation->dateTime);

                ti.annotation->dateTime = nullptr;
            }

            // Clear annotation target
            releaseTargetInfo(ti.annotation->target);

            delete ti.annotation;

            ti.annotation = nullptr;
        }

        // Clear dock
        if (ti.dock)
        {
            delete ti.dock;

            ti.dock = nullptr;
        }

        // Clear expandCollapse
        if (ti.expandCollapse)
        {
            delete ti.expandCollapse;

            ti.expandCollapse = nullptr;
        }

        // Clear legacy accessibility
        if (ti.legacyAccessibility)
        {
            // Clear legacy accessibility action string
            if (ti.legacyAccessibility->action)
            {
                SysFreeString(ti.legacyAccessibility->action);

                ti.legacyAccessibility->action = nullptr;
            }

            // Clear legacy accessibility description string
            if (ti.legacyAccessibility->description)
            {
                SysFreeString(ti.legacyAccessibility->description);

                ti.legacyAccessibility->description = nullptr;
            }

            // Clear legacy accessibility name string
            if (ti.legacyAccessibility->name)
            {
                SysFreeString(ti.legacyAccessibility->name);

                ti.legacyAccessibility->name = nullptr;
            }

            // Clear legacy accessibility value string
            if (ti.legacyAccessibility->value)
            {
                SysFreeString(ti.legacyAccessibility->value);

                ti.legacyAccessibility->value = nullptr;
            }

            delete ti.legacyAccessibility;

            ti.legacyAccessibility = nullptr;
        }

        // Clear range value
        if (ti.rangeValue)
        {
            delete ti.rangeValue;

            ti.rangeValue = nullptr;
        }

        // Clear scroll
        if (ti.scroll)
        {
            delete ti.scroll;

            ti.scroll = nullptr;
        }

        // Clear selection
        if (ti.selection)
        {
            // Clear all selection targets
            for (int i = 0; i < ti.selection->targets.length; i++)
                releaseTargetInfo(ti.selection->targets.elements[i]);

            delete[] ti.selection->targets.elements;
            delete ti.selection;

            ti.selection = nullptr;
        }

        // Clear selection item
        if (ti.selectionItem)
        {
            delete ti.selectionItem;

            ti.selectionItem = nullptr;
        }

        // Clear style
        if (ti.style)
        {
            // Clear style name string
            if (ti.style->name)
            {
                SysFreeString(ti.style->name);

                ti.style->name = nullptr;
            }

            // Clear style extendedProperties string
            if (ti.style->extendedProperties)
            {
                SysFreeString(ti.style->extendedProperties);

                ti.style->extendedProperties = nullptr;
            }

            // Clear style shape string
            if (ti.style->shape)
            {
                SysFreeString(ti.style->shape);

                ti.style->shape = nullptr;
            }

            delete ti.style;

            ti.style = nullptr;
        }

        // Clear text
        if (ti.text)
        {
            if (ti.text->text)
            {
                SysFreeString(ti.text->text);

                ti.text->text = nullptr;
            }

            delete ti.text;

            ti.text = nullptr;
        }

        // Clear toggle
        if (ti.toggle)
        {
            delete ti.toggle;

            ti.toggle = nullptr;
        }

        // Clear transform
        if (ti.transform)
        {
            delete ti.transform;

            ti.transform = nullptr;
        }

        // Clear value
        if (ti.value)
        {
            if (ti.value->value)
            {
                SysFreeString(ti.value->value);

                ti.value->value = nullptr;
            }

            delete ti.value;

            ti.value = nullptr;
        }

        // Clear window
        if (ti.window)
        {
            delete ti.window;

            ti.window = nullptr;
        }
    }
} // namespace IW
