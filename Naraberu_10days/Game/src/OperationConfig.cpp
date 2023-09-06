#include "OperationConfig.h"
#include"FrameWork/UsersInput.h"
#include"FrameWork/WinApp.h"

using namespace KuroEngine;

OperationConfig::OperationConfig()
	:Debugger("OperationConfig", true)
{
	//�e�f�o�C�X�ł̐ݒ�l���J�X�^���p�����[�^�ɒǉ�
	//for (int i = 0; i < INPUT_DEVICE::NUM; ++i)
	//{
	//	auto deviceName = m_inputDeviceNames[i];
	//	AddCustomParameter("CameraSensitivity", { deviceName,"CameraSensitivity" },
	//		PARAM_TYPE::FLOAT, &m_params[i].m_camSensitivity, deviceName);
	//}

	//LoadParameterLog();

	//����L�[���蓖��
	m_operationKeyCode =
	{
		DIK_SPACE,	//����
		DIK_ESCAPE,	//�L�����Z��
		DIK_SPACE,	//�v���Y�����p�l���ɐݒu
		DIK_L,	//���̃v���Y����I��
		DIK_RIGHT,	//�E�̃v���Y����I��
		DIK_RETURN,	//�^�[�����I����
	};

	//����{�^�����蓖��
	m_operationButton =
	{
		A,	//����
		B,	//�L�����Z��
		B,	//�v���Y�����p�l���ɐݒu
		LB,	//���̃v���Y����I��
		RB,	//�E�̃v���Y����I��
		START,	//�^�[�����I����
	};
}

void OperationConfig::OnImguiItems()
{
	using namespace KuroEngine;

	//�Ō�̓��͂̃f�o�C�X
	ImGui::Text("LatestDevice : %s", m_inputDeviceNames[m_nowInputDevice].c_str());

	//�}�E�X���͕\��
	auto mouseMove = UsersInput::Instance()->GetMouseMove();
	ImGui::BeginChild(ImGui::GetID((void *)0), ImVec2(250, 150));
	ImGui::Text("inputX : %d", mouseMove.m_inputX);
	ImGui::Text("inputY : %d", mouseMove.m_inputY);
	ImGui::Text("inputZ : %d", mouseMove.m_inputZ);
	ImGui::EndChild();

}

bool OperationConfig::ControllerInput(INPUT_PATTERN arg_pattern, KuroEngine::XBOX_BUTTON arg_xboxButton)
{
	using namespace KuroEngine;
	switch (arg_pattern)
	{
		case HOLD:
			return UsersInput::Instance()->ControllerInput(0, arg_xboxButton);
			break;
		case ON_TRIGGER:
			return UsersInput::Instance()->ControllerOnTrigger(0, arg_xboxButton);
			break;
		case OFF_TRIGGER:
			return UsersInput::Instance()->ControllerOffTrigger(0, arg_xboxButton);
			break;
		case ON_OFF_TRIGGER:
			return UsersInput::Instance()->ControllerOnTrigger(0, arg_xboxButton) || UsersInput::Instance()->ControllerOffTrigger(0, arg_xboxButton);
			break;
		default:
			break;
	}
	return false;
}

bool OperationConfig::KeyInput(INPUT_PATTERN arg_pattern, int arg_keyCode)
{
	using namespace KuroEngine;
	switch (arg_pattern)
	{
		case HOLD:
			return UsersInput::Instance()->KeyInput(arg_keyCode);
			break;
		case ON_TRIGGER:
			return UsersInput::Instance()->KeyOnTrigger(arg_keyCode);
			break;
		case OFF_TRIGGER:
			return UsersInput::Instance()->KeyOffTrigger(arg_keyCode);
			break;
		case ON_OFF_TRIGGER:
			return UsersInput::Instance()->KeyOnTrigger(arg_keyCode) || UsersInput::Instance()->KeyOffTrigger(arg_keyCode);
			break;
		default:
			break;
	}
	return false;
}

bool OperationConfig::GetSelectVec(SELECT_VEC arg_vec, INPUT_DEVICE* arg_device)
{
	static const float STICK_DEAD_RANGE = 0.15f;
	bool con = false;
	bool key = false;

	switch (arg_vec)
	{
		case SELECT_VEC_UP:
			con = UsersInput::Instance()->ControllerOnTrigger(0, XBOX_STICK::L_UP, STICK_DEAD_RANGE)
				|| UsersInput::Instance()->ControllerOnTrigger(0, XBOX_BUTTON::DPAD_UP);
			key = UsersInput::Instance()->KeyOnTrigger(DIK_W);
			break;
		case SELECT_VEC_DOWN:
			con = UsersInput::Instance()->ControllerOnTrigger(0, XBOX_STICK::L_DOWN, STICK_DEAD_RANGE)
				|| UsersInput::Instance()->ControllerOnTrigger(0, XBOX_BUTTON::DPAD_DOWN);
			key = UsersInput::Instance()->KeyOnTrigger(DIK_S);
			break;
		case SELECT_VEC_LEFT:
			con = UsersInput::Instance()->ControllerOnTrigger(0, XBOX_STICK::L_LEFT, STICK_DEAD_RANGE)
				|| UsersInput::Instance()->ControllerOnTrigger(0, XBOX_BUTTON::DPAD_LEFT);
			key = UsersInput::Instance()->KeyOnTrigger(DIK_A);
			break;
		case SELECT_VEC_RIGHT:
			con = UsersInput::Instance()->ControllerOnTrigger(0, XBOX_STICK::L_RIGHT, STICK_DEAD_RANGE)
				|| UsersInput::Instance()->ControllerOnTrigger(0, XBOX_BUTTON::DPAD_RIGHT);
			key = UsersInput::Instance()->KeyOnTrigger(DIK_D);
			break;
		default:
			break;
	}

	if (con)
	{
		RegisterLatestDevice(INPUT_DEVICE::CONTROLLER);
		if (arg_device)*arg_device = INPUT_DEVICE::CONTROLLER;
	}
	else if (key)
	{
		RegisterLatestDevice(INPUT_DEVICE::KEY_BOARD_MOUSE);
		if (arg_device)*arg_device = INPUT_DEVICE::KEY_BOARD_MOUSE;
	}

	return con || key;
}

