#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "../../src/engine/Common/Singleton.h"

#include "Debug_Attack_01.h"

class UnitBase;

namespace EnemyActions {

	template <class T>
	static std::shared_ptr<T> GetPtr(std::shared_ptr<E_Ac_Base> ptr) { return std::dynamic_pointer_cast<T>(ptr); }

	static std::shared_ptr<E_Ac_Base> SetActionOnName(std::string Action_Name) {
		/*std::shared_ptr<E_Ac_Base> Data;
		if (Action_Name == "Attack_01") {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}
		else if (Action_Name == "Attack_02") {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}
		else if (Action_Name == "Heal_01") {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}
		else if (Action_Name == "Jamming_01") {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}
		else {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}*/
		return std::make_shared<Debug_Attack_01>();
	}

	enum class EnemyActionList {
		DEBUG_ATTACK_01,
		ACTION_MAX
	};
	static std::map<std::string, EnemyActionList> m_List_Pair = {
		{"Attack_01", EnemyActionList::DEBUG_ATTACK_01 },
		{"Attack_02", EnemyActionList::DEBUG_ATTACK_01 }
	};

	class EnemyActionMgr : public KuroEngine::DesignPattern::Singleton<EnemyActionMgr>
	{
		friend class KuroEngine::DesignPattern::Singleton<EnemyActionMgr>;
	private:
		std::vector<std::shared_ptr<E_Ac_Base>> m_List;

		void Initialize();

	private:
		std::string m_Now_Action_Name;
		UnitBase* m_Initiator;
		std::vector<UnitBase*>m_Targets;

		bool m_NowAction;
	public:

		EnemyActionMgr() {
			{
				using enum EnemyActionList;
				for (int i = 0; i < int(ACTION_MAX); i++) {
					m_List.emplace_back(nullptr);
				}
				m_List[size_t(DEBUG_ATTACK_01)] = std::make_shared<Debug_Attack_01>();

				m_NowAction = false;
			}
		}

		void StartAction(std::string Action_Name);

		template<class... A>
		void SetUnits(UnitBase* Initiator, A... Targets) {
			m_Initiator = Initiator;
			for (UnitBase* Target : std::initializer_list<UnitBase*>{ Targets... }) {
				m_Targets.emplace_back(Target);
			}
		}

		void Update();
		void Draw();

		bool GetEnd();
	};

	// -----------------------------
	// 行動に必要な評価値
	// ・全てのユニットのHP
	// ・盤面の状態
	// ・自分のステータス
	// ・プレイヤーのステータス
	// ・プレイヤーの状態

	// ----------------------------
	// 行動決定後に必要な変数
	// ・行動タイプ
	// ・行動結果（ダメージ(回復含む)・状態異常・盤面変化位置）
	// ・リアクション関連
	// 　・アイコンのシェイク
	// 　・カットインの有無
}