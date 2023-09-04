#pragma once
#include<string>
#include<vector>
#include<list>
#include"FrameWork/ImguiApp.h"
#include"JsonData.h"
#include<map>
#include"Common/Color.h"

namespace KuroEngine
{
	//imgui�f�o�b�O�̃C���^�[�t�F�[�X
	class Debugger
	{
	private:
		//���蓖�Ă鎯�ʔԍ�
		static int s_id;
		//�N�����̃f�o�b�K�z��
		static std::vector<Debugger*>s_registeredDebuggerArray;
		//���������f�o�b�K�z��
		static std::vector<Debugger*>s_debuggerArray;

		//��exe���Ă��p�����[�^���c�����߂̂���
		//�p�����[�^���O�̃t�@�C���f�B���N�g��
		static const std::string s_jsonFileDir;
		//�p�����[�^���O�̃t�@�C����
		static const std::string s_jsonName;
		//�p�����[�^���O�̃t�@�C���g���q
		static const std::string s_jsonExt;

		//�f�o�b�K�̐ݒ荀�ڃL�[
		static const std::string s_settingKey;
		//�f�o�b�K�̃C���[�W�J���[���ڃL�[
		static const std::string s_settingColKey;
		//�f�o�b�K�̃e�L�X�g�J���[�[�t���O���ڃL�[
		static const std::string s_settingTextColKey;
		//�f�o�b�K�̃A�N�e�B�u��ԍ��ڃL�[
		static const std::string s_settingActiveKey;
		//�f�o�b�K�̃J�X�^���p�����[�^�̃A�N�e�B�u��ԍ��ڃL�[
		static const std::string s_settingCustomParamKey;

		//�p�����[�^���O
		static JsonData s_parameterLog;

	protected:
		//�J�X�^���p�����[�^�̌^
		enum struct PARAM_TYPE
		{
			INT, INT_VEC2, INT_VEC3, INT_VEC4,
			FLOAT, FLOAT_VEC2, FLOAT_VEC3, FLOAT_VEC4,
			BOOL,
			STRING,
			COLOR,
			NONE
		};

	public:
		virtual ~Debugger() { WriteParameterLog(); }
		//�f�o�b�O�@�\�\��
		static void Draw();
		//�f�o�b�K�o�^
		static void Register(std::vector<Debugger*>arg_debuggerArray)
		{
			s_registeredDebuggerArray = arg_debuggerArray;
		}
		//�f�o�b�K�o�^����
		static void ClearRegister() 
		{
			s_registeredDebuggerArray.clear(); 
		}

		//�p�����[�^���O���t�@�C���ǂݍ���
		static void ImportParameterLog()
		{
			s_parameterLog.Import(s_jsonFileDir, s_jsonName + s_jsonExt);
		}
		//�p�����[�^���O���t�@�C���o��
		static void ExportParameterLog()
		{
			for (auto debugger : s_debuggerArray)debugger->WriteParameterLog();
			s_parameterLog.Export(s_jsonFileDir, s_jsonName, s_jsonExt, false);
		}

	private:
		//���ʔԍ�
		int m_id;
		//�A�N�e�B�u���
		bool m_active = false;
		//imguiWindow�t���O
		ImGuiWindowFlags m_imguiWinFlags;

		//�J�X�^���p�����[�^�iexe���Ă����������l���c���j
		struct CustomParameter
		{
			//���O
			std::string m_label;
			//json�ɋL�^����ۂ̃L�[�z��i����ŊK�w������Ă����j
			std::vector<std::string>m_key;
			//�^
			PARAM_TYPE m_type = PARAM_TYPE::NONE;
			//�l�𓯊�����ϐ��̃|�C���^
			void* m_dataPtr = nullptr;
			//������������邩
			bool m_isMinMax = false;
			//����
			float m_min = 0.0f;
			//���
			float m_max = 0.0f;

			CustomParameter(std::string arg_label, std::initializer_list<std::string>arg_key, PARAM_TYPE arg_type, void* arg_destPtr,
				bool arg_isMinMax, float arg_min, float arg_max)
				:m_label(arg_label), m_key(arg_key), m_type(arg_type), m_dataPtr(arg_destPtr), m_isMinMax(arg_isMinMax), m_min(arg_min), m_max(arg_max) {}
		};

		//�p�����[�^���O�iexe���Ă��c���p�����[�^�j
		std::list<CustomParameter>m_customParamList;
		//�J�X�^���p�����[�^��imgui��ŕ\������ۂ�Tree�g����
		std::map<std::string, std::vector<CustomParameter*>>m_customParamGroup;


		//�J�X�^���p�����[�^��imgui��ł�����֐�
		void CustomParameterOnImgui();
		//�J�X�^���p�����[�^�����E�B���h�E�̃A�N�e�B�u���
		bool m_customParamActive = true;
		//�i�����l�Ɍ���j�l���h���b�O����ۂ̕ω���
		float m_customParamDragSpeed = 0.05f;
		//�J�X�^���p�����[�^���ω�������
		bool m_customParamDirty = false;

	protected:
		//imgui�E�B���h�E��
		std::string m_title;
		//�F
		ImVec4 m_debuggerColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
		//�����F����
		bool m_textColor = false;

		Debugger(std::string arg_title, bool arg_active = false, bool arg_customParamActive = false, ImGuiWindowFlags arg_imguiWinFlags = 0)
			:m_title(arg_title), m_active(arg_active), m_customParamActive(arg_customParamActive), m_id(s_id++), m_imguiWinFlags(arg_imguiWinFlags)
		{
			s_debuggerArray.emplace_back(this);
		}

		/// <summary>
		/// �J�X�^���p�����[�^�ȊO��Imgui���������R�ɒ�`�iBegin ~ End �̊ԂŌĂ΂��j
		/// </summary>
		virtual void OnImguiItems() {};

		/// <summary>
		/// �J�X�^���p�����[�^�̒ǉ�
		/// </summary>
		/// <param name="arg_label">�J�X�^���p�����[�^�̖��O</param>
		/// <param name="arg_key">json��ŋL�^����ۂ̃L�[�z��i����ŊK�w������Ă����j</param>
		/// <param name="arg_type">�p�����[�^�̌^</param>
		/// <param name="arg_destPtr">�J�X�^���p�����[�^�̓�����</param>
		/// <param name="arg_imguiTreeName">imgui��ł�Tree�g������O���[�v��</param>
		/// <param name="arg_isMinMax">�i�����l�ɂ�����j������������邩</param>
		/// <param name="arg_min">����</param>
		/// <param name="arg_max">���</param>
		void AddCustomParameter(std::string arg_label, std::initializer_list<std::string>arg_key, PARAM_TYPE arg_type, void* arg_destPtr, std::string arg_imguiTreeName,
			bool arg_isMinMax = false, float arg_min = 0.0f, float arg_max = 0.0f)
		{
			m_customParamList.emplace_back(arg_label, arg_key, arg_type, arg_destPtr, arg_isMinMax, arg_min, arg_max);
			m_customParamGroup[arg_imguiTreeName].push_back(&m_customParamList.back());
		}

		//�p�����[�^���O����J�X�^���p�����[�^�Ǎ�
		void LoadParameterLog();
		//�p�����[�^���O�ɃJ�X�^���p�����[�^���L�^
		void WriteParameterLog();

		//�J�X�^���p�����[�^���ω�������
		const bool& CustomParamDirty()const { return m_customParamDirty; }
	};
}