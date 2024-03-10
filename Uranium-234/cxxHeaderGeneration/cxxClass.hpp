#pragma once
#include <string>
#include "cxxType.hpp"
#include "cxxEnum.hpp"
#include "cxxFunction.hpp"
#include <vector>
#include <sstream>

namespace Uranium
{
    class CxxClass
    {
    public:
        CxxClass(const std::string& name, const std::vector<CxxType*>& members = {}, const std::string& parent = "", const std::string& namespaceName = "")
            : m_name(name), m_parent(parent), m_namespace(namespaceName), m_members(members) {}
        CxxClass() = default;
        CxxClass(const CxxClass&) = default;
        CxxClass(CxxClass&&) = default;
        CxxClass& operator=(const CxxClass&) = default;

        ~CxxClass()
        {
			for (auto& member : m_members)
				delete member;
		}

        void setName(const std::string& name) { m_name = name; }
        void setParent(const std::string& parent) { m_parent = parent; }
        void setNamespace(const std::string& namespaceName) { m_namespace = namespaceName; }
        void setMembers(const std::vector<CxxType*>& members) { m_members = members; }
        void addMember(CxxType* member) { m_members.push_back(member); }
        void setMembersForParent(const std::vector<std::string>& members) { m_membersForParent = members; }
        void addMemberForParent(const std::string& member) { m_membersForParent.push_back(member); }
        void setFunctions(const std::vector<CxxFunction>& functions) { m_functions = functions; }
        void addFunction(const CxxFunction& function) { m_functions.push_back(function); }

        const std::string& getName() const { return m_name; }
        const std::string& getParent() const { return m_parent; }
        const std::string& getNamespace() const { return m_namespace; }
        const std::vector<CxxType*>& getMembers() const { return m_members; }
        const std::vector<std::string>& getMembersForParent() const { return m_membersForParent; }
        const std::vector<CxxFunction>& getFunctions() const { return m_functions; }

        std::string& getName() { return m_name; }
        std::string& getParent() { return m_parent; }
        std::string& getNamespace() { return m_namespace; }
        std::vector<CxxType*>& getMembers() { return m_members; }
        std::vector<std::string>& getMembersForParent() { return m_membersForParent; }
        std::vector<CxxFunction>& getFunctions() { return m_functions; }

        // Debug funcs
        void printMembers() const;

        std::string makeCxxClassDef() const;
    private:
        std::vector<CxxType*> m_members = {};
        std::vector<std::string> m_membersForParent = {};
        std::vector<CxxFunction> m_functions = {};
        std::string m_name = "";
        std::string m_parent = "";
        std::string m_namespace = "";

    };

};