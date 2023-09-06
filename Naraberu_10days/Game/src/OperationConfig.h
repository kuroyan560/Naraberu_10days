#pragma once
#include"KuroEngine.h"
#include"Common/Singleton.h"
#include"ForUser/Debugger.h"
#include"FrameWork/UsersInput.h"

class OperationConfig : public KuroEngine::DesignPattern::Singleton<OperationConfig>, public KuroEngine::Debugger
{
public:
	//���̓p�^�[��
	enum INPUT_PATTERN { HOLD, ON_TRIGGER, OFF_TRIGGER, ON_OFF_TRIGGER };

	//������͎��
	enum OPERATION_TYPE
	{
		DONE,	//����
		CANCEL,	//�L�����Z��
		SET_PRISM,	//�v���Y�����p�l���ɐݒu
		SELECT_LEFT_PRISM,	//���̃v���Y����I��
		SELECT_RIGHT_PRISM,	//�E�̃v���Y����I��
		END_TURN,	//�^�[�����I����
		OPERATION_TYPE_NUM
	};

	//�I����͕���
	enum SELECT_VEC
	{
		SELECT_VEC_UP,
		SELECT_VEC_DOWN,
		SELECT_VEC_LEFT,
		SELECT_VEC_RIGHT,
		SELECT_VEC_NUM
	};

private:
	friend class KuroEngine::DesignPattern::Singleton<OperationConfig>;
	OperationConfig();
	
	//�f�o�b�O�̃L�[���͂��󂯕t���邩
	bool m_isDebug = false;

	enum INPUT_DEVICE { KEY_BOARD_MOUSE, CONTROLLER, NUM }m_nowInputDevice = INPUT_DEVICE::KEY_BOARD_MOUSE;
	const std::array<std::string, INPUT_DEVICE::NUM>m_inputDeviceNames =
	{
		"KEY_BOARD_MOUSE","CONTROLLER"
	};

	//�L�[�{�[�h���͂̂Ƃ��̊����L�[
	std::array<int, OPERATION_TYPE_NUM>m_operationKeyCode;
	//�R���g���[���[���͂̂Ƃ��̊����{�^��
	std::array<KuroEngine::XBOX_BUTTON, OPERATION_TYPE_NUM>m_operationButton;

	//�C���Q�[���̑�����͂��󂯕t���Ă��邩
	bool m_isInGameOperationActive = true;
	//�C���Q�[��������͂̔z��
	const std::vector<OPERATION_TYPE>m_inGameOperationArray =
	{
	};
	//�S�Ă̓��͂��󂯂Ď󂯂Ă��邩
	bool m_isAllInputActive = true;

	//�ړ��x�N�g���̒��������̓t���[��
	std::array<std::array<int, INPUT_DEVICE::NUM>, SELECT_VEC_NUM>m_moveInputFrame;

	void OnImguiItems()override;

	//�Ō�Ɏg�������̓f�o�C�X���L�^
	void RegisterLatestDevice(INPUT_DEVICE arg_device)
	{
		m_nowInputDevice = arg_device;
	}

	//�R���g���[���[�ɂ�����
	bool ControllerInput(INPUT_PATTERN arg_pattern, KuroEngine::XBOX_BUTTON arg_xboxButton);
	//�L�[�ɂ�����
	bool KeyInput(INPUT_PATTERN arg_pattern, int arg_keyCode);

public:
	void SetInGameOperationActive(bool arg_active) { m_isInGameOperationActive = arg_active; }
	void SetAllInputActive(bool arg_active) { m_isAllInputActive = arg_active; }

	//�I���������
	bool GetSelectVec(SELECT_VEC arg_vec, INPUT_DEVICE* arg_device = nullptr);
	//�ړ���������
	bool GetMoveVec(SELECT_VEC arg_vec);

	//�^�[�Q�b�g�ύX
	bool GetTargetChangeVec(SELECT_VEC arg_vec);

	//�������
	bool GetOperationInput(OPERATION_TYPE arg_operation, INPUT_PATTERN arg_pattern);

	//������͂���������
	bool CheckAllOperationInput();

	//�f�o�b�O�p�̃L�[����
	bool DebugKeyInputOnTrigger(int arg_keyCode);
};