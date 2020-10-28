#include "InfoGroup.h"
#include "Automation.h"
#include "PatternAnalysis.h"

#include <iostream>

namespace IW
{
    InfoGroup getInfoGroupFromPoint(long x, long y)
    {
        std::cout << x << " " << y << std::endl;

        IUIAutomation *pAutomation;

        initializeAutomation(&pAutomation);

        IUIAutomationElement *pElementAtMouse;
        HRESULT hr = pAutomation->ElementFromPoint({x, y}, &pElementAtMouse);

        InfoGroup ig;

        if (SUCCEEDED(hr) && pElementAtMouse)
        {
            std::cout << "Succeeded: " << pElementAtMouse << std::endl;
            pElementAtMouse->get_CurrentName(&ig.name);
            pElementAtMouse->get_CurrentLocalizedControlType(&ig.controleType);
            pElementAtMouse->get_CurrentBoundingRectangle(&ig.boundingRectangle);
            pElementAtMouse->get_CurrentIsContentElement(&ig.isContent);
            pElementAtMouse->get_CurrentIsControlElement(&ig.isControle);

            doIfPatternExists<UIA_AnnotationPatternId, IUIAutomationAnnotationPattern>(pElementAtMouse, [&](IUIAutomationAnnotationPattern *pattern) {
                Annotation *annotation = new Annotation();

                pattern->get_CurrentAnnotationTypeName(&annotation->name);
                pattern->get_CurrentAuthor(&annotation->author);
                pattern->get_CurrentDateTime(&annotation->dateTime);
                pattern->get_CurrentAnnotationTypeName(&annotation->name);

                IUIAutomationElement *target;
                HRESULT hr = pattern->get_CurrentTarget(&target);
                if (SUCCEEDED(hr) && target)
                {
                    target->get_CurrentName(&annotation->target.name);
                    target->get_CurrentLocalizedControlType(&annotation->target.controleType);
                    target->get_CurrentBoundingRectangle(&annotation->target.boundingRectangle);

                    target->get_CurrentIsContentElement(&annotation->target.isContent);
                    target->get_CurrentIsControlElement(&annotation->target.isControle);

                    target->Release();
                }

                ig.annotation = annotation;
            });

            doIfPatternExists<UIA_DockPatternId, IUIAutomationDockPattern>(pElementAtMouse, [&](IUIAutomationDockPattern *pattern) {
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

                    ig.dock = dock;
                }
            });

            doIfPatternExists<UIA_ExpandCollapsePatternId, IUIAutomationExpandCollapsePattern>(pElementAtMouse, [&](IUIAutomationExpandCollapsePattern *pattern) {
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

                    ig.expandCollapse = expandCollapse;
                }
            });

            doIfPatternExists<UIA_LegacyIAccessiblePatternId, IUIAutomationLegacyIAccessiblePattern>(pElementAtMouse, [&](IUIAutomationLegacyIAccessiblePattern *pattern) {
                LegacyAccessibility *legacyAccessibility = new LegacyAccessibility;

                pattern->get_CurrentDefaultAction(&legacyAccessibility->action);
                pattern->get_CurrentDescription(&legacyAccessibility->description);
                pattern->get_CurrentName(&legacyAccessibility->name);
                pattern->get_CurrentValue(&legacyAccessibility->value);

                ig.legacyAccessibility = legacyAccessibility;
            });

            doIfPatternExists<UIA_RangeValuePatternId, IUIAutomationRangeValuePattern>(pElementAtMouse, [&](IUIAutomationRangeValuePattern *pattern) {
                RangeValue *rangeValue = new RangeValue();

                pattern->get_CurrentIsReadOnly(&rangeValue->isReadOnly);
                pattern->get_CurrentLargeChange(&rangeValue->bigChange);
                pattern->get_CurrentSmallChange(&rangeValue->smallChange);
                pattern->get_CurrentValue(&rangeValue->currentValue);
                pattern->get_CurrentMaximum(&rangeValue->maximumValue);
                pattern->get_CurrentMinimum(&rangeValue->minimumValue);

                ig.rangeValue = rangeValue;
            });

            doIfPatternExists<UIA_ScrollPatternId, IUIAutomationScrollPattern>(pElementAtMouse, [&](IUIAutomationScrollPattern *pattern) {
                Scroll *scroll = new Scroll();

                pattern->get_CurrentHorizontallyScrollable(&scroll->horizontalScroll.isScrollable);
                pattern->get_CurrentHorizontalScrollPercent(&scroll->horizontalScroll.scrollPercent);
                pattern->get_CurrentHorizontalViewSize(&scroll->horizontalScroll.viewSize);

                pattern->get_CurrentVerticallyScrollable(&scroll->verticallScroll.isScrollable);
                pattern->get_CurrentVerticalScrollPercent(&scroll->verticallScroll.scrollPercent);
                pattern->get_CurrentVerticalViewSize(&scroll->verticallScroll.viewSize);

                ig.scroll = scroll;
            });

