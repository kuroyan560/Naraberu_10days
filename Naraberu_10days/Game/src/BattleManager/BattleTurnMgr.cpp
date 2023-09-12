#include "BattleTurnMgr.h"
#include "FrameWork/ImguiApp.h"
#include "FrameWork/WinApp.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"

#include "FrameWork/UsersInput.h"
#include "Player_Act/Skills/PlayerSkills.h"
#include "ExistUnits.h"
#include "Reticle/Reticle.h"
#include"../OperationConfig.h"

#include "../Effect/ScreenShakeManager.h"
#include"../SoundConfig.h"

BattleTurnMgr::BattleTurnMgr() {
	m_Whole_Turn_Count = 0;
	TurnNum = 0;
	TurnFrameTime = 0;
	NextGameTimer = 0;
	NextGameTimeFinish = int(float(NextGameTimeFinish_Default) * RefreshRate::RefreshRate_Mag);
	m_IsDefeat = false;
	m_ProgressTime = 0;
	m_FirstTurn = true;
	m_TimeUp_Eff_Timer = 0;
	m_TimeUp_Eff_Timer_Max = int(150.0f * RefreshRate::RefreshRate_Mag);
	T_C_Points.clear();
	T_C_Points.emplace_back(0.0f);
	T_C_Points.emplace_back(0.95f);
	T_C_Points.emplace_back(1.0f);
	T_C_Points.emplace_back(1.0f);
	T_C_Points.emplace_back(1.0f);
	T_C_Points.emplace_back(1.0f);
	T_C_Points.emplace_back(0.95f);
	T_C_Points.emplace_back(0.0f);

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_CutInTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_turn.png");

	m_Timer_Frame_Tex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "timer_gauge_frame.png");
	m_Timer_Gauge_Tex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "timer_gauge.png");
	m_TurnEndTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "turn_end.png");
	m_TurnEnd_EnterTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/operation/key/turn_end.png");
	m_TurnEnd_Crtl_EnterTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/operation/controller/turn_end.png");
	m_TurnEnd_SelectedTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "turn_end_sure.png");

	m_TimeUp_Eff_Timer_CutIn = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "time_up.png");
	m_TimeUp_Eff_Timer_CutIn_Back = D3D12App::Instance()->GenerateTextureBuffer(Color(29, 29, 35, 255));

	gageBG = false;
	gageBGTimer = 0.0f;
}

