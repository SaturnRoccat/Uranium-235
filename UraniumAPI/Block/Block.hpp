#pragma once
#include <string>
#include "../RP/TextureMetadata.hpp"
#include "../RP/Texture.hpp"
#include "../Utils/Logger/Logger.hpp"
#include "../Utils/strUtil.hpp"
#include "../Utils/rapidProxy.hpp"
#include "../Utils/Category.hpp"
#include "../Utils/ItemGroups.hpp"
#include "../Utils/NonOwningPointer.hpp"
#include "../ProjectConfig.hpp"
#include "../Component/BlockComponents/BlockComponent.hpp"
#include "BlockStates.hpp"
#include "Permutations/Permutation.hpp"
#include "../Events/Event.hpp"
#include "../Utils/macros.hpp"

namespace Uranium::Blocks
{
    struct BlockMenuCategory
    {
        Catagories::Catagory category = Catagories::Catagory::none;
        ItemGroups::ItemGroup itemGroup = ItemGroups::ItemGroup::NUL;
        bool isHiddenInCommands = false;
    };

    class Block
    {
    public:

        Block(
            const char* displayName,
            const char* identifierIn,
            Version formatVersion = LATEST_FORMAT_VERSION
        ) 
        {
            std::string identifier = identifierIn;
            if (Uranium::containsUppercase(identifier))
            {
                Logs::Logger::Warning("Block identifier contains uppercase characters, this may cause issues with the game! Converting to lowercase");
                Uranium::toLowercase(identifier);
            }
            m_displayName = displayName;
			m_name = identifier;
            m_formatVersion = formatVersion;

        };

        std::string GetDisplayName() const { return m_displayName; }
        std::string GetName() const { return m_name; }

        void setCategoryData(const BlockMenuCategory& categoryData) { m_categoryData = categoryData; }
        BlockMenuCategory getCategoryData() const { return m_categoryData; }
        void addComponent(Components::Blocks::BlockComponent* component) { m_components.push_back(component); }
        void addBlockState(States::BlockState* blockState);
        void addPermutation(const Permutation& permutation) { m_permutations.push_back(permutation); }
        void addEvent(Events::Event* event) { m_events.push_back(event); }

        void compileBlock(
            ProjectSettings* projectSettings,
            rapidjson::Document* endJson

        );
        void SetTexture(const BlockTexture& texture) { m_texture = texture; };
        //void SetGeometry(class Geometry* geometry) { m_textureMetadata.SetCustomModel(geometry); };
        BlockTextureMetadata GetTextureMetadata() const { return m_textureMetadata; }
    private:
        void recursiveCompile(
            NonOwningPointer<ProjectSettings> projectSettings,
            RapidProxy::DefaultValueWriter writer
        );

        void recursiveCompileStates(
			NonOwningPointer<ProjectSettings> projectSettings,
			RapidProxy::DefaultValueWriter writer
		);

        void recursiveCompilePermutations(
            NonOwningPointer<ProjectSettings> projectSettings,
            RapidProxy::DefaultValueWriter writer
        );

        void recursiveCompileEvents(
			NonOwningPointer<ProjectSettings> projectSettings,
			RapidProxy::DefaultValueWriter writer
		);
    private:
        std::string m_displayName;
        std::string m_name;
        std::string m_texturePath;
        BlockTexture m_texture;
        BlockTextureMetadata m_textureMetadata;
        std::vector<Components::Blocks::BlockComponent*> m_components;
        std::vector<States::BlockState*> m_blockStates;
        std::vector<Permutation> m_permutations;
        std::vector<Events::Event*> m_events;
        Version m_formatVersion;
        BlockMenuCategory m_categoryData;

    };
}