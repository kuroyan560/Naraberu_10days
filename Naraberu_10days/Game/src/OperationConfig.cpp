#include "OperationConfig.h"
#include"FrameWork/UsersInput.h"
#include"FrameWork/WinApp.h"

using namespace KuroEngine;

OperationConfig::OperationConfig()
	:Debugger("OperationConfig", true)
{
	//各デバイスでの設定値をカスタムパラメータに追加
	//for (int i = 0; i < INPUT_DEVICE::NUM; ++i)
	//{
	//	auto deviceName = m_inputDeviceNames[i];
	//	AddCustomParameter("CameraSensitivity", { deviceName,"CameraSensitivity" },
	//		PARAM_TYPE::FLOAT, &m_params[i].m_camSensitivity, deviceName);
	//}

	//LoadParameterLog();

	//操作キー割り当て
	m_operationKeyCode =
	{
		DIK_SPACE,	//決定
		DIK_ESCAPE,	//キャンセル
		DIK_SPACE,	//プリズムをパネルに設置
		DIK_L,	//左のプリズムを選択
		DIK_RIGHT,	//右のプリズムを選択
		DIK_RETURN,	//ターンを終える
	};

	//操作ボタン割り当て
	m_operationButton =
	{
		A,	//決定
		B,	//キャンセル
		B,	//プリズムをパネルに設置
		LB,	//左のプリズムを選択
		RB,	//右のプリズムを選択
		START,	//ターンを終える
	};
}

void OperationConfig::OnImguiItems()
{
	using namespace KuroEngine;

	//最後の入力のデバイス
	ImGui::Text("LatestDevice : %s", m_inputDeviceNames[m_nowInputDevice].c_str());

	//マウス入力表示
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
	//トリガー入力
	INPUT_DEVICE device;
	bool trigger = GetSelectVec(arg_vec, &device);
	if (trigger)
	{
		//長押しフレームのリセット
		m_moveInputFrame[device][arg_vec] = 0;
		return true;
	}

	//長押しで入力判定にするフレーム
	static const int INPUT_FRAME_MAX = 8;

	bool con = false;
	bool key = false;

	//長押し入力
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

	//長押し入力フレーム更新
	if (INPUT_FRAME_MAX <= ++m_moveInputFrame[device][arg_vec])
	{
		//長押し入力フレームリセット
		m_moveInputFrame[device][arg_vec] = 0;
		return true;
	}

	return false;
}

bool OperationConfig::GetTargetChangeVec(SELECT_VEC arg_vec)
{
	static const float STICK_DEAD_RANGE = 0.15f;

	//ターゲット選択は縦方向のみ
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
	//全ての入力を受け付けていない
	if (!m_isAllInputActive)return false;

	//インゲームの操作を受け付けない状態
	if (!m_isInGameOperationActive &&
		std::find(m_inGameOperationArray.begin(), m_inGameOperationArray.end(), arg_operation) != m_inGameOperationArray.end())return false;

	return KeyInput(arg_pattern, m_operationKeyCode[arg_operation]) || ControllerInput(arg_pattern, m_operationButton[arg_operation]);
}

bool OperationConfig::CheckAllOperationInput()
{
	//ボタン入力
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