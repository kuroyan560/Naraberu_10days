#pragma once
#include <string>
#include <vector>
#include <utility>
#include <map>

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"

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
		// AIのタイプ
		AI_TYPE m_AI_Type;
		// 行動リスト
		std::vector<std::string> ActionList;
		// タグ
		ENEMY_TAG m_Tag;

		// 画像
		std::shared_ptr<KuroEngine::TextureBuffer> m_FrameTex;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_green;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_yellow;
		std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_red;

		std::shared_ptr<KuroEngine::TextureBuffer> m_UnitTex;

		EnemyData() : m_Name("UnNamed"), m_HP(1), m_MaxHP(1), m_AI_Type(AI_TYPE::DEFAULT), m_Tag(ENEMY_TAG::DEFAULT) {}

		template<class... A>
		EnemyData(std::string Name, ENEMY_TAG Tag, int HP, AI_TYPE AI_Type, A... ActPatterns) :
			m_Name(Name), m_Tag(Tag), m_HP(HP), m_MaxHP(HP), m_AI_Type(AI_Type) {

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
				m_HpTex_green = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_green.png");
				m_HpTex_yellow = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_yellow.png");
				m_HpTex_red = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "enemy_hp_gauge_red.png");
			}
			if (m_Tag == ENEMY_TAG::BOSS) {
				m_FrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_frame.png");
				m_HpFrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_frame.png");
				m_HpTex_green = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_gauge_green.png");
				m_HpTex_yellow = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_gauge_yellow.png");
				m_HpTex_red = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "boss_hp_gauge_red.png");
			}

			m_UnitTex = D3D12App::Instance()->GenerateTextureBuffer(EnemyTexturesDir[m_Name]);
		}
	};

	// ここに敵を追加していく
	static const EnemyData DebugEnemy_1 = EnemyData("Debug_1", ENEMY_TAG::DEFAULT, 64, AI_TYPE::DEFAULT, "Attack_01", "Attack_02");
	static const EnemyData DebugEnemy_2 = EnemyData("Debug_2", ENEMY_TAG::DEFAULT, 128, AI_TYPE::DEFAULT, "Attack_02", "Heal_01", "Jamming_01");
	static const EnemyData DebugEnemy_3 = EnemyData("Debug_3", ENEMY_TAG::DEFAULT, 128, AI_TYPE::DEFAULT, "Attack_01", "Attack_01", "Attack_01");

	static const EnemyData DebugEnemy_Boss_1 = EnemyData("Debug_1", ENEMY_TAG::BOSS, 1000, AI_TYPE::DEFAULT, "Attack_01", "Attack_01", "Attack_01");
}