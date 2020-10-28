#include <node.h>

#include "InfoGroup.h"

#define V8_EXCEPTION(msg) v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, msg))

namespace IW
{
    v8::Local<v8::Object> packTargetInfo(v8::Isolate *isolate, TargetInfo &ti);
    v8::Local<v8::Array> packTargetInfoList(v8::Isolate *isolate, TargetInfoList target);

    v8::Local<v8::Object> packBoundingRect(v8::Isolate *isolate, RECT boundingRect)
    {
        v8::Local<v8::Object> packedBoundingRect = v8::Object::New(isolate);

        packedBoundingRect->Set(v8::String::NewFromUtf8(isolate, "top"), v8::Number::New(isolate, boundingRect.top));
        packedBoundingRect->Set(v8::String::NewFromUtf8(isolate, "left"), v8::Number::New(isolate, boundingRect.left));
        packedBoundingRect->Set(v8::String::NewFromUtf8(isolate, "bottom"), v8::Number::New(isolate, boundingRect.bottom));
        packedBoundingRect->Set(v8::String::NewFromUtf8(isolate, "right"), v8::Number::New(isolate, boundingRect.right));
        packedBoundingRect->Set(v8::String::NewFromUtf8(isolate, "width"), v8::Number::New(isolate, boundingRect.right - boundingRect.left));
        packedBoundingRect->Set(v8::String::NewFromUtf8(isolate, "hegith"), v8::Number::New(isolate, boundingRect.right - boundingRect.top));

        return packedBoundingRect;
    }

    v8::Local<v8::Object> packAnnotaion(v8::Isolate *isolate, Annotation *annotation)
    {
        v8::Local<v8::Object> packedAnnotation = v8::Object::New(isolate);

        packedAnnotation->Set(v8::String::NewFromUtf8(isolate, "name"), v8::String::NewFromTwoByte(isolate, (uint16_t *)annotation->name));
        packedAnnotation->Set(v8::String::NewFromUtf8(isolate, "author"), v8::String::NewFromTwoByte(isolate, (uint16_t *)annotation->author));
        packedAnnotation->Set(v8::String::NewFromUtf8(isolate, "dateTime"), v8::String::NewFromTwoByte(isolate, (uint16_t *)annotation->dateTime));
        packedAnnotation->Set(v8::String::NewFromUtf8(isolate, "target"), packTargetInfo(isolate, annotation->target));

        return packedAnnotation;
    }

    v8::Local<v8::Object> packDock(v8::Isolate *isolate, Dock *dock)
    {
        v8::Local<v8::Object> packedDock = v8::Object::New(isolate);

        packedDock->Set(v8::String::NewFromUtf8(isolate, "position"), v8::String::NewFromTwoByte(isolate, (uint16_t *)dock->position));

        return packedDock;
    }

    v8::Local<v8::Object> packExpandCollapse(v8::Isolate *isolate, ExpandCollapse *expandCollapse)
    {
        v8::Local<v8::Object> packedExpandCollapse = v8::Object::New(isolate);

        packedExpandCollapse->Set(v8::String::NewFromUtf8(isolate, "position"), v8::String::NewFromTwoByte(isolate, (uint16_t *)expandCollapse->state));

        return packedExpandCollapse;
    }

    v8::Local<v8::Object> packLegacyAccessibility(v8::Isolate *isolate, LegacyAccessibility *legacyAccessibility)
    {
        v8::Local<v8::Object> packedLegacyAccessibility = v8::Object::New(isolate);

        packedLegacyAccessibility->Set(v8::String::NewFromUtf8(isolate, "action"), v8::String::NewFromTwoByte(isolate, (uint16_t *)legacyAccessibility->action));
        packedLegacyAccessibility->Set(v8::String::NewFromUtf8(isolate, "description"), v8::String::NewFromTwoByte(isolate, (uint16_t *)legacyAccessibility->description));
        packedLegacyAccessibility->Set(v8::String::NewFromUtf8(isolate, "name"), v8::String::NewFromTwoByte(isolate, (uint16_t *)legacyAccessibility->name));
        packedLegacyAccessibility->Set(v8::String::NewFromUtf8(isolate, "value"), v8::String::NewFromTwoByte(isolate, (uint16_t *)legacyAccessibility->value));

        return packedLegacyAccessibility;
    }

    v8::Local<v8::Object> packRangeValue(v8::Isolate *isolate, RangeValue *rangeValue)
    {
        v8::Local<v8::Object> packedRangeValue = v8::Object::New(isolate);

        packedRangeValue->Set(v8::String::NewFromUtf8(isolate, "isReadOnly"), v8::Boolean::New(isolate, rangeValue->isReadOnly));
        packedRangeValue->Set(v8::String::NewFromUtf8(isolate, "bigChange"), v8::Number::New(isolate, rangeValue->bigChange));
        packedRangeValue->Set(v8::String::NewFromUtf8(isolate, "smallChange"), v8::Number::New(isolate, rangeValue->smallChange));
        packedRangeValue->Set(v8::String::NewFromUtf8(isolate, "currentValue"), v8::Number::New(isolate, rangeValue->currentValue));
        packedRangeValue->Set(v8::String::NewFromUtf8(isolate, "maximumValue"), v8::Number::New(isolate, rangeValue->maximumValue));
        packedRangeValue->Set(v8::String::NewFromUtf8(isolate, "minimumValue"), v8::Number::New(isolate, rangeValue->minimumValue));

        return packedRangeValue;
    }

