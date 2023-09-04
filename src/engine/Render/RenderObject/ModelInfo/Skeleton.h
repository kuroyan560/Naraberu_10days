#pragma once
#include"KuroEngine.h"
#include<string>
#include<map>
#include"Render/RenderObject/Animation.h"
#include<array>

namespace KuroEngine
{
	class Bone
	{
	public:
		static size_t GetSizeWithOutName()
		{
			return sizeof(char) + sizeof(int) + sizeof(Vec3<float>) + sizeof(Matrix);
		}

		std::string name;
		char parent = -1;	//�e�{�[��
		std::vector<char>children;	//�q�{�[��
		int transLayer = 0;	//�ό`�K�w
		Vec3<float> pos = { 0.0f,0.0f,0.0f };
		Matrix invBindMat = DirectX::XMMatrixIdentity();
	};

	class Skeleton
	{
	public:
		static const std::string DEFINE_PARENT_BONE_NAME;
		struct BoneAnimation
		{
		public:
			Animation<Vec3<float>>posAnim;
			Animation<XMVECTOR>rotateAnim;
			Animation<Vec3<float>>scaleAnim;
		};
		struct ModelAnimation
		{
			//�S�Ẵ{�[���̃A�j���[�V�������I�����鎞��
			int finishTime;
			//�{�[���P�ʂ̃A�j���[�V����
			std::map<std::string, BoneAnimation>boneAnim;
		};

		//���W�n�ϊ����C�����邽�߂̍s��
		Matrix coordinateSystemConvertMat = XMMatrixIdentity();

		std::vector<Bone>bones;
		std::map<std::string, int>boneIdxTable;
		/*
			�A�j���[�V�������iSkeleton���A�j���[�V�������s����ł͖����BAnimator����̎Q�Ɨp�j
			�L�[�� �A�j���[�V������
		*/
		std::map<std::string, ModelAnimation>animations;

		void CreateBoneTree(const Matrix CoordinateSysConvertMat);
		int GetIndex(const std::string& BoneName);
	};
}