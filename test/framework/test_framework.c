#include "unity_fixture.h"

static void runAllTests(void)
{
    RUN_TEST_GROUP(command);
    RUN_TEST_GROUP(shell);
    //RUN_TEST_GROUP(observer);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, runAllTests);
}

