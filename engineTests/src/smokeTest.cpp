#include "smokeTest.h"
#include <Windows.h>

TEST(SmokeTest, OpenAndClose) {
	int exitCode = callAndGetResult("start ../bin/Debug-windows/Sandbox/Sandbox.exe");
	//int exitCode = callAndGetResult("cd");
	EXPECT_EQ(exitCode, 0);
}