void BattleTurnMgr::TurnEndButtonUpdate()
{
	using namespace KuroEngine;
	
	// �v���C���[�^�[���̂�
	if (TurnNum == 0 && UnitList[0]->IsAlive() && AliveEnemys()) {
		// �����^�[���G���h
		// ���ݎ���
		GetLocalTime(&NowTime);
		// �ϊ�
		FILETIME ftime1;
		FILETIME ftime2;
		SystemTimeToFileTime(&StartTime, &ftime1);
		SystemTimeToFileTime(&NowTime, &ftime2);
		// int64�ɃL���X�g
		__int64* nTime1 = (__int64*)&ftime1;
		__int64* nTime2 = (__int64*)&ftime2;
		// �o�ߕb
		//m_ProgressTime = (*nTime2 - *nTime1) / 10000 / 1000;
		m_ProgressTime = (*nTime2 - *nTime1);

		// �{�[�Y�^�C���R���e�i
		__int64 TotalPuaseTime = 0;
		for (int i = int(m_PauseTimeContainer.size()) - 1; i >= 0; --i) {
			//if (i != int(m_PauseTimeContainer.size()) - 1) {
				TotalPuaseTime += m_PauseTimeContainer[i];
			//}
		}

		if (m_ProgressTime >= (__int64(600000000) + m_PauseTime + TotalPuaseTime) && !m_Checked_TurnEnd && ExistUnits::Instance()->m_StageName != "Tutorial" &&
			m_TimeUp_Eff_Timer == 0) {
			/*m_Checked_TurnEnd = true;
			m_Moving_Flag = true;
			GetUnitPtr<Player>(UnitList[0])->TurnEndTrigger();
			m_FirstTurn = false;*/
			m_TimeUp_Eff_Timer = 1;
		}

		if (m_TimeUp_Eff_Timer > 0) {
			m_TimeUp_Eff_Timer < m_TimeUp_Eff_Timer_Max ? m_TimeUp_Eff_Timer++ : m_TimeUp_Eff_Timer = m_TimeUp_Eff_Timer_Max;
		}

		if (m_TimeUp_Eff_Timer >= m_TimeUp_Eff_Timer_Max) {
			m_Checked_TurnEnd = true;
			m_Moving_Flag = true;
			m_TimeUp_Eff_Timer = 0;
			GetUnitPtr<Player>(UnitList[0])->TurnEndTrigger();
			m_FirstTurn = false;
		}

		// �^�[���G���h�{�^���������ꂽ(����)
		if (!m_Selected_TurnEnd && !m_Checked_TurnEnd ) {
			if (ExistUnits::Instance()->m_StageName == "Tutorial" && ExistUnits::Instance()->m_CanTurnEnd == true) {
				if (OperationConfig::Instance()->GetOperationInput(OperationConfig::END_TURN, OperationConfig::ON_TRIGGER)) {
					m_Selected_TurnEnd = true;
					m_Selected_TurnEnd_Timer = 0;
				}
			}
			if (ExistUnits::Instance()->m_StageName != "Tutorial") {
				if (OperationConfig::Instance()->GetOperationInput(OperationConfig::END_TURN, OperationConfig::ON_TRIGGER)) {
					m_Selected_TurnEnd = true;
					m_Selected_TurnEnd_Timer = 0;
				}
			}
		}
		// 2��ڈȍ~
		else if (m_Selected_TurnEnd && !m_Checked_TurnEnd) {
			// �^�[���G���h�{�^���������ꂽ
			if (OperationConfig::Instance()->GetOperationInput(OperationConfig::END_TURN, OperationConfig::ON_TRIGGER)) {
				m_Checked_TurnEnd = true;
				m_Moving_Flag = true;
				GetUnitPtr<Player>(UnitList[0])->TurnEndTrigger();
				m_FirstTurn = false;
				m_TimeUp_Eff_Timer = 0;
			}
			// ����ȊO�̃{�^���������ꂽ
			else if (OperationConfig::Instance()->CheckAllOperationInputTrigger()) {
				m_Selected_TurnEnd = false;
			}
		}
	}

	// �ړ��t���O�������Ă鎞(�^�[���G���h�m�莞)
	if (m_Moving_Flag) {
		m_Moving_Timer < m_Moving_Timer_Max ? m_Moving_Timer++ : 0;
	}
	else {
		m_Moving_Timer > 0 ? m_Moving_Timer-- : m_Moving_Timer = 0;
	}

	// 1��ڂ̓��͂�����ҋ@��Ԃ��p�����Ă��鎞
	if (m_Selected_TurnEnd) {
		// �^�C�}�[�̍X�V
		m_Selected_TurnEnd_Timer++;
		m_Scaling_Timer < m_Scaling_Timer_Max ? m_Scaling_Timer++ : 0;
	}
	// �ҋ@��Ԃł͂Ȃ��Ƃ�
	else {
		// �^�C�}�[�̍X�V
		m_Selected_TurnEnd_Timer > 0 ? m_Selected_TurnEnd_Timer-- : m_Selected_TurnEnd_Timer = 0;
		m_Scaling_Timer > 0 ? m_Scaling_Timer-- : m_Scaling_Timer = 0;
	}
}

