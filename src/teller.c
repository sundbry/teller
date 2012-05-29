#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "teller.h"
#include "teller-listen.h"
#include "teller-hyp.h"
#include "teller-action.h"

void teller_init(TellerState *teller_state) {
	teller_state->logfd = open(teller_state->logPath, O_WRONLY | O_CREAT | O_TRUNC, 0660);
	if(teller_state->logfd == -1) {
		fprintf(stderr, "Error opening log file: '%s'\n", 
			teller_state->logPath);
		exit(EXIT_FAILURE);
	}

	dup2(teller_state->logfd, STDERR_FILENO);

	teller_load_actions(teller_state);

	teller_state->config = cmd_ln_init(NULL, ps_args(), TRUE,
			     "-hmm", TELLER_HMM,
			     "-lm", TELLER_LM,
			     "-dict", TELLER_DICT,
			     NULL);

	if (teller_state->config == NULL) {
		fprintf(stderr, "Error initializing pocketsphinx config\n");
		exit(EXIT_FAILURE);
	}
		
	teller_state->ps = ps_init(teller_state->config);
	if (teller_state->ps == NULL) {
		fprintf(stderr, "Error initializing pocketsphinx\n");
		exit(EXIT_FAILURE);
	}
}

void teller_deinit(TellerState *teller_state) {
	printf("Deinitializing...\n");
	ps_free(teller_state->ps);	
	teller_unload_actions(teller_state);
	close(teller_state->logfd);
}