bool OperationConfig::GetMoveVec(SELECT_VEC arg_vec)
{
	//�g���K�[����
	INPUT_DEVICE device;
	bool trigger = GetSelectVec(arg_vec, &device);
	if (trigger)
	{
		//�������t���[���̃��Z�b�g
		m_moveInputFrame[device][arg_vec] = 0;
		return true;
	}

	//�������œ��͔���ɂ���t���[��
	static const int INPUT_FRAME_MAX = 8;

	bool con = false;
	bool key = false;

	//����������
	switch (arg_vec)
	{
		case SELECT_VEC_UP:
			con = UsersInput::Instance()->ControllerInput(0, XBOX_STICK::L_UP)
				|| UsersInput::Instance()->ControllerInput(0, XBOX_BUTTON::DPAD_UP);
			key = UsersInput::Instance()->KeyInput(DIK_W);
			break;
		case SELECT_VEC_DOWN:
			con = UsersInput::Instance()->ControllerInput(0, XBOX_STICK::L_DOWN)
				|| UsersInput::Instance()->ControllerInput(0, XBOX_BUTTON::DPAD_DOWN);
			key = UsersInput::Instance()->KeyInput(DIK_S);
			break;
		case SELECT_VEC_LEFT:
			con = UsersInput::Instance()->ControllerInput(0, XBOX_STICK::L_LEFT)
				|| UsersInput::Instance()->ControllerInput(0, XBOX_BUTTON::DPAD_LEFT);
			key = UsersInput::Instance()->KeyInput(DIK_A);
			break;
		case SELECT_VEC_RIGHT:
			con = UsersInput::Instance()->ControllerInput(0, XBOX_STICK::L_RIGHT)
				|| UsersInput::Instance()->ControllerInput(0, XBOX_BUTTON::DPAD_RIGHT);
			key = UsersInput::Instance()->KeyInput(DIK_D);
			break;
		default:
			break;
	}

	if (con)
	{
		RegisterLatestDevice(INPUT_DEVICE::CONTROLLER);
		device = INPUT_DEVICE::CONTROLLER;
	}
	else if (key)
	{
		RegisterLatestDevice(INPUT_DEVICE::KEY_BOARD_MOUSE);
		device = INPUT_DEVICE::KEY_BOARD_MOUSE;
	}
	else return false;

	//���������̓t���[���X�V
	if (INPUT_FRAME_MAX <= ++m_moveInputFrame[device][arg_vec])
	{
		//���������̓t���[�����Z�b�g
		m_moveInputFrame[device][arg_vec] = 0;
		return true;
	}

	return false;
}

bool OperationConfig::GetTargetChangeVec(SELECT_VEC arg_vec)
{
	static const float STICK_DEAD_RANGE = 0.15f;

	//�^�[�Q�b�g�I���͏c�����̂�
	if (arg_vec == SELECT_VEC_LEFT || arg_vec == SELECT_VEC_RIGHT)return false;

	bool con = false;
	bool key = false;

	switch (arg_vec)
	{
		case SELECT_VEC_UP:
			con = UsersInput::Instance()->ControllerOnTrigger(0, XBOX_STICK::R_UP, STICK_DEAD_RANGE);
			key = UsersInput::Instance()->KeyOnTrigger(DIK_UP);
			break;
		case SELECT_VEC_DOWN:
			con = UsersInput::Instance()->ControllerOnTrigger(0, XBOX_STICK::R_DOWN, STICK_DEAD_RANGE);
			key = UsersInput::Instance()->KeyOnTrigger(DIK_DOWN);
			break;
		default:
			break;
	}

	if (con)RegisterLatestDevice(INPUT_DEVICE::CONTROLLER);
	else if (key)RegisterLatestDevice(INPUT_DEVICE::KEY_BOARD_MOUSE);

	return con || key;
}

bool OperationConfig::GetOperationInput(OPERATION_TYPE arg_operation, INPUT_PATTERN arg_pattern)
{
	//�S�Ă̓��͂��󂯕t���Ă��Ȃ�
	if (!m_isAllInputActive)return false;

	//�C���Q�[���̑�����󂯕t���Ȃ����
	if (!m_isInGameOperationActive &&
		std::find(m_inGameOperationArray.begin(), m_inGameOperationArray.end(), arg_operation) != m_inGameOperationArray.end())return false;

	return KeyInput(arg_pattern, m_operationKeyCode[arg_operation]) || ControllerInput(arg_pattern, m_operationButton[arg_operation]);
}

bool OperationConfig::CheckAllOperationInput()
{
	//�{�^������
	for (int ope = 0; ope < OPERATION_TYPE_NUM; ++ope)
	{
		if (GetOperationInput((OPERATION_TYPE)ope, HOLD))
		{
			return true;
		}
	}
	return false;
}

bool OperationConfig::DebugKeyInputOnTrigger(int arg_keyCode)
{
	if (!m_isDebug)return false;
	return UsersInput::Instance()->KeyOnTrigger(arg_keyCode);
}