void BattleTurnMgr::TurnEndButtonDraw()
{
	using namespace KuroEngine;

	// SURE�傫��
	Vec2 SureTexSize_Int = m_TurnEnd_SelectedTex->GetGraphSize();
	Vec2 SureTexSize = Vec2(float(SureTexSize_Int.x), float(SureTexSize_Int.y));
	Vec2 SureRB = Vec2(930.0f, 583.0f);

	// �X�P�[��
	float Scale_Value = m_Scaling_Timer / m_Scaling_Timer_Max;
	Vec2 Scale_Size = SureTexSize * (Scale_Value > 1.0f ? 1.0f : Scale_Value);

	// �ʒu
	float Moving_Pars = m_Moving_Timer / m_Moving_Timer_Max;
	Vec2 Moving_Pos = Vec2(0.0f, 400.0f - ResultEasing(Moving_Pars) * 400.0f);

	// 1��ڂ̓��͂�����ҋ@��Ԃ��p�����Ă��鎞
	if (m_Selected_TurnEnd) {
		DrawFunc2D::DrawExtendGraph2D(SureRB - Scale_Size + Moving_Pos + ScreenShakeManager::Instance()->GetOffset()
			, SureRB + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_SelectedTex);
	}
	// �ҋ@��Ԃł͂Ȃ��Ƃ�
	else {
		DrawFunc2D::DrawExtendGraph2D(SureRB - Scale_Size + Moving_Pos + ScreenShakeManager::Instance()->GetOffset()
			, SureRB + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_SelectedTex);
	}

	Vec2 EndButton_Size_Int = m_TurnEndTex->GetGraphSize();
	Vec2 EndButton_Size = Vec2(float(EndButton_Size_Int.x), float(EndButton_Size_Int.y));
	Vec2 EndButton_LT = Vec2(893.0f, 559.0f);
	Vec2 EndButton_Center = EndButton_LT + EndButton_Size / 2.0f;
	DrawFunc2D::DrawExtendGraph2D(EndButton_Center - EndButton_Size / 2.0f + Moving_Pos + ScreenShakeManager::Instance()->GetOffset()
		, EndButton_Center + EndButton_Size / 2.0f + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEndTex);

	if (OperationConfig::Instance()->GetLatestDevice() == OperationConfig::Instance()->KEY_BOARD_MOUSE) {
		Vec2 EnterButton_Size_Int = m_TurnEnd_EnterTex->GetGraphSize();
		Vec2 EnterButton_Size = Vec2(float(EnterButton_Size_Int.x), float(EnterButton_Size_Int.y));
		Vec2 Height_ = Vec2(0.0f, EnterButton_Size.y / 2.0f - 8.0f);

		DrawFunc2D::DrawExtendGraph2D(EndButton_Center - EnterButton_Size / 2.0f + Height_ + Moving_Pos + ScreenShakeManager::Instance()->GetOffset() - Vec2(5.0f, 0.0f),
			EndButton_Center + EnterButton_Size / 2.0f + Height_ + Moving_Pos + ScreenShakeManager::Instance()->GetOffset() - Vec2(5.0f, 0.0f), m_TurnEnd_EnterTex);

		if (Scale_Value > 0.98f) {
			DrawFunc2D::DrawGraph(Vec2(845.0f, 483.0f) + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_EnterTex);
		}
	}
	else {
		Vec2 EnterButton_Size_Int = m_TurnEnd_Crtl_EnterTex->GetGraphSize();
		Vec2 EnterButton_Size = Vec2(float(EnterButton_Size_Int.x), float(EnterButton_Size_Int.y));
		Vec2 Height_ = Vec2(0.0f, EnterButton_Size.y / 2.0f - 8.0f);

		DrawFunc2D::DrawExtendGraph2D(EndButton_Center - EnterButton_Size / 2.0f + Height_ + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(),
			EndButton_Center + EnterButton_Size / 2.0f + Height_ + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_Crtl_EnterTex);

		if (Scale_Value > 0.98f) {
			DrawFunc2D::DrawGraph(Vec2(849.0f, 483.0f) + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_Crtl_EnterTex);
		}
	}
}

