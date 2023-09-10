#pragma once
#include <string>
#include <vector>
#include <utility>
#include <map>

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"

namespace EnemysData {

	enum class AI_TYPE {
		DEFAULT, CUSTOM
	};

	enum class ENEMY_TAG {
		DEFAULT, BOSS
	};

	// 敵のテクスチャ
	static std::map<std::string, std::string> EnemyTexturesDir{
		{ "Debug_1", "resource/user/tex/battle_scene/enemy/inu_blue.png"},
		{ "Debug_2", "resource/user/tex/battle_scene/enemy/lizard_red.png" },
		{ "Debug_3", "resource/user/tex/battle_scene/enemy/zako_yellow.png" },
	};

	class EnemyData {
	public:
		// ユニット名
		std::string m_Name;
		// 現在体力
		int m_HP;
		// 最大体力
		int m_MaxHP;
		// 攻撃力
		int m_ATK;
		// AIのタイプ
		AI_TYPE m_AI_Type;
		// 行動リスト
		std::vector<std::string> ActionList;
		// タグ
		ENEMY_TAG m_Tag;

		// 画像
		std::shared_ptr<KuroEngine::TextureBuffer> m_FrameTex;
		std::shared_ptr<KuroEngine::TextureBuffer> m_FrameTex_Target;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex_Target;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_green;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_yellow;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_red;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_break;
		std::shared_ptr<KuroEngine::TextureBuffer> m_UnitTex;
		std::shared_ptr<KuroEngine::TextureBuffer> m_IndexTex_1;
		std::shared_ptr<KuroEngine::TextureBuffer> m_IndexTex_2;
		std::shared_ptr<KuroEngine::TextureBuffer> m_IndexTex_3;
		std::shared_ptr<KuroEngine::TextureBuffer> m_IndexTex_1_Target;
		std::shared_ptr<KuroEngine::TextureBuffer> m_IndexTex_2_Target;
		std::shared_ptr<KuroEngine::TextureBuffer> m_IndexTex_3_Target;

		std::shared_ptr<KuroEngine::TextureBuffer> m_ReticleTex;

		EnemyData() : m_Name("NONE"), m_HP(1), m_MaxHP(1), m_ATK(1), m_AI_Type(AI_TYPE::DEFAULT), m_Tag(ENEMY_TAG::DEFAULT) {}

		template<class... A>
		EnemyData(std::string Name, ENEMY_TAG Tag, int HP, int ATK, AI_TYPE AI_Type, A... ActPatterns) :
			m_Name(Name), m_Tag(Tag), m_HP(HP), m_MaxHP(HP), m_ATK(ATK), m_AI_Type(AI_Type) {

			for (std::string ActionName : std::initializer_list<std::string>{ ActPatterns... }) {
				ActionList.emplace_back(ActionName);
			}
		}

		void SetTexture() {
			using namespace KuroEngine;
			std::string TexDir = "resource/user/tex/battle_scene/";
			if (m_Tag == ENEMY_TAG::DEFAULT) {
				m_FrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_icon_frame.png");
				m_HpFrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_frame.png");
				m_FrameTex_Target = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "target_enemy_icon_frame.png");
				m_HpFrameTex_Target = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "target_enemy_hp_frame.png");
				m_HpTex_green = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_green.png");
				m_HpTex_yellow = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_yellow.png");
				m_HpTex_red = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_red.png");
				m_HpTex_break = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_break.png");
			}
			if (m_Tag == ENEMY_TAG::BOSS) {
				m_FrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_frame.png");
				m_HpFrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_frame.png");
				m_FrameTex_Target = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "target_enemy_icon_frame.png");
				m_HpFrameTex_Target = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "target_enemy_hp_frame.png");
				m_HpTex_green = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_gauge_green.png");
				m_HpTex_yellow = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_gauge_yellow.png");
				m_HpTex_red = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_gauge_red.png");
				m_HpTex_break = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_gauge_break.png");
			}

			m_ReticleTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "reticle.png");
			m_UnitTex = D3D12App::Instance()->GenerateTextureBuffer(EnemyTexturesDir[m_Name]);
			m_IndexTex_1 = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_number_1.png");
			m_IndexTex_2 = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_number_2.png");
			m_IndexTex_3 = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_number_3.png");
			m_IndexTex_1_Target = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "target_enemy_number_1.png");
			m_IndexTex_2_Target = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "target_enemy_number_2.png");
			m_IndexTex_3_Target = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "target_enemy_number_3.png");
		}
	};

	// ここに敵を追加していく
	static const EnemyData DebugEnemy_1 = EnemyData("Debug_1", ENEMY_TAG::DEFAULT, 64, 30, AI_TYPE::DEFAULT, "Attack_01", "Attack_01", "Attack_02");
	static const EnemyData DebugEnemy_2 = EnemyData("Debug_2", ENEMY_TAG::DEFAULT, 128, 20, AI_TYPE::DEFAULT, "Attack_02", "Heal_01", "Heal_01");
	static const EnemyData DebugEnemy_3 = EnemyData("Debug_3", ENEMY_TAG::DEFAULT, 128, 20, AI_TYPE::DEFAULT, "Attack_01", "Jamming_01", "Jamming_01");

	static const EnemyData DebugEnemy_Boss_1 = EnemyData("Debug_1", ENEMY_TAG::BOSS, 1000, 80, AI_TYPE::DEFAULT, "Attack_01", "Attack_01", "Attack_01");
}