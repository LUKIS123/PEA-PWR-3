#include "AppController.h"


AppController::AppController() {
    matrix = new ATSPMatrix();
    geneticAlgorithm = new GeneticAlgorithm();
}

AppController::~AppController() {
    clearMemory();
}

void AppController::clearMemory() {
    delete matrix;
    delete geneticAlgorithm;
}

void AppController::mainIndex() {
    ActionResult::mainMenu status = ActionResult::BACK_TO_MENU;
    while (status != ActionResult::END) {
        switch (status) {
            case ActionResult::BACK_TO_MENU:
                status = ConsoleView::mainMenu();
                break;
            case ActionResult::LOAD_DATA:
                loadDataFile();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::DISPLAY_DATA_BUFFER:
                system("CLS");
                matrix->displayMatrix();
                system("PAUSE");
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::SET_TIMEOUT:
                setTimeout();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::RUN_ALGORITHM:
                runGeneticAlgorithm();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::RUN_TESTS:
                testsMenu();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::DISPLAY_LATEST_RESULT:
                displayLatestResults();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::READ_AND_CALCULATE_SAVED_PATH:
                readPathAndDisplayCalculatedCost();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::SET_STARTING_POPULATION:
                setPopulationSize();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::SET_MUTATION_FACTOR:
                setMutationFactor();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::SET_CROSSING_FACTOR:
                setCrossFactor();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::SWITCH_CROSS_METHOD:
                switchCrossMethod();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::END:
                break;
        }
    }
    std::cout << "Exiting Application..." << std::endl;
}

// Wczytanie macierzy sasiedztwa grafu z pliku
void AppController::loadDataFile() {
    std::string fileName;
    std::cout << "Place .atsp file in executable folder and enter file name (WITHOUT .atsp):" << std::endl;
    std::cin >> fileName;
    matrix->loadFromFile(fileName);
    system("PAUSE");
}

// Ustawienie kryterium stopu
void AppController::setTimeout() {
    std::cout << "Set timeout [s]: ";
    std::cin >> timeoutSeconds;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> timeoutSeconds;
    }
    if (timeoutSeconds <= 0) {
        std::cout << "Wrong input!";
        timeoutSeconds = 120;
        system("PAUSE");
    }
}

// Metoda uruchamiajaca algorytm genetyczny
void AppController::runGeneticAlgorithm() {
    if (!matrix->exists) {
        std::cout << "MATRIX IS EMPTY" << std::endl;
        system("PAUSE");
        return;
    }

    long long start = Timer::read_QPC();
    geneticAlgorithm->mainFun(matrix->getMatrix(), matrix->getSize(), populationSize, mutationFactor, crossFactor,
                              start, timeoutSeconds);
    long long end = Timer::read_QPC();
    geneticAlgorithm->displayLatestResults();
    latestTimerResult = Timer::getMicroSecondsElapsed(start, end);
    latestTimerStart = start;
    latestRun = LatestAlgorithm::GENETIC;

    std::cout << "Timer: " << latestTimerResult / 1000 << " ms" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000000 << " s" << std::endl;
    std::cout << "Best result found after: " <<
              Timer::getMicroSecondsElapsed(start, geneticAlgorithm->bestCostFoundQPC) / 1000000 << " s" << std::endl;

    DataFileUtility::saveResultPath(resultPathFileName, geneticAlgorithm->bestPath);
    system("PAUSE");
}

// Wyswietl wynik ostatniego uruchomienia
void AppController::displayLatestResults() {
    std::cout << "algorithm: ";
    switch (latestRun) {
        case GENETIC:
            std::cout << algorithmTypes[0] << std::endl;
            break;
        default:
            return;
    }

    if (latestRun == LatestAlgorithm::GENETIC) {
        geneticAlgorithm->displayLatestResults();
        std::cout << "Best result found after: " <<
                  Timer::getSecondsElapsed(latestTimerStart, geneticAlgorithm->bestCostFoundQPC) << " s" << std::endl;
    }
    std::cout << "Timer: " << latestTimerResult << " us" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000 << " ms" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000000 << " s" << std::endl;
    system("PAUSE");
}

