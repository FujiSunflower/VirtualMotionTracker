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
#include "CommunicationManager.h"
namespace VMTDriver {
	//����M�X���b�h
	std::mutex CommunicationWorkerMutex;
	std::deque<string> CommunicationWorkerReadQue;
	std::deque<string> CommunicationWorkerWriteQue;
	bool CommunicationWorkerExit = false;
	std::thread* CommunicationWorkerThread;
	void CommunicationWorker()
	{
		while (!CommunicationWorkerExit)
		{
			//�N���e�B�J���Z�N�V����
			{
				std::lock_guard<std::mutex> lock(CommunicationWorkerMutex);

				//��M�f�[�^����������Ђ�����ǂݍ���
				string r = CommunicationManager::GetInstance()->GetSM()->readM2D();
				//��M�f�[�^���L��ꍇ(�����~�b�^�[�ȉ��̏ꍇ)
				if (r != "" && CommunicationWorkerReadQue.size() < 1024) {
					CommunicationWorkerReadQue.push_back(r);
					//��ɂȂ�܂ő҂����ԍŒZ�œǂݍ���
					continue;
				}
				//���M�f�[�^����������Ђ����珑������
				if (!CommunicationWorkerWriteQue.empty())
				{
					if (CommunicationManager::GetInstance()->GetSM()->writeD2M(CommunicationWorkerWriteQue.front()) == true)
					{
						//������������������đ�����
						CommunicationWorkerWriteQue.pop_front();
						continue;
					}
					//�������ݎ��s���͎��̎����܂ő҂�(���肪�N�����Ă��Ȃ����A���肪�����ς������ς�)
				}
			}
			//�m���N���e�B�J���Z�N�V����
			Sleep(4); //240fps
		}
	}

	string CommunicationRead() {
		string r = "";
		//�N���e�B�J���Z�N�V����
		{
			std::lock_guard<std::mutex> lock(CommunicationWorkerMutex);
			if (!CommunicationWorkerReadQue.empty())
			{
				//��M�L���[������o��
				r = CommunicationWorkerReadQue.front();
				CommunicationWorkerReadQue.pop_front();
			}
		}
		return r;
	}
	void CommunicationWrite(string s) {
		std::lock_guard<std::mutex> lock(CommunicationWorkerMutex);
		if (CommunicationWorkerWriteQue.size() < 1024) { //1024���ȏ�͎̂Ă�(�ُ펞�A�ʐM�s�ǎ�)
			CommunicationWorkerWriteQue.push_back(s);
		}
	}
}

namespace VMTDriver {
	CommunicationManager* CommunicationManager::GetInstance()
	{
		static CommunicationManager cm;
		return &cm;
	}
	SharedMemory::SharedMemory* CommunicationManager::GetSM()
	{
		return m_sm;
	}
	void CommunicationManager::Open()
	{
		if (m_opened) {
			return;
		}
		m_sm = new SharedMemory::SharedMemory();
		if (!m_sm->open()) {
			//�I�[�v���Ɏ��s
			delete m_sm;
			m_sm = nullptr;
			return;
		}
		
		CommunicationWorkerThread = new std::thread(CommunicationWorker);
		m_opened = true;
	}
	void CommunicationManager::Close()
	{
		CommunicationWorkerExit = true;
		CommunicationWorkerThread->join();
		m_sm->close();
		delete m_sm;
		m_sm = nullptr;
		m_opened = false;
	}
	void CommunicationManager::Process(ServerTrackedDeviceProvider* server)
	{
		//�ʐM�̏������ł��Ă��Ȃ�
		if (m_sm == nullptr) {
			return;
		}
		try {
			//��M(�o�b�t�@�����܂��Ă���ꍇ�����ɏ�������)
			do {
				string r = CommunicationRead();
				if (r.empty()) {
					break;
				}
				//Log::printf("CommunicationWorkerReadQue:%d", CommunicationWorkerReadQue.size());

				json j = json::parse(r);
				string type = j["type"];
				if (type == "Pos") {
					string j2s = j["json"];
					json j2 = json::parse(j2s);


					DriverPose_t pose{ 0 };

					pose.qRotation = VMTDriver::HmdQuaternion_Identity;
					pose.qWorldFromDriverRotation = VMTDriver::HmdQuaternion_Identity;
					pose.qDriverFromHeadRotation = VMTDriver::HmdQuaternion_Identity;

					int idx = j2["idx"];
					if (j2["en"]) {
						pose.deviceIsConnected = true;
						pose.poseIsValid = true;
						pose.result = TrackingResult_Running_OK;
					}
					else {
						pose.deviceIsConnected = false;
						pose.poseIsValid = false;
						pose.result = ETrackingResult::TrackingResult_Calibrating_OutOfRange;
					}
					pose.vecPosition[0] = j2["x"];
					pose.vecPosition[1] = j2["y"];
					pose.vecPosition[2] = j2["z"];
					pose.qRotation.x = j2["qx"];
					pose.qRotation.y = j2["qy"];
					pose.qRotation.z = j2["qz"];
					pose.qRotation.w = j2["qw"];

					if (idx >= 0 && idx <= server->GetDevices().size())
					{
						server->GetDevices()[idx].RegisterToVRSystem();
						server->GetDevices()[idx].SetPose(pose);
					}
				}
				printf("%s\n", type.c_str());
			} while (true);

			//���M
			/*
			json jw;
			jw["type"] = "Hello";
			jw["json"] = json{ {"msg","Hello from cpp"} }.dump();
			CommunicationWrite(jw.dump());
			*/
		}
		catch (...) {
			m_sm->logError("Exception in CommunicationManager::Process");
		}
	}
}