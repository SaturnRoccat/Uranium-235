#pragma once
#include "../../Utils/rapidProxy.hpp"
#include "../../Component/BlockComponents/BlockComponent.hpp"
#include "../../Utils/NonOwningPointer.hpp"
#include <vector>

namespace Uranium::Blocks
{
	class Permutation
	{
	public:
		Permutation() {};
		Permutation(const char* condition, std::vector<Components::Blocks::BlockComponent*> components) : condition(condition), components(components) {};

		const char* getCondition() const { return condition; };
		std::vector<Components::Blocks::BlockComponent*> getComponents() { return components; };

		void setCondition(const char* condition) { this->condition = condition; };
		void setComponents(std::vector<Components::Blocks::BlockComponent*> components) { this->components = components; };

		rapidjson::Value compilePermutation(rapidjson::Document::AllocatorType& allocator, NonOwningPointer<ProjectSettings> pjs) const;

	private:
		const char* condition = "";
		std::vector<Components::Blocks::BlockComponent*> components = {};
	};

}