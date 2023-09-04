#pragma once
#include<memory>
#include<string>
#include<list>
#include<array>
#include"Render/RenderObject/ModelInfo/Skeleton.h"
#include<vector>
#include"Common/Transform.h"
#include"ForUser/Timer.h"

namespace KuroEngine
{
	class Model;
	class ConstantBuffer;
	class TimeScale;

	class ModelAnimator
	{
		static const int MAX_BONE_NUM = 256;

		//�Ή�����X�P���g���̎Q��
		std::weak_ptr<Skeleton>attachSkelton;
		//�{�[���̃��[�J���s��
		std::shared_ptr<ConstantBuffer>boneBuff;
		//�{�[���s��iGPU���M�p�j
		std::array<Matrix, MAX_BONE_NUM>boneMatricies;
		//�e�{�[����Transform
		std::vector<Transform>m_boneTransform;

		struct PlayAnimation
		{
			std::string name;	//�A�j���[�V������
			float past = 0.0f;
			bool loop = false;
			bool finish = false;

			PlayAnimation(const std::string& Name, const bool& Loop) :name(Name), loop(Loop) {}
		};

		std::list<PlayAnimation>playAnimations;

		//�{�[��Transform�ꊇ�v�Z
		void CalculateTransform(Transform& BoneTransform, const KuroEngine::Skeleton::BoneAnimation& BoneAnim, const float& Frame, bool& FinishFlg);
		void BoneMatrixRecursive(const int& BoneIdx, const float& Past, bool* Finish, KuroEngine::Skeleton::ModelAnimation& Anim);

	public:
		ModelAnimator() {}
		ModelAnimator(std::weak_ptr<Model>Model);
		void Attach(std::weak_ptr<Model>Model);

		//�P�ʍs��Ŗ��߂�
		void Reset();

		//�w�肵���A�j���[�V�����̎n�܂�̎p�����Z�b�g
		void SetStartPosture(const std::string& AnimationName);

		//�w�肵���A�j���[�V�����̏I���̎p�����Z�b�g
		void SetEndPosture(const std::string& AnimationName);

		//�A�j���[�V�����Đ�
		void Play(const std::string& AnimationName, const bool& Loop, const bool& Blend, float InitPast = 0.0f);
		//��������̃A�j���[�V���������ݍĐ�����
		bool IsPlay()const
		{
			return !playAnimations.empty();
		}

		//�w��̃A�j���[�V���������ݍĐ�����
		bool IsPlay(const std::string& AnimationName)
		{
			auto result = std::find_if(playAnimations.begin(), playAnimations.end(), [AnimationName](PlayAnimation& Anim)
				{
					return AnimationName.compare(Anim.name) == 0;
				});
			return result != playAnimations.end();
		}
		//�A�j���[�V�����X�V
		void Update(const float& arg_timeScale);

		//�{�[���g�����X�t�H�[���ɐe�ݒ�
		void SetParentTransform(Transform& arg_parent);

		const std::shared_ptr<ConstantBuffer>& GetBoneMatBuff() { return boneBuff; }
		Transform& GetBoneTransform(const std::string& BoneName);
	};
}