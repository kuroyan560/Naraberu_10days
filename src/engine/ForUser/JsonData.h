#pragma once
#include"json.hpp"
#include<string>
#include<initializer_list>

namespace KuroEngine
{
	class JsonData
	{
	public:
		JsonData() {}
		JsonData(std::string arg_dir, std::string arg_fileName)
		{
			Import(arg_dir, arg_fileName);
		}

		//JSON���f�V���A���C�Y�����f�[�^
		nlohmann::json m_jsonData;

		/// <summary>
		/// �l�̃Q�b�^
		/// </summary>
		/// <typeparam name="T">�^�̎w��</typeparam>
		/// <param name="arg_dest">�l�̊i�[��</param>
		/// <param name="arg_keyArray">�L�[�w��i����ŃL�[�����ĊK�w������Ă����j</param>
		/// <returns>�l���i�[������</returns>
		template<typename T>
		bool Get(T& arg_dest, std::initializer_list<std::string>arg_keyArray)const;

		/// <summary>
		/// �t�@�C���o��
		/// </summary>
		/// <param name="arg_dir">�f�B���N�g���p�X</param>
		/// <param name="arg_name">�t�@�C����</param>
		/// <param name="arg_ext">�g���q</param>
		/// <param name="arg_safeRewrite">�㏑���ۑ��̏ꍇ�A�ŏI�m�F���邩</param>
		void Export(std::string arg_dir, std::string arg_name, std::string arg_ext, bool arg_safeRewrite = true);

		/// <summary>
		/// �t�@�C���ǂݍ���
		/// </summary>
		/// <param name="arg_dir">�f�B���N�g���p�X</param>
		/// <param name="arg_fileName">�t�@�C�����i�g���q�܂߁j</param>
		void Import(std::string arg_dir, std::string arg_fileName);
	};

	template<typename T>
	inline bool JsonData::Get(T& arg_dest, std::initializer_list<std::string> arg_keyArray)const
	{
		//�f�[�^����
		if (m_jsonData.empty())return false;

		//�������̃f�[�^
		nlohmann::json scanData = m_jsonData;

		//�L�[�̔z�񂩂�A�f�[�^�K�w�������Ă���
		for (auto& key : arg_keyArray)
		{
			//�w�肳�ꂽ�L�[��json�I�u�W�F�N�g���Ȃ�
			if (!scanData.contains(key))return false;

			//�K�w���~���
			scanData = scanData[key];
		}

		//�l�̊i�[
		arg_dest = scanData.get<T>();

		return true;
	}
}