    v8::Local<v8::Object> packScrollData(v8::Isolate *isolate, ScrollData scrollData)
    {
        v8::Local<v8::Object> packedScrollData = v8::Object::New(isolate);

        packedScrollData->Set(v8::String::NewFromUtf8(isolate, "isScrollable"), v8::Boolean::New(isolate, scrollData.isScrollable));
        packedScrollData->Set(v8::String::NewFromUtf8(isolate, "scrollPercent"), v8::Number::New(isolate, scrollData.scrollPercent));
        packedScrollData->Set(v8::String::NewFromUtf8(isolate, "viewSize"), v8::Number::New(isolate, scrollData.viewSize));

        return packedScrollData;
    }

    v8::Local<v8::Object> packscroll(v8::Isolate *isolate, Scroll *scroll)
    {
        v8::Local<v8::Object> packedScroll = v8::Object::New(isolate);

        packedScroll->Set(v8::String::NewFromUtf8(isolate, "horizontalScroll"), packScrollData(isolate, scroll->horizontalScroll));
        packedScroll->Set(v8::String::NewFromUtf8(isolate, "verticallScroll"), packScrollData(isolate, scroll->verticallScroll));

        return packedScroll;
    }

    v8::Local<v8::Object> packSelection(v8::Isolate *isolate, Selection *selection)
    {
        v8::Local<v8::Object> packedSelection = v8::Object::New(isolate);

        packedSelection->Set(v8::String::NewFromUtf8(isolate, "isRequired"), v8::Boolean::New(isolate, selection->isRequired));
        packedSelection->Set(v8::String::NewFromUtf8(isolate, "isMultipleSelectionAllowed"), v8::Boolean::New(isolate, selection->isMultipleSelectionAllowed));
        packedSelection->Set(v8::String::NewFromUtf8(isolate, "targets"), packTargetInfoList(isolate, selection->targets));

        return packedSelection;
    }

    v8::Local<v8::Object> packSelectionItem(v8::Isolate *isolate, SelectionItem *selectionItem)
    {
        v8::Local<v8::Object> packedSelectionItem = v8::Object::New(isolate);

        packedSelectionItem->Set(v8::String::NewFromUtf8(isolate, "isSelected"), v8::Boolean::New(isolate, selectionItem->isSelected));

        return packedSelectionItem;
    }

    v8::Local<v8::Object> packStyle(v8::Isolate *isolate, Style *style)
    {
        v8::Local<v8::Object> packedStyle = v8::Object::New(isolate);

        packedStyle->Set(v8::String::NewFromUtf8(isolate, "name"), v8::String::NewFromTwoByte(isolate, (uint16_t *)style->name));
        packedStyle->Set(v8::String::NewFromUtf8(isolate, "extendedProperties"), v8::String::NewFromTwoByte(isolate, (uint16_t *)style->extendedProperties));
        packedStyle->Set(v8::String::NewFromUtf8(isolate, "shape"), v8::String::NewFromTwoByte(isolate, (uint16_t *)style->shape));
        packedStyle->Set(v8::String::NewFromUtf8(isolate, "fillColor"), v8::Number::New(isolate, style->fillColor));
        packedStyle->Set(v8::String::NewFromUtf8(isolate, "fillPatternColor"), v8::Number::New(isolate, style->fillPatternColor));
        packedStyle->Set(v8::String::NewFromUtf8(isolate, "styleId"), v8::Number::New(isolate, style->styleId));

        return packedStyle;
    }

    v8::Local<v8::Object> packText(v8::Isolate *isolate, Text *text)
    {
        v8::Local<v8::Object> packedText = v8::Object::New(isolate);

        packedText->Set(v8::String::NewFromUtf8(isolate, "text"), v8::String::NewFromTwoByte(isolate, (uint16_t *)text->text));

        return packedText;
    }

    v8::Local<v8::Object> packToggle(v8::Isolate *isolate, Toggle *toggle)
    {
        v8::Local<v8::Object> packedToggle = v8::Object::New(isolate);

        packedToggle->Set(v8::String::NewFromUtf8(isolate, "state"), v8::String::NewFromTwoByte(isolate, (uint16_t *)toggle->state));

        return packedToggle;
    }

    v8::Local<v8::Object> packTransform(v8::Isolate *isolate, Transform *transform)
    {
        v8::Local<v8::Object> packedTransform = v8::Object::New(isolate);

        packedTransform->Set(v8::String::NewFromUtf8(isolate, "canMove"), v8::Boolean::New(isolate, transform->canMove));
        packedTransform->Set(v8::String::NewFromUtf8(isolate, "canResize"), v8::Boolean::New(isolate, transform->canResize));
        packedTransform->Set(v8::String::NewFromUtf8(isolate, "canRotate"), v8::Boolean::New(isolate, transform->canRotate));

        return packedTransform;
    }