// Wczytaj sciezke z pliku i oblicz koszt sciezki
void AppController::readPathAndDisplayCalculatedCost() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string fileName;
    std::cout << "DEFAULT FILE: 'latest_result_path.txt', PRESS ENTER TO SKIP" << std::endl;
    std::cout << "Or place .txt file in executable folder and enter file name (WITHOUT .txt):" << std::endl;
    std::getline(std::cin, fileName);

    std::vector<int> pathVector;
    if (fileName.empty()) {
        pathVector = DataFileUtility::readPathFromFile(resultPathFileName);
    } else {
        pathVector = DataFileUtility::readPathFromFile(fileName);
    }

    if (pathVector.empty() || pathVector.size() != matrix->getSize() + 1) {
        std::cout << "Wrong input! Cannot calculate cost" << std::endl;
        system("PAUSE");
        return;
    }

    matrix->calculatePathCost(pathVector);
    system("PAUSE");
}

void AppController::setPopulationSize() {
    std::cout << "Set population size: ";
    std::cin >> populationSize;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> populationSize;
    }
    if (populationSize <= 0) {
        std::cout << "Wrong input!";
        populationSize = 500;
        system("PAUSE");
    }
}

void AppController::setMutationFactor() {
    std::cout << "Set mutation factor (1 >= m >= 0): ";
    std::cin >> mutationFactor;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a DECIMAL: ";
        std::cin >> mutationFactor;
    }
    if (mutationFactor > 1.0 || mutationFactor < 0.0) {
        std::cout << "Wrong input!";
        mutationFactor = 0.01;
        system("PAUSE");
    }
}

void AppController::setCrossFactor() {
    std::cout << "Set cross factor (1 >= c >= 0): ";
    std::cin >> crossFactor;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a DECIMAL: ";
        std::cin >> crossFactor;
    }
    if (crossFactor > 1.0 || crossFactor < 0.0) {
        std::cout << "Wrong input!";
        crossFactor = 0.8;
        system("PAUSE");
    }
}

void AppController::switchCrossMethod() {
    std::cout << "Choose cross method:" << std::endl;
    std::cout << "Order crossover => 1" << std::endl;
    std::cout << "Edge crossover => 2" << std::endl;
    int choice;
    std::cout << "Choice: ";
    std::cin >> choice;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> choice;
    }
    std::string method;
    if (choice == 1) {
        crossoverMethod = CrossMethod::OX;
        method = "Order Crossover";
    } else if (choice == 2) {
        crossoverMethod = CrossMethod::EX;
        method = "Edge Crossover";
    }

    std::cout << "Current cross method: " << method << std::endl;
    system("PAUSE");
}

void AppController::testsMenu() {
    if (!matrix->exists) {
        std::cout << "MATRIX IS EMPTY" << std::endl;
        system("PAUSE");
        return;
    }
    system("CLS");
    std::cout << "AUTOMATIC TESTS Initialization..." << std::endl;
    std::cout << std::endl << "Set number of tests..." << std::endl;
    std::cout << "Tests: ";
    std::cin >> testNumber;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> testNumber;
    }

    ActionResult::automaticTestsMenu status = ActionResult::MENU_TEST;
    while (status != ActionResult::END_TEST) {
        switch (status) {
            case ActionResult::MENU_TEST:
                status = ConsoleView::automaticTestsMenu();
                break;
            case ActionResult::TEST_GENETIC:
                testGeneticAlgorithm();
                status = ActionResult::MENU_TEST;
                break;
            case ActionResult::END_TEST:
                break;
        }
    }
}

// Metoda sluzaca testom algorytmu genetycznego
void AppController::testGeneticAlgorithm() {

}