            doIfPatternExists<UIA_SelectionPatternId, IUIAutomationSelectionPattern>(pElementAtMouse, [&](IUIAutomationSelectionPattern *pattern) {
                Selection *selection = new Selection();

                pattern->get_CurrentIsSelectionRequired(&selection->isRequired);
                pattern->get_CurrentCanSelectMultiple(&selection->isMultipleSelectionAllowed);

                IUIAutomationElementArray *targetsArray;
                HRESULT hr = pattern->GetCurrentSelection(&targetsArray);
                if (SUCCEEDED(hr) && targetsArray)
                {
                    targetsArray->get_Length(&selection->targets.length);

                    selection->targets.elements = new TargetElement[selection->targets.length];

                    for (int i = 0; i < selection->targets.length; i++)
                    {
                        IUIAutomationElement *target;
                        HRESULT hr = targetsArray->GetElement(i, &target);

                        if (SUCCEEDED(hr) && target)
                        {
                            target->get_CurrentName(&selection->targets.elements[i].name);
                            target->get_CurrentLocalizedControlType(&selection->targets.elements[i].controleType);
                            target->get_CurrentBoundingRectangle(&selection->targets.elements[i].boundingRectangle);

                            target->get_CurrentIsContentElement(&selection->targets.elements[i].isContent);
                            target->get_CurrentIsControlElement(&selection->targets.elements[i].isControle);

                            target->Release();
                        }
                    }

                    targetsArray->Release();
                }

                ig.selection = selection;
            });

            doIfPatternExists<UIA_SelectionItemPatternId, IUIAutomationSelectionItemPattern>(pElementAtMouse, [&](IUIAutomationSelectionItemPattern *pattern) {
                SelectionItem *selectionItem = new SelectionItem();

                pattern->get_CurrentIsSelected(&selectionItem->isSelected);

                ig.selectionItem = selectionItem;
            });

            doIfPatternExists<UIA_StylesPatternId, IUIAutomationStylesPattern>(pElementAtMouse, [&](IUIAutomationStylesPattern *pattern) {
                Style *style = new Style();

                pattern->get_CurrentStyleName(&style->name);
                pattern->get_CurrentExtendedProperties(&style->extendedProperties);
                pattern->get_CurrentShape(&style->shape);
                pattern->get_CurrentFillColor(&style->fillColor);
                pattern->get_CurrentFillPatternColor(&style->fillPatternColor);
                pattern->get_CurrentStyleId(&style->styleId);

                ig.style = style;
            });

            doIfPatternExists<UIA_TextPatternId, IUIAutomationTextPattern>(pElementAtMouse, [&](IUIAutomationTextPattern *pattern) {
                Text *text = new Text();

                IUIAutomationTextRange *range;
                HRESULT hr = pattern->get_DocumentRange(&range);
                if (SUCCEEDED(hr))
                {
                    range->GetText(-1, &text->text);

                    range->Release();
                }

                ig.text = text;
            });

            doIfPatternExists<UIA_TogglePatternId, IUIAutomationTogglePattern>(pElementAtMouse, [&](IUIAutomationTogglePattern *pattern) {
                BSTR states[] = {
                    (BSTR)L"ToggleState_Off",
                    (BSTR)L"ToggleState_On",
                    (BSTR)L"ToggleState_Indeterminate"};

                ToggleState stateIndex;
                HRESULT he = pattern->get_CurrentToggleState(&stateIndex);
                if (SUCCEEDED(hr))
                {
                    Toggle *toggle = new Toggle();
                    toggle->state = states[stateIndex];

                    ig.toggle = toggle;
                }
            });

            doIfPatternExists<UIA_TransformPatternId, IUIAutomationTransformPattern>(pElementAtMouse, [&](IUIAutomationTransformPattern *pattern) {
                Transform *transform = new Transform;

                pattern->get_CurrentCanMove(&transform->canMove);
                pattern->get_CurrentCanResize(&transform->canResize);
                pattern->get_CurrentCanRotate(&transform->canRotate);

                ig.transform = transform;
            });

            doIfPatternExists<UIA_ValuePatternId, IUIAutomationValuePattern>(pElementAtMouse, [&](IUIAutomationValuePattern *pattern) {
                Value *value = new Value();

                pattern->get_CurrentIsReadOnly(&value->isReadOnly);
                pattern->get_CurrentValue(&value->value);

                ig.value = value;
            });

            doIfPatternExists<UIA_WindowPatternId, IUIAutomationWindowPattern>(pElementAtMouse, [&](IUIAutomationWindowPattern *pattern) {
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

                ig.window = window;
            });

