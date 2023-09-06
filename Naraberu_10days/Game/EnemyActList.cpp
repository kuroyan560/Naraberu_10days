#include "EnemyActList.h"
#include "UnitBase.h"
#include "ExistUnits.h"

#include "RefreshRate.h"

void EnemyActions::EnemyActionMgr::Initialize()
{
	/*using enum EnemyActionList;
	for (int i = 0; i < int(ACTION_MAX); i++) {
		m_List.emplace_back(nullptr);
	}
	m_List[size_t(DEBUG_ATTACK_01)] = std::make_shared<Debug_Attack_01>();

	m_NowAction = false;*/
}

void EnemyActions::EnemyActionMgr::StartAction(std::string Action_Name)
{
	m_Now_Action_Name = Action_Name;
	m_NowAction = true;

	if (m_Now_Action_Name == "Attack_01") {
		EnemyActions::GetPtr<Debug_Attack_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Ready();
		EnemyActions::GetPtr<Debug_Attack_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Need_Object_Set(m_Initiator, m_Targets[0]);
		EnemyActions::GetPtr<Debug_Attack_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Action_Start();
	}
	else if (m_Now_Action_Name == "Attack_02") {
		EnemyActions::GetPtr<Debug_Attack_02>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Ready();
		EnemyActions::GetPtr<Debug_Attack_02>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Need_Object_Set(m_Initiator, m_Targets[0]);
		EnemyActions::GetPtr<Debug_Attack_02>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Action_Start();
	}
	else if (m_Now_Action_Name == "Heal_01") {
		EnemyActions::GetPtr<Debug_Heal_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Ready();
		EnemyActions::GetPtr<Debug_Heal_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Need_Object_Set(m_Initiator, m_Targets[0]);
		EnemyActions::GetPtr<Debug_Heal_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Action_Start();
	}
	else if (m_Now_Action_Name == "Jamming_01") {
		EnemyActions::GetPtr<Debug_Jamming_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Ready();
		EnemyActions::GetPtr<Debug_Jamming_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Need_Object_Set(m_Initiator, ExistUnits::Instance()->m_MapChipData);
		EnemyActions::GetPtr<Debug_Jamming_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Action_Start();
	}
}

void EnemyActions::EnemyActionMgr::Update()
{
	// アクション中でないなら何もしない
	if (!m_NowAction) {
		return;
	}
	// アクションの更新
	m_List[size_t(m_List_Pair[m_Now_Action_Name])]->Action_Update();
	// アクションが終わった
	if (m_List[size_t(m_List_Pair[m_Now_Action_Name])]->GetEnd()) {
		m_NowAction = false;
	}
}

void EnemyActions::EnemyActionMgr::Draw()
{
	// アクション中でないなら何もしない
	if (!m_NowAction) {
		return;
	}
	m_List[size_t(m_List_Pair[m_Now_Action_Name])]->Action_Draw();
}

bool EnemyActions::EnemyActionMgr::GetEnd()
{
	return m_List[size_t(m_List_Pair[m_Now_Action_Name])]->GetEnd();
}