    v8::Local<v8::Object> packValue(v8::Isolate *isolate, Value *value)
    {
        v8::Local<v8::Object> packedValue = v8::Object::New(isolate);

        packedValue->Set(v8::String::NewFromUtf8(isolate, "isReadOnly"), v8::Boolean::New(isolate, value->isReadOnly));
        packedValue->Set(v8::String::NewFromUtf8(isolate, "value"), v8::String::NewFromTwoByte(isolate, (uint16_t *)value->value));

        return packedValue;
    }

    v8::Local<v8::Object> packWindow(v8::Isolate *isolate, Window *window)
    {
        v8::Local<v8::Object> packedWindow = v8::Object::New(isolate);

        packedWindow->Set(v8::String::NewFromUtf8(isolate, "canMaximize"), v8::Boolean::New(isolate, window->canMaximize));
        packedWindow->Set(v8::String::NewFromUtf8(isolate, "canMinimize"), v8::Boolean::New(isolate, window->canMinimize));
        packedWindow->Set(v8::String::NewFromUtf8(isolate, "isModal"), v8::Boolean::New(isolate, window->isModal));
        packedWindow->Set(v8::String::NewFromUtf8(isolate, "isTopMost"), v8::Boolean::New(isolate, window->isTopMost));
        packedWindow->Set(v8::String::NewFromUtf8(isolate, "interactionState"), v8::String::NewFromTwoByte(isolate, (uint16_t *)window->interactionState));
        packedWindow->Set(v8::String::NewFromUtf8(isolate, "visualState"), v8::String::NewFromTwoByte(isolate, (uint16_t *)window->visualState));

        return packedWindow;
    }

    v8::Local<v8::Object> packTargetInfo(v8::Isolate *isolate, TargetInfo &ti)
    {
        v8::Local<v8::Object> packedTargetInfo = v8::Object::New(isolate);

        packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "name"), v8::String::NewFromTwoByte(isolate, (uint16_t *)ti.name));
        packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "controleType"), v8::String::NewFromTwoByte(isolate, (uint16_t *)ti.controleType));
        packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "boundingRect"), packBoundingRect(isolate, ti.boundingRectangle));
        packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "isContent"), v8::Boolean::New(isolate, ti.isContent));
        packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "isControl"), v8::Boolean::New(isolate, ti.isControle));

        if (ti.annotation)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "annotation"), packAnnotaion(isolate, ti.annotation));
        if (ti.dock)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "dock"), packDock(isolate, ti.dock));
        if (ti.expandCollapse)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "expandCollapse"), packExpandCollapse(isolate, ti.expandCollapse));
        if (ti.legacyAccessibility)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "legacyAccessibility"), packLegacyAccessibility(isolate, ti.legacyAccessibility));
        if (ti.rangeValue)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "rangeValue"), packRangeValue(isolate, ti.rangeValue));
        if (ti.scroll)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "scroll"), packscroll(isolate, ti.scroll));
        if (ti.selection)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "selection"), packSelection(isolate, ti.selection));
        if (ti.selectionItem)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "selectionItem"), packSelectionItem(isolate, ti.selectionItem));
        if (ti.style)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "style"), packStyle(isolate, ti.style));
        if (ti.text)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "text"), packText(isolate, ti.text));
        if (ti.toggle)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "toggle"), packToggle(isolate, ti.toggle));
        if (ti.transform)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "transform"), packTransform(isolate, ti.transform));
        if (ti.value)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "value"), packValue(isolate, ti.value));
        if (ti.window)
            packedTargetInfo->Set(v8::String::NewFromUtf8(isolate, "window"), packWindow(isolate, ti.window));

        return packedTargetInfo;
    }

    v8::Local<v8::Array> packTargetInfoList(v8::Isolate *isolate, TargetInfoList target)
    {
        v8::Local<v8::Array> packedTargetElementList = v8::Array::New(isolate, target.length);

        for (uint32_t i = 0; i < target.length; i++)
            packedTargetElementList->Set(i, packTargetInfo(isolate, target.elements[i]));

        return packedTargetElementList;
    }

    void getInfoFromPoint(const v8::FunctionCallbackInfo<v8::Value> &args)
    {
        v8::Isolate *isolate = args.GetIsolate();

        if (args.Length() != 2)
        {
            isolate->ThrowException(V8_EXCEPTION("Invalid number of arguments. Screen coordinates x and y are required"));
            return;
        }

        if (!args[0]->IsNumber() || !args[1]->IsNumber())
        {
            isolate->ThrowException(V8_EXCEPTION("Invalid arguments type. Both must be of type Number"));
            return;
        }

        TargetInfo it = getTargetInfoFromPoint((long)args[0].As<v8::Number>()->Value(), (long)args[1].As<v8::Number>()->Value());

        args.GetReturnValue().Set(packTargetInfo(isolate, it));

        releaseTargetInfo(it);
    }

    void initialize(v8::Local<v8::Object> exports)
    {
        NODE_SET_METHOD(exports, "getInfoFromPoint", getInfoFromPoint);
    }

    NODE_MODULE(inspectorWindows, initialize)
} // namespace IW
