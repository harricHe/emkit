#include "unity_fixture.h"

static void runAllTests(void)
{
    //RUN_TEST_GROUP(graph);
    //RUN_TEST_GROUP(list);
    //RUN_TEST_GROUP(map);
    //RUN_TEST_GROUP(queue);
    //RUN_TEST_GROUP(priority_queue);
    //RUN_TEST_GROUP(ringbuffer);
    RUN_TEST_GROUP(fixed_ringbuffer);
    //RUN_TEST_GROUP(stack);
    //RUN_TEST_GROUP(tree);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, runAllTests);
}

