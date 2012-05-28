#ifndef TELLER_HYP_H
#define TELLER_HYP_H

#include "teller.h"
#include "teller-action.h"

/** A hypothesis */
typedef struct TellerHyp_t {
	const char *str;
	const char *uttid;
	int32 score;
} TellerHyp;

TellerHyp *teller_new_hyp();
void teller_delete_hyp(TellerHyp *hyp);
TellerActionCommand *teller_parse_hyp(TellerState *teller_state, TellerHyp *hyp);

#endif
