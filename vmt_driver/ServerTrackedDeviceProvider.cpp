/*
MIT License

Copyright (c) 2020 gpsnmeajp

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "ServerTrackedDeviceProvider.h"
namespace VMTDriver {
    //�f�o�C�X���Ǘ�����e
    //���̉��Ɏq�Ƃ��ăf�o�C�X���Ԃ牺����

    vector<TrackedDeviceServerDriver>& ServerTrackedDeviceProvider::GetDevices()
    {
        return m_devices;
    }

    //������
    EVRInitError ServerTrackedDeviceProvider::Init(IVRDriverContext* pDriverContext)
    {
        VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext)
        Log::Open(VRDriverLog());
        CommunicationManager::GetInstance()->Open(this);
        Log::Output("HelloWorld");

        //16�f�o�C�X������
        m_devices.resize(16);

        //16�f�o�C�X��o�^
        for (int i = 0; i < m_devices.size(); i++)
        {
            string name = "VMT_";
            name.append(std::to_string(i));

            m_devices[i].SetDeviceSerial(name);
            m_devices[i].SetObjectIndex(i);
            //m_devices[i].RegisterToVRSystem(); //�o�^�͕K�v�ɂȂ�������
            m_devicesNum++;
        }

        return EVRInitError::VRInitError_None;
    }

    //�I��
    void ServerTrackedDeviceProvider::Cleanup()
    {
        VR_CLEANUP_SERVER_DRIVER_CONTEXT()
        CommunicationManager::GetInstance()->Close();
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
        //�ʐM����������
        CommunicationManager::GetInstance()->Process();
        for (int i = 0; i < m_devicesNum; i++)
        {
            m_devices[i].UpdatePoseToVRSystem();
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