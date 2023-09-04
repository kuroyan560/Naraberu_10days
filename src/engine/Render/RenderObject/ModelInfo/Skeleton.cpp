#include "Skeleton.h"
#include"KuroEngine.h"

const std::string KuroEngine::Skeleton::DEFINE_PARENT_BONE_NAME = "Defined_Parent";

void KuroEngine::Skeleton::CreateBoneTree(const Matrix CoordinateSysConvertMat)
{
	//�{�[�����Ȃ��Ȃ疳��
	if (bones.empty())return;

	//�S�Ă̐e�ƂȂ�{�[����ǉ�
	int parentBoneIdx = static_cast<int>(bones.size());
	bones.emplace_back();
	bones.back().name = "Defined_Parent";

	//�{�[���m�[�h�}�b�v�����
	for (int idx = 0; idx < bones.size(); ++idx)
	{
		auto& bone = bones[idx];
		boneIdxTable[bone.name] = idx;

		if (bone.parent != -1)continue;	//���ɐe������
		if (idx == parentBoneIdx)continue;	//���M���S�Ă̐e
		bone.parent = parentBoneIdx;
	}

	//�e�q�֌W�\�z
	for (int i = 0; i < bones.size(); ++i)
	{
		auto& bone = bones[i];
		//�e�C���f�b�N�X���`�F�b�N(���肦�Ȃ��ԍ��Ȃ�Ƃ΂�)
		if (bone.parent < 0 || bones.size() < bone.parent)
		{
			continue;
		}
		bones[bone.parent].children.emplace_back(i);
	}

	coordinateSystemConvertMat = CoordinateSysConvertMat;
}

int KuroEngine::Skeleton::GetIndex(const std::string& BoneName)
{
	assert(!bones.empty());
	assert(boneIdxTable.contains(BoneName));
	return boneIdxTable[BoneName];
}