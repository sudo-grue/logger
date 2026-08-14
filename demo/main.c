#include "dlog.h"

static void do_work(int value)
{
	DLOG("processing value=%d", value);
}

int main(void)
{
	DLOG("starting logger smoke test");
	do_work(42);
	DLOG("smoke test complete");

	return 0;
}
