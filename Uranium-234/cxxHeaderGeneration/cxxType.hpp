#pragma once
#include <string>
#include <functional>
#include "cxxFiles.hpp"
namespace Uranium
{
	void registerCustomCxxTypeConversion(const std::string& type, const std::string& conversion);
	std::string_view getCustomCxxTypeConversion(const std::string& type);

	void registerCustomCxxTypeCompiler(const std::string& type, std::function<std::string(const std::string&)> convFunc);
	std::function<std::string(const std::string&)> getCustomCxxTypeCompiler(const std::string& type);

	void registerRequiredInclude(const std::string& include);
	std::string_view getRequiredInclude(const std::string& include);


	class CxxTypeData : public CxxGenericTypeData
	{
	private:
		std::string compiledTypeString = "";
	public:
		CxxTypeData(const std::string& typeName, const std::vector<std::string>& requiredIncludes = {}) : CxxGenericTypeData({ requiredIncludes }), compiledTypeString(typeName) {}
	};

	class CxxType
	{
	private:
		CxxGenericTypeData m_GenericData;
		std::string m_typeName = "";
	public:
		CxxType(const std::string& typeName, const std::vector<std::string>& requiredIncludes = {}) : m_typeName(typeName), m_GenericData({ requiredIncludes }) {}
		CxxType() : m_typeName("void"), m_GenericData({}) {}

		CxxGenericTypeData getGenericData() const { return m_GenericData; }
		CxxGenericTypeData& getGenericData() { return m_GenericData; }

		std::string getTypeName() const { return m_typeName; }
		std::string& getTypeName() { return m_typeName; }

		void setTypeName(const std::string& name) { m_typeName = name; }
		void setGenericData(const CxxGenericTypeData& data) { m_GenericData = data; }

		void addRequiredInclude(const std::string& include) { m_GenericData.requiredIncludes.push_back(include); }

		CxxTypeData compileType() const { return CxxTypeData(m_typeName, m_GenericData.requiredIncludes); }
	};

	class CxxMember : public CxxType
	{
	private:
		std::string m_Name;
		std::string m_defaultValue = "";
	public:
		CxxMember(
			const std::string& memberName, const std::string& defaultVal = "", const std::string & typeName = "void", const std::vector<std::string>&requiredIncludes = {}
		) : CxxType(typeName, requiredIncludes), m_Name(memberName), m_defaultValue(defaultVal) {}
	};
}