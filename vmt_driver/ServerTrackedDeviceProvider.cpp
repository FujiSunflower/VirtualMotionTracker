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

        //16�f�o�C�X������
        m_devices.resize(16);

        //16�f�o�C�X��o�^
        for (int i = 0; i < m_devices.size(); i++)
        {
            m_devices[i].m_serial = "Hello Device";
            m_devices[i].m_serial.append(std::to_string(i));
            m_devices[i].index = i;
            VRServerDriverHost()->TrackedDeviceAdded(m_devices[i].m_serial.c_str(), ETrackedDeviceClass::TrackedDeviceClass_GenericTracker, &m_devices[i]);
        }

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
        for (int i = 0; i < m_devices.size(); i++)
        {
            VRServerDriverHost()->TrackedDevicePoseUpdated(m_devices[i].m_deviceIndex, m_devices[i].GetPose(), sizeof(DriverPose_t));
        }
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