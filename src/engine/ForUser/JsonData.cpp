#include "JsonData.h"
#include"KuroEngine.h"
#include<fstream>

void KuroEngine::JsonData::Export(std::string arg_dir, std::string arg_name, std::string arg_ext, bool arg_safeRewrite)
{
	//�����
	if (m_jsonData.empty())
	{
		AppearMessageBox(arg_name + arg_ext, "����json�t�@�C������ہB");
		return;
	}

	//�㏑���ۑ��̊m�F
	if (arg_safeRewrite && ExistFile(arg_dir + arg_name + arg_ext))
	{
		if (!AppearMessageBoxYN(arg_name + arg_ext, "�����̃t�@�C�������邯�Ǐ㏑�����ėǂ��H"))return;
	}

	auto serialized = m_jsonData.dump(2);
	std::ofstream file;
	file.open(arg_dir + arg_name + arg_ext, std::ios::out);
	file << serialized << std::endl;
	file.close();
}

void KuroEngine::JsonData::Import(std::string arg_dir, std::string arg_fileName)
{
	std::ifstream file;
	file.open(arg_dir + arg_fileName);
	if (file.fail())
	{
		AppearMessageBox(arg_fileName, "�t�@�C���J���̂Ɏ��s����������B");
		return;
	}
	file >> m_jsonData;
	file.close();
}