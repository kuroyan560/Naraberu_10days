#include "KuroEngineDevice.h"
#include <array>

class BackGroundEffect
{
private:

	struct Info {
		int isAlive;
		KuroEngine::Vec2<float> pos;
		KuroEngine::Vec2<float> scale;
		float angle;
		float alpha;
		int texNum;
	};

public:
	void Initialize();

	void Update();

	void Draw();

private:

	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 3> triangleTex;
	static const int maxNum = 1200;
	std::array<Info, maxNum> info;
	int timer;

	KuroEngine::Vec2<float> pos_a;
};