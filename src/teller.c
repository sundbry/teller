#include "teller.h"

#define SAMPLEDIR "/home/ryan/projects/teller/lib/pocketsphinx-0.7/test/data"

int main(int argc, char *argv[]) {
	TellerState teller_state;

	teller_init(&teller_state);

	teller_listen(&teller_state);

	return EXIT_SUCCESS;
}

void teller_init(TellerState *teller_state) {
	teller_state->config = cmd_ln_init(NULL, ps_args(), TRUE,
			     "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
			     "-lm", MODELDIR "/lm/en/turtle.DMP",
			     "-dict", MODELDIR "/lm/en/turtle.dic",
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
	ps_free(teller_state->ps);	
}

void teller_listen(TellerState *teller_state) {
	FILE *fh;
	int samples;
	TellerHyp *hyp = teller_new_hyp();

	fh = fopen(SAMPLEDIR "/goforward.raw", "rb");
	if (fh == NULL) {
		perror("Failed to open goforward.raw");
		return;
	}

	// decode with utterance id speech, -1 -> until end of file
	samples = ps_decode_raw(teller_state->ps, fh, "speech", -1);
	if (samples < 0) {
		fprintf(stderr, "Error decoding speech\n");
		return;
	}

	fclose(fh);

	// get the hypothesis from the decoding
	hyp->str = ps_get_hyp(teller_state->ps, &hyp->score, &hyp->uttid);
	if(hyp->str == NULL) {
		fprintf(stderr, "Error making hypothesis\n");
		return;
	}

	teller_parse_hyp(teller_state, hyp);
	teller_delete_hyp(hyp);
}

TellerHyp *teller_new_hyp() {
	TellerHyp *hyp = (TellerHyp *) malloc(sizeof(TellerHyp));

	hyp->str = NULL;
	hyp->uttid = NULL;
	hyp->score = 0;

	return hyp;
}

void teller_delete_hyp(TellerHyp *hyp) {
	free(hyp);
}

void teller_parse_hyp(TellerState *teller_state, TellerHyp *hyp) {
	printf("Recognized: %s\n", hyp->str);
}