float BattleTurnMgr::ResultEasing(float time)
{
	float t = time;
	if (t > 1.0f) {
		t = 1.0f;
	}
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;
	float Ret = t < 0.5f
		? (powf(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
		: (powf(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	return 1.0f - Ret;
}

void BattleTurnMgr::AutoTurnEndTimerDraw()
{
	if (TurnNum == 0 && ExistUnits::Instance()->m_StageName != "Tutorial") {
		// �����^�[���G���h
			// ���ݎ���
		GetLocalTime(&NowTime);
		// �ϊ�
		FILETIME ftime1;
		FILETIME ftime2;
		SystemTimeToFileTime(&StartTime, &ftime1);
		SystemTimeToFileTime(&NowTime, &ftime2);
		// int64�ɃL���X�g
		__int64* nTime1 = (__int64*)&ftime1;
		__int64* nTime2 = (__int64*)&ftime2;
		// �o�ߕb
		m_ProgressTime = (*nTime2 - *nTime1);
	}

	using namespace KuroEngine;
	Vec2 LT = Vec2(384.0f, 576.0f);
	Vec2 RB = Vec2(896.0f, 589.0f);
	DrawFunc2D::DrawExtendGraph2D(LT + ScreenShakeManager::Instance()->GetOffset(), RB + ScreenShakeManager::Instance()->GetOffset(), m_Timer_Frame_Tex);

	Vec2 LT_Gauge = Vec2(386.0f, 578.0f);
	Vec2 RB_Gauge = Vec2(894.0f, 587.0f);

	// �{�[�Y�^�C���R���e�i
	__int64 TotalPuaseTime = 0;
	for (int i = int(m_PauseTimeContainer.size()) - 1; i >= 0; --i) {
		//if (i != int(m_PauseTimeContainer.size()) - 1) {
			TotalPuaseTime += m_PauseTimeContainer[i];
		//}
	}

	// ���݂̊���
	float Now_Rate = float(m_ProgressTime - m_PauseTime - TotalPuaseTime) / (600000000.0f);
	// �Q�[�W�̒���
	float Gauge_Max_Width = RB_Gauge.x - LT_Gauge.x;
	// ���݂̃Q�[�W�̒���
	float Gauge_Width = Gauge_Max_Width * Now_Rate;

	//�h�N���h�N��
	JustInTime(Now_Rate, LT_Gauge, RB_Gauge - Vec2(Gauge_Width, 0.0f));

	if (ExistUnits::Instance()->m_StageName != "Tutorial") {
		DrawFunc2D_Mask::DrawExtendGraph2D(LT_Gauge + ScreenShakeManager::Instance()->GetOffset(), RB_Gauge + ScreenShakeManager::Instance()->GetOffset(), m_Timer_Gauge_Tex,
			LT_Gauge + ScreenShakeManager::Instance()->GetOffset(), RB_Gauge - Vec2(Gauge_Width, 0.0f) + ScreenShakeManager::Instance()->GetOffset());
	}
	else {
		DrawFunc2D::DrawExtendGraph2D(LT_Gauge + ScreenShakeManager::Instance()->GetOffset(), RB_Gauge + ScreenShakeManager::Instance()->GetOffset(), m_Timer_Gauge_Tex);
	}
}

void BattleTurnMgr::DrawTimerCutIn()
{
	using namespace KuroEngine;
	// �J�b�g�C��
	/*float CutIn_Size = KuroEngine::Math::GetSpline((float(m_TimeUp_Eff_Timer) / float(m_TimeUp_Eff_Timer_Max)) * 5.0f,
		0, T_C_Points);*/

	float CutIn_Size = 0.0f;
	if (float(m_TimeUp_Eff_Timer) < float(m_TimeUp_Eff_Timer_Max) / 2.0f) {
		CutIn_Size = OutQuint(float(m_TimeUp_Eff_Timer) / float(float(m_TimeUp_Eff_Timer_Max) / 2.0f));
	}
	else {
		CutIn_Size = 1.0f - InQuint((float(m_TimeUp_Eff_Timer) - (float(m_TimeUp_Eff_Timer_Max) / 2.0f)) / (float(m_TimeUp_Eff_Timer_Max) / 2.0f));
	}

	float CutIn_Size_Text = CutIn_Size * 36.0f;
	float CutIn_Size_Box = CutIn_Size * 48.0f;

	Vec2 WinSize = WinApp::Instance()->GetExpandWinSize();

	Vec2 Panel_LT = Vec2(391.0f, 67.0f);
	Vec2 Panel_RB = Vec2(893.0f, 564.0f);

	float Center = WinSize.x / 2.0f;

	Center = WinSize.x - OutInQuart(float(m_TimeUp_Eff_Timer), 0.0f, 1.0f, float(m_TimeUp_Eff_Timer_Max)) * WinSize.x;

	DrawFunc2D_Mask::DrawExtendGraph2D(Vec2(0.0f, 320.0f - CutIn_Size_Box / 2.0f) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(WinSize.x, 320.0f + CutIn_Size_Box / 2.0f) + ScreenShakeManager::Instance()->GetOffset(), m_TimeUp_Eff_Timer_CutIn_Back
		, Panel_LT, Panel_RB);

	DrawFunc2D_Mask::DrawExtendGraph2D(Vec2(Center - 99.0f, 320.0f - CutIn_Size_Text / 2.0f) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(Center + 99.0f, 320.0f + CutIn_Size_Text / 2.0f) + ScreenShakeManager::Instance()->GetOffset(), m_TimeUp_Eff_Timer_CutIn
		, Panel_LT, Panel_RB);
}

void BattleTurnMgr::JustInTime(const float _Now_Rate, const KuroEngine::Vec2<float> _pos1, const KuroEngine::Vec2<float> _pos2)
{
	using namespace KuroEngine;
	if (_Now_Rate < 0.5f || _Now_Rate >= 1.0f) { return; }

	float maxTimerRate = Math::Ease(EASE_CHANGE_TYPE::In, EASING_TYPE::Sine,
		_Now_Rate - 0.5f, 0.4f, 70.0f, 40.0f);

	float alphaRate = Math::Ease(EASE_CHANGE_TYPE::In, EASING_TYPE::Sine,
		_Now_Rate - 0.5f, 0.4f, 0.5f, 1.0f);

	const float maxTimer = maxTimerRate * RefreshRate::RefreshRate_Mag;
	Vec2<float> scale = { 0.0f,0.0f };
	float alpha = 0.0f;
	const KuroEngine::Vec2<float> maxScale = { 8.0f,8.0f };
	gageBGTimer++;
	//��~
	if (gageBG) {
		if (gageBGTimer > maxTimer) {
			gageBG = true;
			gageBGTimer = 0.0f;
		}
		return;
	}
	//�g��
	else {
		scale = Math::Ease(EASE_CHANGE_TYPE::Out, EASING_TYPE::Sine,
			gageBGTimer, maxTimer, { 0.0f,0.0f }, maxScale);

		alpha = Math::Ease(EASE_CHANGE_TYPE::In, EASING_TYPE::Sine,
			gageBGTimer, maxTimer, 1.0f, 0.0f) * alphaRate;

		if (gageBGTimer > maxTimer) {
			gageBG=0;
			gageBGTimer = 0.0f;
		}
	}

	//���ԃo�[
	DrawFunc2D::DrawExtendGraph2D(_pos1 - scale + ScreenShakeManager::Instance()->GetOffset(), _pos2 + scale + ScreenShakeManager::Instance()->GetOffset(),
		m_Timer_Gauge_Tex, alpha);

	Color color = { 0.498f,0.576f,0.866f,alpha };
	DrawFunc2D::DrawBox2D(Vec2<float>(385.0f, 60.0f) - scale, Vec2<float>(391.0f, 572.0f) + scale, color, true);
	DrawFunc2D::DrawBox2D(Vec2<float>(385.0f, 566.0f) - scale, Vec2<float>(899.0f, 572.0f) + scale, color, true);
	DrawFunc2D::DrawBox2D(Vec2<float>(892.0f, 60.0f) - scale, Vec2<float>(899.0f, 572.0f) + scale, color, true);
	DrawFunc2D::DrawBox2D(Vec2<float>(385.0f, 60.0f) - scale, Vec2<float>(899.0f, 66.0f) + scale, color, true);
}

void BattleTurnMgr::OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.clear();
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	m_Total_Turn_Count = 1;
	TurnNum = 0;
	TurnFrameTime = 0;
	NextGameTimer = 0;
	ExistUnits::Instance()->m_NowTarget = 0;
	m_IsDefeat = false;

	using namespace KuroEngine;

	// �^�[���G���h
	m_Selected_TurnEnd = false;
	m_Checked_TurnEnd = false;
	m_Selected_TurnEnd_Timer = 0;
	m_Scaling_Timer = 0;
	m_Scaling_Timer_Max = 20.0f * RefreshRate::RefreshRate_Mag;
	m_Moving_Flag = false;
	m_Moving_Timer = 0;
	m_Moving_Timer_Max = 80.0f * RefreshRate::RefreshRate_Mag;

	m_FirstTurn = true;

	// �����^�[���G���h
	m_PauseTime = 0;
	m_PauseTimeContainer.clear();
	GetLocalTime(&StartTime);

	// ���e�B�N��
	Reticle::Instance()->SetBattleTurnManager(this);

	// �J�b�g�C���X�^�[�g
	CutInMgr::Instance()->StartCutIn(CutInType::PLAYER_TURN);
}

void BattleTurnMgr::SetUnits(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.clear();
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	m_Total_Turn_Count++;
	TurnNum = 0;
	TurnFrameTime = 0;
	NextGameTimer = 0;
	// �^�[���G���h
	m_Selected_TurnEnd = false;
	m_Checked_TurnEnd = false;
	m_Moving_Flag = false;
	m_Scaling_Timer_Max = 20.0f * RefreshRate::RefreshRate_Mag;
	// ���ݎ���
	m_PauseTime = 0;
	m_PauseTimeContainer.clear();
	GetLocalTime(&StartTime);

	ExistUnits::Instance()->m_NowTarget = 0;
	m_IsDefeat = false;

	m_FirstTurn = true;
}

void BattleTurnMgr::OnUpdate()
{
	// �펞�X�V����
	for (auto& Unit : UnitList) {
		Unit->OnAlwaysUpdate();
	}

	// �G�̐�������
	bool EnemyAlive = false;

	// �v���C���[�^�[���G��������
	// �����Ă�G�����邩�m�F
	for (int i = 1; i < UnitList.size(); i++) {
		if (UnitList[i]->IsAlive()) {
			// �����Ă�G������΃t���O���グ��
			EnemyAlive = true;
		}
	}

	// �v���C���[������ł���
	if (!UnitList[0]->IsAlive()) {
		if (!m_IsDefeat)
		{
			SoundConfig::Instance()->Play(SoundConfig::SE_GAME_OVER, 35);
		}

		// �o�g���I������(�s�k)
		m_IsDefeat = true;
	}

	// �����Ă�G�����Ȃ�
	if (!EnemyAlive) {
		// �o�g���I������(����)
		Update_NextWave();
	}

	TurnEndButtonUpdate();

	// �ʏ�̍X�V����
	if (EnemyAlive && UnitList[TurnNum]->IsAlive() && UnitList[0]->IsAlive()) {
		Update_Battle();
	}
	// ���b�N�I��
	Reticle::Instance()->Update();

	// �X�L���̍X�V
	PlayerSkills::PlayerSkillMgr::Instance()->Update();
}

void BattleTurnMgr::OnDraw()
{
	for (int i = 1; i < UnitList.size(); i++) {
		int EnemyIndex = i - 1;
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[i]);

		// �v���C���[�^�[���Ȃ�Â����Ȃ�
		if (TurnNum == 0) {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), false, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
		}
		// �G�^�[���̎������̃^�[���ӊO�Ȃ�Â�����
		else if (i != TurnNum) {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), true, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
		}
		else {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), false, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
		}
	}

	// �G�`��
	UnitList[TurnNum]->OnDraw();

	// �v���C���[�X�L���`��
	PlayerSkills::PlayerSkillMgr::Instance()->Draw();

	for (int i = 1; i < UnitList.size(); i++) {
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[i]);
		En->DrawHpGauge();
	}

	// �^�[���G���h�{�^��
	TurnEndButtonDraw();

	using namespace KuroEngine;
	// �J�b�g�C�����ł����
	if (CutInMgr::Instance()->NowCutIn()) {
		CutInMgr::Instance()->OnDraw();
	}
}

