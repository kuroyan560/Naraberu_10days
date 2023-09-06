#pragma once
#include"KuroEngine.h"
#include"Common/Singleton.h"
#include"ForUser/Debugger.h"
#include"FrameWork/UsersInput.h"

class OperationConfig : public KuroEngine::DesignPattern::Singleton<OperationConfig>, public KuroEngine::Debugger
{
public:
	//入力パターン
	enum INPUT_PATTERN { HOLD, ON_TRIGGER, OFF_TRIGGER, ON_OFF_TRIGGER };

	//操作入力種別
	enum OPERATION_TYPE
	{
		DONE,	//決定
		CANCEL,	//キャンセル
		SET_PRISM,	//プリズムをパネルに設置
		SELECT_LEFT_PRISM,	//左のプリズムを選択
		SELECT_RIGHT_PRISM,	//右のプリズムを選択
		END_TURN,	//ターンを終える
		OPERATION_TYPE_NUM
	};

	//選択入力方向
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
	
	//デバッグのキー入力を受け付けるか
	bool m_isDebug = false;

	enum INPUT_DEVICE { KEY_BOARD_MOUSE, CONTROLLER, NUM }m_nowInputDevice = INPUT_DEVICE::KEY_BOARD_MOUSE;
	const std::array<std::string, INPUT_DEVICE::NUM>m_inputDeviceNames =
	{
		"KEY_BOARD_MOUSE","CONTROLLER"
	};

	//キーボード入力のときの割当キー
	std::array<int, OPERATION_TYPE_NUM>m_operationKeyCode;
	//コントローラー入力のときの割当ボタン
	std::array<KuroEngine::XBOX_BUTTON, OPERATION_TYPE_NUM>m_operationButton;

	//インゲームの操作入力を受け付けているか
	bool m_isInGameOperationActive = true;
	//インゲーム操作入力の配列
	const std::vector<OPERATION_TYPE>m_inGameOperationArray =
	{
	};
	//全ての入力を受けて受けているか
	bool m_isAllInputActive = true;

	//移動ベクトルの長押し入力フレーム
	std::array<std::array<int, INPUT_DEVICE::NUM>, SELECT_VEC_NUM>m_moveInputFrame;

	void OnImguiItems()override;

	//最後に使った入力デバイスを記録
	void RegisterLatestDevice(INPUT_DEVICE arg_device)
	{
		m_nowInputDevice = arg_device;
	}

	//コントローラーによる入力
	bool ControllerInput(INPUT_PATTERN arg_pattern, KuroEngine::XBOX_BUTTON arg_xboxButton);
	//キーによる入力
	bool KeyInput(INPUT_PATTERN arg_pattern, int arg_keyCode);

public:
	void SetInGameOperationActive(bool arg_active) { m_isInGameOperationActive = arg_active; }
	void SetAllInputActive(bool arg_active) { m_isAllInputActive = arg_active; }

	//選択方向入力
	bool GetSelectVec(SELECT_VEC arg_vec, INPUT_DEVICE* arg_device = nullptr);
	//移動方向入力
	bool GetMoveVec(SELECT_VEC arg_vec);

	//ターゲット変更
	bool GetTargetChangeVec(SELECT_VEC arg_vec);

	//操作入力
	bool GetOperationInput(OPERATION_TYPE arg_operation, INPUT_PATTERN arg_pattern);

	//操作入力があったか
	bool CheckAllOperationInput();

	//デバッグ用のキー入力
	bool DebugKeyInputOnTrigger(int arg_keyCode);
};