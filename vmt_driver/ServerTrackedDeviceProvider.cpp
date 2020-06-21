#include "ServerTrackedDeviceProvider.h"
namespace VMTDriver {
    //�f�o�C�X���Ǘ�����e
    //���̉��Ɏq�Ƃ��ăf�o�C�X���Ԃ牺����

    //������
    EVRInitError ServerTrackedDeviceProvider::Init(IVRDriverContext* pDriverContext)
    {
        VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext)
        Log::Open(VRDriverLog());
        Log::Output("HelloWorld");
        //VRServerDriverHost()->TrackedDeviceAdded(serial, ETrackedDeviceClass::TrackedDeviceClass_GenericTracker, deriver);

        return EVRInitError::VRInitError_None;
    }

    //�I��
    void ServerTrackedDeviceProvider::Cleanup()
    {
        VR_CLEANUP_SERVER_DRIVER_CONTEXT()
        Log::Close();
    }

    //�C���^�[�t�F�[�X�o�[�W����(���C�u�����Ɉˑ�)
    const char* const* ServerTrackedDeviceProvider::GetInterfaceVersions()
    {
        return k_InterfaceVersions;
    }

    //���t���[������
    void ServerTrackedDeviceProvider::RunFrame()
    {
    }

    //�X�^���o�C���u���b�N���邩
    bool ServerTrackedDeviceProvider::ShouldBlockStandbyMode()
    {
        return false;
    }

    //�X�^���o�C�ɓ�����
    void ServerTrackedDeviceProvider::EnterStandby()
    {
    }

    //�X�^���o�C����o��
    void ServerTrackedDeviceProvider::LeaveStandby()
    {
    }
}