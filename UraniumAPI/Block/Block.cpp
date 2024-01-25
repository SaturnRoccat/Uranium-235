#include "Block.hpp"

void Uranium::Block::addBlockState(BlockState* blockState)
{
	size_t permutationCount = blockState->GetPermutationSize();
	for (auto state : this->m_blockStates)
		permutationCount *= state->GetPermutationSize();

	if (permutationCount > 65536)
		Logs::Logger::Error("Too many states for block {}!", this->m_name);

	this->m_blockStates.push_back(blockState);

}

void Uranium::Block::compileBlock(NonOwningPointer<ProjectSettings> projectSettings, rapidjson::Document* endJson)
{
	// Sets up the json
	endJson->SetObject();
	rapidjson::Document::AllocatorType& allocator = endJson->GetAllocator();
	endJson->AddMember("format_version", RJ_STL_S(projectSettings.Get()->version.ToString()), allocator);

	rapidjson::Value blockObj(rapidjson::kObjectType);
	blockObj.AddMember("description", rapidjson::Value(rapidjson::kObjectType), allocator);
	rapidjson::Value& description = blockObj["description"];

	description.AddMember("identifier", RJ_STL_S(projectSettings.Get()->getNameWithNamespace(this->m_name)), allocator);
	description.AddMember("menu_catagory", rapidjson::Value(rapidjson::kObjectType), allocator);

	blockObj.AddMember("components", rapidjson::Value(rapidjson::kObjectType), allocator);
	rapidjson::Value& components = blockObj["components"];


	// Compile States if there are any
	if (this->m_blockStates.size() > 0)
	{
		description.AddMember("states", rapidjson::Value(rapidjson::kObjectType), allocator);
		rapidjson::Value& states = description["states"];
		this->recursiveCompileStates(projectSettings, RapidProxy::DefaultValueWriter(&states, allocator));
	}

	if (this->m_permutations.size() > 0)
	{
		blockObj.AddMember("permutations", rapidjson::Value(rapidjson::kArrayType), allocator);
		rapidjson::Value& permutations = blockObj["permutations"];
		this->recursiveCompilePermutations(projectSettings, RapidProxy::DefaultValueWriter(&permutations, allocator));
	}

	this->recursiveCompile(projectSettings, RapidProxy::DefaultValueWriter(&components, allocator));


	endJson->AddMember("minecraft:block", blockObj, allocator);
}

void Uranium::Block::recursiveCompile(NonOwningPointer<ProjectSettings> projectSettings, RapidProxy::DefaultValueWriter writer)
{
	for (auto comp : this->m_components)
	{
		comp->CompileComponent(writer, projectSettings);
	}
}

void Uranium::Block::recursiveCompileStates(NonOwningPointer<ProjectSettings> projectSettings, RapidProxy::DefaultValueWriter writer)
{
	for (auto state : this->m_blockStates)
	{
		state->WriteState(writer);
	}
}

void Uranium::Block::recursiveCompilePermutations(NonOwningPointer<ProjectSettings> projectSettings, RapidProxy::DefaultValueWriter writer)
{
	DVAP()
	for (auto permutation : this->m_permutations)
	{
		rapidjson::Value permutationData = permutation.compilePermutation(allocator, projectSettings);
		data->PushBack(permutationData, allocator);
	}
}
