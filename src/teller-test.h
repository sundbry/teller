#ifndef TELLER_TEST_H
#define TELLER_TEST_H

#include "teller.h"

#define TEST_DIR BASEDIR "/test"

void teller_run_tests(TellerState *ts);
int teller_test_file(const char *path, const char *expectActionStr);

#endif
