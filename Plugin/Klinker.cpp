#include "Common.h"
#include "Receiver.h"
#include "Unity/IUnityRenderingExtensions.h"

namespace klinker
{
    // Callback for texture update events
    void TextureUpdateCallback(int eventID, void* data)
    {
        auto event = static_cast<UnityRenderingExtEventType>(eventID);
        if (event == kUnityRenderingExtEventUpdateTextureBeginV2)
        {
            // UpdateTextureBegin
            auto params = reinterpret_cast<UnityRenderingExtTextureUpdateParamsV2*>(data);
            auto receiver = Receiver::GetInstanceFromID(params->userData);
            params->texData = receiver->LockBuffer();
        }
        else if (event == kUnityRenderingExtEventUpdateTextureEndV2)
        {
            // UpdateTextureEnd
            auto params = reinterpret_cast<UnityRenderingExtTextureUpdateParamsV2*>(data);
            auto receiver = Receiver::GetInstanceFromID(params->userData);
            receiver->UnlockBuffer();
        }
    }
}

extern "C" void UNITY_INTERFACE_EXPORT *CreateReceiver()
{
    auto receiver = new klinker::Receiver();
    receiver->StartReceiving();
    return receiver;
}

extern "C" void UNITY_INTERFACE_EXPORT DestroyReceiver(void* receiverPointer)
{
    auto receiver = reinterpret_cast<klinker::Receiver*>(receiverPointer);
    receiver->StopReceiving();
    receiver->Release();
}

extern "C" unsigned int UNITY_INTERFACE_EXPORT GetReceiverID(void* receiver)
{
    return reinterpret_cast<klinker::Receiver*>(receiver)->GetID();
}

extern "C" int UNITY_INTERFACE_EXPORT GetReceiverFrameWidth(void* receiver)
{
    return reinterpret_cast<klinker::Receiver*>(receiver)->GetFrameWidth();
}

extern "C" int UNITY_INTERFACE_EXPORT GetReceiverFrameHeight(void* receiver)
{
    return reinterpret_cast<klinker::Receiver*>(receiver)->GetFrameHeight();
}

extern "C" UnityRenderingEventAndData UNITY_INTERFACE_EXPORT GetTextureUpdateCallback()
{
    return klinker::TextureUpdateCallback;
}
