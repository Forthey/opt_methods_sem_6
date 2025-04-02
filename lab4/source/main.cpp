#include <cstdlib>


#include "Task.h"


int main() {
    Task::gradientMethodCalcs();
    Task::newtonMethodCalcs();
    Task::hookeJeevesCalcs();

    Task::checkForOrthogonalityInGradient();

    Task::gradientWithNewtonCalcs();
    Task::hookeJeevesWithNewtonCalcs();
    return EXIT_SUCCESS;
}
