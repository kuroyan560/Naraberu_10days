#pragma once
#include "KuroEngineDevice.h"
#include <array>

class PlayerDamageUi
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ダメージ
	/// </summary>
	void SetDamage(const int _damage) {
		isDamage = true;
		damageNum = _damage;
		timer = 0;
	}

private:

	//ボーナス数字画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 11> numberTex;

	bool isDamage;
	int damageNum;
	KuroEngine::Vec2<float> pos;
	float alpha;
	float timer;
};

