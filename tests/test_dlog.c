#include "dlog.h"
#include "unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/* Smoke test only: proves the Unity harness itself is wired up correctly.
 * DLOG()'s actual job is writing to stderr, which TEST_ASSERT_* can't
 * observe here -- testing the real output format needs a stderr capture
 * strategy, which is worth its own discussion before writing more of
 * these. */
static void test_dlog_does_not_crash(void)
{
	DLOG("smoke test message %d", 1);
	TEST_PASS();
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_dlog_does_not_crash);
	return UNITY_END();
}
