#pragma once
#include<string>
#include"KuroEngine.h"

namespace KuroEngine
{
	class ImportHeader
	{
	public:
		const std::string dir;
		const std::string fileName;
		const std::string ext;

		ImportHeader(const std::string& Dir, const std::string& FileName)
			:dir(Dir), fileName(FileName), ext("." + GetExtension(fileName)) {}

		std::string GetPath()const { return dir + fileName; }
		std::string GetFileNameWithoutExt()const	//�t�@�C�����i�g���q�Ȃ��j
		{
			auto result = fileName;
			return result.erase(result.size() - ext.size());
		}
	};
}