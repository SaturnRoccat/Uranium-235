#include "Uranium235.hpp"
#include <iostream>

int main()
{
    Uranium::Uranium235 uranium(new Uranium::ProjectSettings(
        Uranium::Version(1, 0, 0),
        "Uranium Tester",
        "This is a project made to test the Uranium API",
        "Duckos",
        "UraniumOutput/",
        {
            {Uranium::Experimentals::BetaAPIs, true}
        }
    ));

    Uranium::Block* block = new Uranium::Block("Test Block!","testIdentifier_to_Try_andBreak_myCode");


    return 0;
}