void BattleTurnMgr::OnImguiDebug()
{
	ImGui::Begin("GameScene");
	ImGui::Text("Naw Trun is %d", TurnNum);
	if (TurnNum == 0) {
		ImGui::Text("Type : Player");
	}
	else {
		ImGui::Text("Type : Enemy");
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[TurnNum]);
		ImGui::Text(En->GetEnemyData().m_Name.c_str());
		ImGui::Text("HP : %d", En->GetEnemyData().m_HP);
		for (auto& ActionName : En->GetEnemyData().ActionList) {
			ImGui::Text(ActionName.c_str());
		}
	}
	ImGui::End();
}

void BattleTurnMgr::NextTurnStart()
{
	TurnFrameTime = 0;
}

void BattleTurnMgr::Update_NextWave()
{

	UnitList[0]->StartTurn();
	m_FirstTurn = true;
	GetUnitPtr<Player>(UnitList[0])->m_DoBonus = false;
	GetUnitPtr<Player>(UnitList[0])->TurnChangeTimer = 0;
	NextGameTimer++;
	if (NextGameTimer >= NextGameTimeFinish) {

	}
}

void BattleTurnMgr::Update_Battle()
{
	if (UnitList[TurnNum]->IsNextTurn() && m_FirstTurn == false) {
		if (TurnNum == 0) {
			if(GetUnitPtr<Player>(UnitList[TurnNum])->GetEndBonusAttack() == true && GetUnitPtr<Player>(UnitList[TurnNum])->m_DoBonus){
				// �^�[���I������
				UnitList[TurnNum]->End();
				m_FirstTurn = false;
				// �^�[���؂�ւ��E�S�̃^�[�������Z
				// �����Ă郆�j�b�g�܂Ŕ�΂�
				while (1) {
					// �����Ă�G������Ȃ�
					if (AliveEnemys() && TurnNum == 0) {
						CutInMgr::Instance()->StartCutIn(CutInType::ENEMY_TURN);
					}
					TurnNum < UnitList.size() - 1 ? TurnNum++ : TurnNum = 0, m_Whole_Turn_Count++;
					// �����Ă郆�j�b�g�̃^�[��
					if (UnitList[TurnNum]->IsAlive()) {
						UnitList[TurnNum]->StartTurn();
						if (TurnNum == 0) {
							m_Moving_Flag = false;
							//m_AutoTurnEndTimer = 0;
							m_PauseTime = 0;
							m_PauseTimeContainer.clear();
							GetLocalTime(&StartTime);
						}
						m_Checked_TurnEnd = false;
						m_Selected_TurnEnd = false;
						NextTurnStart();
						break;
					}
				}

				if (TurnNum == 0) {
					UnitList[TurnNum]->m_isMyTurn = true;
					UnitList[TurnNum]->m_NextTurn = false;
					CutInMgr::Instance()->StartCutIn(CutInType::PLAYER_TURN);
					GetUnitPtr<Player>(UnitList[TurnNum])->m_DoBonus = false;
					GetUnitPtr<Player>(UnitList[TurnNum])->TurnChangeTimer = 0;
				}
				/*else if (TurnNum == 1) {
					CutInMgr::Instance()->StartCutIn(CutInType::ENEMY_TURN);
				}*/
			}
		}
		else {
			// �^�[���I������
			UnitList[TurnNum]->End();

			// �^�[���؂�ւ��E�S�̃^�[�������Z
			// �����Ă郆�j�b�g�܂Ŕ�΂�
			while (1) {
				// �����Ă�G������Ȃ�
				if (AliveEnemys() && TurnNum == 0) {
					CutInMgr::Instance()->StartCutIn(CutInType::ENEMY_TURN);
				}
				TurnNum < UnitList.size() - 1 ? TurnNum++ : TurnNum = 0, m_Whole_Turn_Count++;
				// �����Ă郆�j�b�g�̃^�[��
				if (UnitList[TurnNum]->IsAlive()) {
					UnitList[TurnNum]->StartTurn();
					if (TurnNum == 0) {
						m_Moving_Flag = false;
						//m_AutoTurnEndTimer = 0;
						m_PauseTime = 0;
						m_PauseTimeContainer.clear();
						GetLocalTime(&StartTime);
					}
					m_Checked_TurnEnd = false;
					m_Selected_TurnEnd = false;
					NextTurnStart();
					break;
				}
			}

			if (TurnNum == 0) {
				UnitList[TurnNum]->m_isMyTurn = true;
				UnitList[TurnNum]->m_NextTurn = false;
				CutInMgr::Instance()->StartCutIn(CutInType::PLAYER_TURN);
				GetUnitPtr<Player>(UnitList[TurnNum])->m_DoBonus = false;
				GetUnitPtr<Player>(UnitList[TurnNum])->TurnChangeTimer = 0;
			}
			/*else if (TurnNum == 1) {
				CutInMgr::Instance()->StartCutIn(CutInType::ENEMY_TURN);
			}*/
		}
	}
	// �J�b�g�C�����ł����
	if (CutInMgr::Instance()->NowCutIn()) {
		CutInMgr::Instance()->OnUpdate();
	}
	if (!CutInMgr::Instance()->NowCutIn()) {
		// FrameTime���Z
		TurnFrameTime++;
		// �^�[���X�V
		UnitList[TurnNum]->OnUpdate();
	}

	ExistUnits::Instance()->m_NowTurn = TurnNum;
}

bool BattleTurnMgr::AliveEnemys()
{
	for (int i = 1; i < UnitList.size(); i++) {
		if (UnitList[i]->IsAlive()) {
			// �����Ă�G������΃t���O���グ��
			return true;
		}
	}
	return false;
}
