#pragma once
#include "dllmain.h"

using namespace vr;
namespace VMTDriver {
    //�󋵂Ɋ֌W�Ȃ������f�o�C�X�Ȃǂ��Ď����邽�߂�WatchDog
    class VRWatchdogProvider : public IVRWatchdogProvider
    {
    public:
        // IVRWatchdogProvider ����Čp������܂���
        virtual EVRInitError Init(IVRDriverContext* pDriverContext) override;
        virtual void Cleanup() override;
    };
}