#include "Block.hpp"
#include "../Utils/macros.hpp"
void Uranium::Blocks::Block::addBlockState(States::BlockState* blockState)
{
	size_t permutationCount = blockState->GetPermutationSize();
	for (auto state : this->m_blockStates) {
		// make sure we dont test against itself
		if (state != blockState)
			permutationCount *= state->GetPermutationSize();
	}

	if (permutationCount > 65536)
		Logs::Logger::Error("Too many states for block {}!", this->m_name);

	this->m_blockStates.push_back(blockState);

}

void Uranium::Blocks::Block::compileBlock(ProjectSettings* projectSettings, rapidjson::Document* endJson)
{
	// Sets up the json
	endJson->SetObject();
	rapidjson::Document::AllocatorType& allocator = endJson->GetAllocator();
	CStrWithLength str = m_formatVersion.ToString();
	endJson->AddMember("format_version", str.toValue(), allocator);

	rapidjson::Value blockObj(rapidjson::kObjectType);
	blockObj.AddMember("description", rapidjson::Value(rapidjson::kObjectType), allocator);
	rapidjson::Value& description = blockObj["description"];

	CStrWithLength NameWithNamespace = projectSettings->getNameWithNamespace(this->m_name);
	description.AddMember("identifier", NameWithNamespace.toValue(), allocator);
	description.AddMember("menu_catagory", rapidjson::Value(rapidjson::kObjectType), allocator);

	if (this->m_categoryData.category != Catagories::Catagory::none)
	{
		rapidjson::Value& menuCategory = description["menu_catagory"];
		auto catData = Catagories::catagoryToString(this->m_categoryData.category);
		menuCategory.AddMember("category", catData.toValue(), allocator);
		if (this->m_categoryData.itemGroup != ItemGroups::ItemGroup::NUL)
		{
			auto itemGroup = ItemGroups::itemGroupToString(this->m_categoryData.itemGroup);
			menuCategory.AddMember("group", itemGroup.toValue(), allocator);
		}
		if (this->m_categoryData.isHiddenInCommands)
			menuCategory.AddMember("is_hidden_in_commands", true, allocator);

	}

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

	if (this->m_events.size() > 0)
	{
		this->recursiveCompileEvents(projectSettings, {&blockObj, allocator});
	}

	this->recursiveCompile(projectSettings, RapidProxy::DefaultValueWriter(&components, allocator));


	endJson->AddMember("minecraft:block", blockObj, allocator);

	DEBUGCODE(
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		endJson->Accept(writer);
		Logs::Logger::Debug("Block JSON: {}", buffer.GetString());
	)
}

void Uranium::Blocks::Block::recursiveCompile(NonOwningPointer<ProjectSettings> projectSettings, RapidProxy::DefaultValueWriter writer)
{
	// Compute enabled experiments
	unsigned char enabledExperiments = 0;
	for (auto exp : projectSettings.Get()->experimentals)
		enabledExperiments |= (unsigned char)exp.first;

	for (auto comp : this->m_components)
	{
#ifndef BYPASSCHECKS
		if ((comp->GetReqiredExperimentsBitfield() & enabledExperiments) != comp->GetReqiredExperimentsBitfield())
		{
			Logs::Logger::NonFatalError("Component {} requires experiments that are not enabled!", comp->GetName());
			continue;
		}
#endif //BYPASSCHECKS

		comp->CompileComponent(writer, projectSettings);
	}
}

void Uranium::Blocks::Block::recursiveCompileStates(NonOwningPointer<ProjectSettings> projectSettings, RapidProxy::DefaultValueWriter writer)
{
	for (auto state : this->m_blockStates)
	{
		state->WriteState(writer);
	}
}

void Uranium::Blocks::Block::recursiveCompilePermutations(NonOwningPointer<ProjectSettings> projectSettings, RapidProxy::DefaultValueWriter writer)
{
	DVAP()
	for (auto permutation : this->m_permutations)
	{
		rapidjson::Value permutationData = permutation.compilePermutation(allocator, projectSettings);
		data->PushBack(permutationData, allocator);
	}
}

void Uranium::Blocks::Block::recursiveCompileEvents(NonOwningPointer<ProjectSettings> projectSettings, RapidProxy::DefaultValueWriter writer)
{
	DVAP()
	rapidjson::Value events(rapidjson::kObjectType);
	for (auto e : this->m_events)
	{
		e->CompileEvent({ &events, allocator });
	}
	data->AddMember("events", events, allocator);
}
