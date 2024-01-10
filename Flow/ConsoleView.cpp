#include "ConsoleView.h"


ActionResult::mainMenu ConsoleView::mainMenu() {
    system("CLS");
    std::cout << "Menu: Choose from given options..." << std::endl;
    std::cout << "1 => Load input from file" << std::endl;
    std::cout << "2 => Display matrix" << std::endl;
    std::cout << "3 => Display latest result" << std::endl;
    std::cout << "4 => Set timeout" << std::endl;
    std::cout << "5 => Set starting Population size" << std::endl;
    std::cout << "6 => Set Mutation factor" << std::endl;
    std::cout << "7 => Set Crossing factor" << std::endl;
    std::cout << "8 => RUN GENETIC ALGORITHM..." << std::endl;
    std::cout << "9 => Set Cross Method" << std::endl;
    std::cout << "10 => Auto Tests" << std::endl;
    std::cout << "11 => Read and calculate path cost" << std::endl;
    std::cout << "0 => Exit" << std::endl;

    std::cout << "Choice: ";
    int next;
    std::cin >> next;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> next;
    }
    std::cout << std::endl;
    switch (next) {
        case 1:
            return ActionResult::LOAD_DATA;
        case 2:
            return ActionResult::DISPLAY_DATA_BUFFER;
        case 3:
            return ActionResult::DISPLAY_LATEST_RESULT;
        case 4:
            return ActionResult::SET_TIMEOUT;
        case 5:
            return ActionResult::SET_STARTING_POPULATION;
        case 6:
            return ActionResult::SET_MUTATION_FACTOR;
        case 7:
            return ActionResult::SET_CROSSING_FACTOR;
        case 8:
            return ActionResult::RUN_ALGORITHM;
        case 9:
            return ActionResult::SWITCH_CROSS_METHOD;
        case 10:
            return ActionResult::RUN_TESTS;
        case 11:
            return ActionResult::READ_AND_CALCULATE_SAVED_PATH;
        case 0:
            return ActionResult::END;
        default:
            return ActionResult::BACK_TO_MENU;
    }
}

ActionResult::automaticTestsMenu ConsoleView::automaticTestsMenu() {
    system("CLS");
    std::cout << "AUTOMATIC TESTS: Choose from given options..." << std::endl;
    std::cout << "1 => Test" << std::endl;
    std::cout << "2 => Test" << std::endl;
    std::cout << "0 => Exit" << std::endl;

    std::cout << "Choice: ";
    int next = 0;
    std::cin >> next;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> next;
    }
    switch (next) {
        case 0:
            return ActionResult::automaticTestsMenu::END_TEST;
        case 1:
            return ActionResult::automaticTestsMenu::TEST_GENETIC;
        case 2:
            return ActionResult::automaticTestsMenu::TEST_GENETIC;
        default:
            return ActionResult::automaticTestsMenu::MENU_TEST;
    }
}
