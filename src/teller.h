#include <pocketsphinx.h>

typedef struct TellerState_t {
	ps_decoder_t *ps;
	cmd_ln_t *config;
} TellerState;

/** A hypothesis */
typedef struct TellerHyp_t {
	const char *str;
	const char *uttid;
	int32 score;
} TellerHyp;

void teller_init(TellerState *teller_state);
void teller_listen(TellerState *teller_state);


TellerHyp *teller_new_hyp();
void teller_delete_hyp(TellerHyp *hyp);
void teller_parse_hyp(TellerState *teller_state, TellerHyp *hyp);