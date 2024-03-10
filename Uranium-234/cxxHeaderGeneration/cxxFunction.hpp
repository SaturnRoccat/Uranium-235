#pragma once
#include <xstring>
#include <vector>
#include "cxxType.hpp"
#include <sstream>

namespace Uranium
{
	class CxxFunction
	{
	private:
		std::string m_name;
		CxxType m_returnType;
		std::vector<CxxType> m_parameters;
		std::string m_namespace;
		std::string m_class;
		std::vector<std::string> m_body;
		bool m_isVirtual;
		bool m_isOverride;
		bool m_isConst;
		bool m_isStatic;
		bool m_isInline;
		bool m_hasSeperateDecAndDef;
	public:
		CxxFunction(
			const std::string& name,
			const CxxType& returnType,
			bool m_hasSeperateDecAndDef = false,
			const std::vector<CxxType>& parameters = {},
			const std::string namespace_ = "",
			const std::string class_ = "",
			const std::vector<std::string>& body = {},
			bool isVirtual = false,
			bool isOverride = false,
			bool isConst = false,
			bool isStatic = false,
			bool isInline = false)
			 :
				m_name(name),
				m_returnType(returnType),
				m_parameters(parameters),
				m_body(body),
				m_isVirtual(isVirtual),
				m_isOverride(isOverride),
				m_isConst(isConst),
				m_isStatic(isStatic),
				m_isInline(isInline),
				m_namespace(namespace_),
				m_hasSeperateDecAndDef(m_hasSeperateDecAndDef)
		{
		}

		CxxFunction() : m_name(""), m_returnType(CxxType()), m_parameters({}), m_body({}), m_isVirtual(false), m_isOverride(false), m_isConst(false), m_isStatic(false), m_isInline(false), m_hasSeperateDecAndDef(false)
		{
		}
		



		void addFunctionLine(const std::string& line)
		{
			m_body.push_back(line);
		}

		void removeLastFunctionLine()
		{
			m_body.pop_back();
		}

		void removeFunctionLine(int index)
		{
			m_body.erase(m_body.begin() + index);
		}

		void clearFunctionBody()
		{
			m_body.clear();
		}

		std::string& getLine(int index)
		{
			return m_body[index];
		}

		std::string& getLastLine()
		{
			return m_body.back();
		}

		const std::vector<std::string>& getBodyRaw() const
		{
			return m_body;
		}

		std::string compileBody() const
		{
			std::stringstream ss;
			for (const auto& line : m_body)
			{
				ss << line << std::endl;
			}
			return ss.str();
		}

		std::string getName() const
		{
			return m_name;
		}

		CxxType getReturnType() const
		{
			return m_returnType;
		}
		CxxType& getReturnType()
		{
			return m_returnType;
		}

		const std::vector<CxxType>& getParameters() const
		{
			return m_parameters;
		}
		std::vector<CxxType>& getParameters()
		{
			return m_parameters;
		}

		bool isVirtual() const
		{
			return m_isVirtual;
		}

		bool isOverride() const
		{
			return m_isOverride;
		}

		bool isConst() const
		{
			return m_isConst;
		}

		bool isStatic() const
		{
			return m_isStatic;
		}

		bool isInline() const
		{
			return m_isInline;
		}

		bool hasSeperateDecAndDef() const
		{
			return m_hasSeperateDecAndDef;
		}

		void setName(const std::string& name)
		{
			m_name = name;
		}

		void setReturnType(const CxxType& returnType)
		{
			m_returnType = returnType;
		}

		void setParameters(const std::vector<CxxType>& parameters)
		{
			m_parameters = parameters;
		}

		void setVirtual(bool isVirtual)
		{
			m_isVirtual = isVirtual;
		}

		void setOverride(bool isOverride)
		{
			m_isOverride = isOverride;
		}

		void setConst(bool isConst)
		{
			m_isConst = isConst;
		}

		void setStatic(bool isStatic)
		{
			m_isStatic = isStatic;
		}

		void setInline(bool isInline)
		{
			m_isInline = isInline;
		}

		void setHasSeperateDecAndDef(bool hasSeperateDecAndDef)
		{
			m_hasSeperateDecAndDef = hasSeperateDecAndDef;
		}

		void addParameter(const CxxType& parameter)
		{
			m_parameters.push_back(parameter);
		}

		bool hasNamespace() const
		{
			return m_namespace != "";
		}

		std::string getNamespace() const
		{
			return m_namespace;
		}

		void setNamespace(const std::string& namespace_)
		{
			m_namespace = namespace_;
		}

		bool hasClass() const
		{
			return m_class != "";
		}

		std::string getClass() const
		{
			return m_class;
		}

		void setClass(const std::string& class_)
		{
			m_class = class_;
		}

		std::string generateCxxDec() const;
		std::string generateCxxDef(bool skipFunctionDef = false) const;

	};
}