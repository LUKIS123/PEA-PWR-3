#ifndef PEA_PWR_3_ACTIONRESULT_H
#define PEA_PWR_3_ACTIONRESULT_H

class ActionResult {
public:
    enum mainMenu {
        END = 0,
        LOAD_DATA = 1,
        DISPLAY_DATA_BUFFER = 2,
        SET_TIMEOUT = 3,
        SET_STARTING_POPULATION = 4,
        SET_MUTATION_FACTOR = 5,
        SET_CROSSING_FACTOR = 6,
        RUN_ALGORITHM = 7,
        RUN_TESTS = 8,
        BACK_TO_MENU = 9,
        DISPLAY_LATEST_RESULT = 10,
        READ_AND_CALCULATE_SAVED_PATH = 11,
        SWITCH_CROSS_METHOD = 12
    };

    enum automaticTestsMenu {
        END_TEST = 0,
        MENU_TEST = 1,
        TEST_GENETIC_OX = 2,
        TEST_GENETIC_EX = 3,
    };
};

#endif //PEA_PWR_3_ACTIONRESULT_H
