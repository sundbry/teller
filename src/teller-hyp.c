#include "teller.h"
#include "teller-hyp.h"

static int teller_hyp_score_is_actionable(int score);

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

TellerActionCommand *teller_parse_hyp(TellerState *teller_state, TellerHyp *hyp) {
	TellerActionCommand *aCmd = NULL, *nextCmd = teller_state->actionList;
	char *cmdStr = NULL, *matchStr;
	static const char *tellerNames[] = {"TELEVISION", "T_V"};
	static const int tellerNameCount = 2;
	int idx, cmdStrLen;

	printf("Recognized: %s (score %d)\n", hyp->str, hyp->score);

	if(teller_hyp_score_is_actionable(hyp->score)) {
		for(idx = 0; idx < tellerNameCount && cmdStr == NULL; idx++) {
			cmdStr = strstr(hyp->str, tellerNames[idx]);
			if(cmdStr != NULL) {
				cmdStr += strlen(tellerNames[idx]);
			}
		}

		// if no name (command indicator) match
		if(cmdStr == NULL) {
			return NULL;
		}

		// trim space off the left side
		while(*cmdStr == ' ') {
			cmdStr++;
		}

		printf("command str: '%s'\n", cmdStr);

		cmdStrLen = strlen(cmdStr);

		// match against a command
		while(aCmd == NULL && nextCmd != NULL) {
			matchStr = strstr(cmdStr, nextCmd->str);
			// if matched and no junk after
			if(matchStr != NULL && strlen(nextCmd->str) == cmdStrLen) {
				aCmd = nextCmd;
			}
			nextCmd = nextCmd->next;
		}
	}
	
	return aCmd;
}

int teller_hyp_score_is_actionable(int score) {
	return 1;
}

