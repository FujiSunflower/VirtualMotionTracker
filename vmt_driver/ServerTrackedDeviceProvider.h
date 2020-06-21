#pragma once
#include "dllmain.h"

using namespace vr;
namespace VMTDriver {
    //�f�o�C�X�T�[�o�[
    class ServerTrackedDeviceProvider : IServerTrackedDeviceProvider
    {
    public:
        virtual EVRInitError Init(IVRDriverContext* pDriverContext) override;
        virtual void Cleanup() override;
        virtual const char* const* GetInterfaceVersions() override;
        virtual void RunFrame() override;
        virtual bool ShouldBlockStandbyMode() override;
        virtual void EnterStandby() override;
        virtual void LeaveStandby() override;
    };
}