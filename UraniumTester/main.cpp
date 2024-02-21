#define ENABLE_TEST_COMPONENTS
#define BYPASSCHECKS // This is only for testing, do not use this in production code!
#include "Uranium235.hpp"
#include "Utils/Timer.hpp"
#include <iostream>

Uranium::Blocks::Block* generateBlock(int index)
{
    // "testidentifier_to_try_andbreak_mycode"
    Uranium::Blocks::Block* block = new Uranium::Blocks::Block("Test Block!", "block", Uranium::Version(1, 20, 30));
    //Uranium::Blocks::Block* block = new Uranium::Blocks::Block("Test Block!", "testIdentifier_to_Try_andBreak_myCode" + index, Uranium::Version(1, 20, 30));

    block->addBlockState(
        new Uranium::Blocks::States::BlockStateInteger("local:testInt2", 0, 5)
    );
    block->addBlockState(
        new Uranium::Blocks::States::BlockStateBoolean("local:testBool")
    );

    block->setCategoryData({
        Uranium::Catagories::Catagory::construction,
        Uranium::ItemGroups::ItemGroup::axe,
        true
        });

    block->addComponent(
        new Uranium::Components::Blocks::TestBlockComponent()
    );

    block->addPermutation(
        Uranium::Blocks::Permutation(
            "thing",
            {
                new Uranium::Components::Blocks::TestBlockComponent2()
            }
        )
    );
    return block;
}

int main()
{
    Uranium::Uranium235* uranium = new Uranium::Uranium235(new Uranium::ProjectSettings(
        Uranium::Version(1, 0, 0),
        "Uranium Tester",
        "This is a project made to test the Uranium API",
        "Duckos",
        "UraniumOutput/",
        "uranium",
        {
            {Uranium::Experimentals::BetaAPIs, true}
        }
    ));
    const int blockCount = 1600000;
    std::vector<Uranium::Blocks::Block*> block;
    block.reserve(blockCount);
    double genTime = 0;
    //Uranium::Blocks::Block* block;
    {
        Uranium::ScopedTimer timerScoped("Block Generation. In worst case");
        Uranium::AverageTimer timer("Block Creation. In worst case", blockCount);
        for (int i = 0; i < blockCount; i++)
        {
            block.push_back( generateBlock(i));
		}
    }

    rapidjson::Document json;
    {
        Uranium::AverageTimer timer("Block Compilation and writing. In worst case", blockCount);
        for (int i = 0; i < blockCount; i++)
        {
            block[i]->compileBlock(uranium->getProjectSettings(), &json);
		}
	}


    DEBUGCODE(
        {
            Uranium::ScopedTimer timer("File Writing.");
            rapidjson::StringBuffer buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            json.Accept(writer);
            Uranium::Logs::Logger::Debug("JSON: {}", buffer.GetString());
            // write to file
            std::ofstream file("uranium.json");
            file << buffer.GetString();
            file.close();
        }
    )

    {
        Uranium::ScopedTimer timer("Block Deletion.");
        for (int i = 0; i < blockCount; i++)
        {
			delete block[i];
		}
    }
    return 0;
}