#include "Permutation.hpp"

rapidjson::Value Uranium::Permutation::compilePermutation(rapidjson::Document::AllocatorType& allocator, NonOwningPointer<ProjectSettings> pjs) const
{
	rapidjson::Value permutationObject(rapidjson::kObjectType);
	permutationObject.AddMember("condition", RJ_STL_S(this->condition), allocator);

	permutationObject.AddMember("components", rapidjson::Value(rapidjson::kObjectType), allocator);
	auto& componentsObj = permutationObject["components"];

	for (auto component : this->components)
	{
		component->CompileComponent({ &componentsObj, allocator }, pjs);
	}

	return permutationObject;
}