#include "teller.h"
#include "teller-listen.h"

int main(int argc, char *argv[]) {
	TellerState teller_state;

	teller_state.logPath = "teller.log";
	if(argc == 2) {
		teller_state.remoteName = argv[1];
	}
	else {
		teller_state.remoteName = TELLER_DEFAULT_REMOTE;
	}

	teller_init(&teller_state);

	teller_listen_mic(&teller_state);

	teller_deinit(&teller_state);

	return EXIT_SUCCESS;
}

