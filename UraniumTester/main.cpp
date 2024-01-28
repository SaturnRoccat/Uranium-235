#define ENABLE_TEST_COMPONENTS
#define BYPASSCHECKS // This is only for testing, do not use this in production code!
#include "Uranium235.hpp"
#include <iostream>

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

    Uranium::Blocks::Block* block = new Uranium::Blocks::Block("Test Block!", "testIdentifier_to_Try_andBreak_myCode", Uranium::Version(1,20,30));
    block->addBlockState(
        new Uranium::Blocks::States::BlockStateInteger("local:testInt", 0, 12)
    );
    block->addBlockState(
        new Uranium::Blocks::States::BlockStateInteger("local:testInt2", 0, 12)
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

    Uranium::Events::FixedResponseEvent* eventOne =
        new Uranium::Events::FixedResponseEvent(
            "uranium:testingEventOne"
        );
    eventOne->AddEventComponent(
        new Uranium::Components::Events::TestEventComponentOne()
    );

    block->addEvent(eventOne);

    Uranium::Events::SequencedEvent* eventTwo = new Uranium::Events::SequencedEvent(
        "uranium:testingEventTwo"
    );
    
    eventTwo->AddEventSequencePart(
        new Uranium::Events::SequencedEvent::SequencedEventPart(
            new Uranium::Events::EventDataPair(
                "self",
                "uranium:testingEventOne"
            )
        )
    );
    eventTwo->AddEventSequencePart(
        new Uranium::Events::SequencedEvent::SequencedEventPart(
            new Uranium::Events::EventDataPair(
                "self",
                "uranium:testingEventThree"
            )
        )
    );
    block->addEvent(eventTwo);

    rapidjson::Document json;
    block->compileBlock(uranium->getProjectSettings(), &json);
    


    return 0;
}