#pragma once
#include "KuroEngineDevice.h"
#include <array>

class PlayerDamageUi
{
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �_���[�W
	/// </summary>
	void SetDamage(const int _damage) {
		isDamage = true;
		damageNum = _damage;
		timer = 0;
	}

private:

	//�{�[�i�X�����摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 11> numberTex;

	bool isDamage;
	int damageNum;
	KuroEngine::Vec2<float> pos;
	float alpha;
	float timer;
};