            pElementAtMouse->Release();
        }

        releaseAutomation(pAutomation);

        return ig;
    }

    void releaseInfoGroup(InfoGroup &ig)
    {
        auto clearTarget = [](TargetElement &target) {
            // Clear target name string
            if (target.name)
            {
                SysFreeString(target.name);

                target.name = nullptr;
            }

            // Clear target controle type string
            if (target.controleType)
            {
                SysFreeString(target.controleType);
            }
        };

        // Clear name string
        if (ig.name)
        {
            SysFreeString(ig.name);

            ig.name = nullptr;
        }

        // Clear controle type string
        if (ig.controleType)
        {
            SysFreeString(ig.controleType);

            ig.controleType = nullptr;
        }

        // Clear annotation
        if (ig.annotation)
        {
            // Clear annotation name string
            if (ig.annotation->name)
            {
                SysFreeString(ig.annotation->name);

                ig.annotation->name = nullptr;
            }

            // Clear annotation author string
            if (ig.annotation->author)
            {
                SysFreeString(ig.annotation->author);

                ig.annotation->author = nullptr;
            }

            // Clear annotation date time string
            if (ig.annotation->dateTime)
            {
                SysFreeString(ig.annotation->dateTime);

                ig.annotation->dateTime = nullptr;
            }

            // Clear annotation target
            clearTarget(ig.annotation->target);

            delete ig.annotation;

            ig.annotation = nullptr;
        }

        // Clear dock
        if (ig.dock)
        {
            delete ig.dock;

            ig.dock = nullptr;
        }

        // Clear expandCollapse
        if (ig.expandCollapse)
        {
            delete ig.expandCollapse;

            ig.expandCollapse = nullptr;
        }

        // Clear legacy accessibility
        if (ig.legacyAccessibility)
        {
            // Clear legacy accessibility action string
            if (ig.legacyAccessibility->action)
            {
                SysFreeString(ig.legacyAccessibility->action);

                ig.legacyAccessibility->action = nullptr;
            }

            // Clear legacy accessibility description string
            if (ig.legacyAccessibility->description)
            {
                SysFreeString(ig.legacyAccessibility->description);

                ig.legacyAccessibility->description = nullptr;
            }

            // Clear legacy accessibility name string
            if (ig.legacyAccessibility->name)
            {
                SysFreeString(ig.legacyAccessibility->name);

                ig.legacyAccessibility->name = nullptr;
            }

            // Clear legacy accessibility value string
            if (ig.legacyAccessibility->value)
            {
                SysFreeString(ig.legacyAccessibility->value);

                ig.legacyAccessibility->value = nullptr;
            }

            delete ig.legacyAccessibility;

            ig.legacyAccessibility = nullptr;
        }

        // Clear range value
        if (ig.rangeValue)
        {
            delete ig.rangeValue;

            ig.rangeValue = nullptr;
        }

        // Clear scroll
        if (ig.scroll)
        {
            delete ig.scroll;

            ig.scroll = nullptr;
        }

        // Clear selection
        if (ig.selection)
        {
            // Clear all selection targets
            for (int i = 0; i < ig.selection->targets.length; i++)
                clearTarget(ig.selection->targets.elements[i]);

            delete[] ig.selection->targets.elements;
            delete ig.selection;

            ig.selection = nullptr;
        }

        // Clear selection item
        if (ig.selectionItem)
        {
            delete ig.selectionItem;

            ig.selectionItem = nullptr;
        }

        // Clear style
        if (ig.style)
        {
            // Clear style name string
            if (ig.style->name)
            {
                SysFreeString(ig.style->name);

                ig.style->name = nullptr;
            }

            // Clear style extendedProperties string
            if (ig.style->extendedProperties)
            {
                SysFreeString(ig.style->extendedProperties);

                ig.style->extendedProperties = nullptr;
            }

            // Clear style shape string
            if (ig.style->shape)
            {
                SysFreeString(ig.style->shape);

                ig.style->shape = nullptr;
            }

            delete ig.style;

            ig.style = nullptr;
        }

        // Clear text
        if (ig.text)
        {
            if (ig.text->text)
            {
                SysFreeString(ig.text->text);

                ig.text->text = nullptr;
            }

            delete ig.text;

            ig.text = nullptr;
        }

        // Clear toggle
        if (ig.toggle)
        {
            delete ig.toggle;

            ig.toggle = nullptr;
        }

        // Clear transform
        if (ig.transform)
        {
            delete ig.transform;

            ig.transform = nullptr;
        }

        // Clear value
        if (ig.value)
        {
            if (ig.value->value)
            {
                SysFreeString(ig.value->value);

                ig.value->value = nullptr;
            }

            delete ig.value;

            ig.value = nullptr;
        }

        // Clear window
        if (ig.window)
        {
            delete ig.window;

            ig.window = nullptr;
        }
    }
} // namespace IW
