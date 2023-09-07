#include "PlayerSkills.h"

void PlayerSkills::PlayerSkillMgr::Initialize()
{
}

void PlayerSkills::PlayerSkillMgr::Update()
{
    // スキルリストのすべてを更新
    int Skill_Count = 0;
    for (auto& Skill : m_List) {
        Skill->Action_Update();
        // スキル発動中ならカウントを加算
        if (!Skill->GetEnd()) {
            Skill_Count++;
        }
    }
    // スキルが発動されていない場合リストをクリア
    if (Skill_Count < 1) {
        m_List.clear();
    }
}

void PlayerSkills::PlayerSkillMgr::Draw()
{
    // 発動中のスキル全てを描画
    for (auto& Skill : m_List) {
        Skill->Action_Draw();
    }
}

bool PlayerSkills::PlayerSkillMgr::GetEnd()
{
    return false;
}
