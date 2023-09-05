#include "EnemyActList.h"
#include "UnitBase.h"

void EnemyActions::EnemyActionMgr::Initialize()
{
	using enum EnemyActionList;
	for (int i = 0; i < int(ACTION_MAX); i++) {
		m_List.emplace_back(nullptr);
	}
	m_List[size_t(DEBUG_ATTACK_01)] = std::make_shared<Debug_Attack_01>();

	m_NowAction = false;
}

void EnemyActions::EnemyActionMgr::StartAction(std::string Action_Name)
{
	m_Now_Action_Name = Action_Name;
	m_NowAction = true;

	EnemyActions::GetPtr<Debug_Attack_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Param_Set(20, 200);
	EnemyActions::GetPtr<Debug_Attack_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Need_Object_Set(m_Initiator);

	EnemyActions::GetPtr<Debug_Attack_01>(m_List[size_t(m_List_Pair[m_Now_Action_Name])])->Action_Start();
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