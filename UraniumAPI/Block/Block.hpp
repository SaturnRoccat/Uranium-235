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

namespace Uranium
{
    struct BlockMenuCategory
    {
        Catagories::Catagory category;
        ItemGroups::ItemGroup itemGroup = ItemGroups::ItemGroup::NUL;
        bool isHiddenInCommands = false;
    };

    class Block
    {
    public:

        Block(
            const char* displayName,
            const char* identifierIn
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
        };

        std::string GetDisplayName() const { return m_displayName; }
        std::string GetName() const { return m_name; }

        void setCategoryData(const BlockMenuCategory& categoryData) { m_categoryData = categoryData; }
        BlockMenuCategory getCategoryData() const { return m_categoryData; }
        void addComponent(Components::BlockComponent* component) { m_components.push_back(component); }
        void addBlockState(BlockState* blockState);
        void addPermutation(const Permutation& permutation) { m_permutations.push_back(permutation); }

        void compileBlock(
            NonOwningPointer<ProjectSettings> projectSettings,
            rapidjson::Document* endJson

        ); // TODO: Implement this
        void SetTexture(const BlockTexture& texture) { m_texture = texture; };
        void SetGeometry(class Geometry* geometry) { m_textureMetadata.SetCustomModel(geometry); };
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
    private:
        std::string m_displayName;
        std::string m_name;
        std::string m_texturePath;
        BlockTextureMetadata m_textureMetadata;
        BlockTexture m_texture;
        BlockMenuCategory m_categoryData;
        std::vector<Components::BlockComponent*> m_components;
        std::vector<BlockState*> m_blockStates;
        std::vector<Permutation> m_permutations;
        
    };
}