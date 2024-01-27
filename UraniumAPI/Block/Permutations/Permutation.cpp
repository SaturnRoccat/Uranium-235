#include "Permutation.hpp"

rapidjson::Value Uranium::Blocks::Permutation::compilePermutation(rapidjson::Document::AllocatorType& allocator, NonOwningPointer<ProjectSettings> pjs) const
{
	rapidjson::Value permutationObject(rapidjson::kObjectType);
	rapidjson::Value name(this->condition, allocator);
	permutationObject.AddMember("condition", name, allocator);

	permutationObject.AddMember("components", rapidjson::Value(rapidjson::kObjectType), allocator);
	auto& componentsObj = permutationObject["components"];

	for (auto component : this->components)
	{
		component->CompileComponent({ &componentsObj, allocator }, pjs);
	}

	return permutationObject;
}