#pragma once
#include <string>
#include <vector>
#include <utility>

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"

namespace EnemysData {

	enum class AI_TYPE {
		DEFAULT, CUSTOM
	};

	class EnemyData {
	public:
		// ���j�b�g��
		std::string m_Name;
		// ���ݑ̗�
		int m_HP;
		// �ő�̗�
		int m_MaxHP;
		// AI�̃^�C�v
		AI_TYPE m_AI_Type;
		// �s�����X�g
		std::vector<std::string> ActionList;

		// �摜
		std::shared_ptr<KuroEngine::TextureBuffer> m_FrameTex;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_green;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_yellow;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_red;

		EnemyData() : m_Name("UnNamed"), m_HP(1), m_MaxHP(1), m_AI_Type(AI_TYPE::DEFAULT) {}

		template<class... A>
		EnemyData(std::string Name, int HP, AI_TYPE AI_Type, A... ActPatterns) :
			m_Name(Name), m_HP(HP), m_MaxHP(HP), m_AI_Type(AI_Type) {

			for (std::string ActionName : std::initializer_list<std::string>{ ActPatterns... }) {
				ActionList.emplace_back(ActionName);
			}
		}

		void SetTexture() {
			using namespace KuroEngine;
			std::string TexDir = "resource/user/tex/battle_scene/";
			m_FrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_icon_frame.png");
			m_HpFrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_frame.png");
			m_HpTex_green = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_green.png");
			m_HpTex_yellow = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_yellow.png");
			m_HpTex_red = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_red.png");
		}
	};

	// �����ɓG��ǉ����Ă���
	static const EnemyData DebugEnemy_1 = EnemyData("Debug_1", 64, AI_TYPE::DEFAULT, "Attack_01", "Attack_02");
	static const EnemyData DebugEnemy_2 = EnemyData("Debug_2", 128, AI_TYPE::DEFAULT, "Attack02", "Heal_01", "Jamming_01");

}