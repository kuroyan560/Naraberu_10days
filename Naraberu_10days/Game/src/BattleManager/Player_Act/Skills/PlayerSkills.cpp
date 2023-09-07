#include "PlayerSkills.h"

void PlayerSkills::PlayerSkillMgr::Initialize()
{
}

void PlayerSkills::PlayerSkillMgr::Update()
{
    // �X�L�����X�g�̂��ׂĂ��X�V
    int Skill_Count = 0;
    for (auto& Skill : m_List) {
        Skill->Action_Update();
        // �X�L���������Ȃ�J�E���g�����Z
        if (!Skill->GetEnd()) {
            Skill_Count++;
        }
    }
    // �X�L������������Ă��Ȃ��ꍇ���X�g���N���A
    if (Skill_Count < 1) {
        m_List.clear();
    }
}

void PlayerSkills::PlayerSkillMgr::Draw()
{
    // �������̃X�L���S�Ă�`��
    for (auto& Skill : m_List) {
        Skill->Action_Draw();
    }
}

bool PlayerSkills::PlayerSkillMgr::GetEnd()
{
    return false;
}
