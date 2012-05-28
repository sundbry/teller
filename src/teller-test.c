#include "teller.h"
#include "teller-listen.h"
#include "teller-test.h"

static TellerState *ts;

int main(int argc, char *argv[]) {
	TellerState teller_state;

	teller_state.logPath = "teller.log";
	teller_state.remoteName = "test-remote";

	teller_init(&teller_state);

	teller_run_tests(&teller_state);

	teller_deinit(&teller_state);

	return EXIT_SUCCESS;
}

void teller_run_tests(TellerState *teller_state) {
	ts = teller_state;

	teller_test_file(TEST_DIR "/ryan-sundberg/television-on-1.raw",
		"ON");
}

int teller_test_file(const char *path, const char *expectActionStr) {

	teller_listen_file(ts, path);

	return 0;
}

