#pragma once
#include <gtest/gtest.h>

#define private public
#define protected public

#include "core/application.h"

#include <cstdio>

int callAndGetResult(const char* cmd) {
	FILE* fd = _popen(cmd, "r");
	
	return _pclose(fd);
}

