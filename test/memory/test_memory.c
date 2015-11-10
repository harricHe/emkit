#include "unity_fixture.h"

static void runAllTests(void)
{
    //RUN_TEST_GROUP(memorypool);
    RUN_TEST_GROUP(fixed_memorypool);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, runAllTests);
}

