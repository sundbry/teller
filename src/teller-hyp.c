#include "teller.h"
#include "teller-hyp.h"

static int teller_hyp_score_is_actionable(int score);
static int teller_parse_count(const char *numStr);

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

TellerAction *teller_parse_hyp(TellerState *teller_state, TellerHyp *hyp) {
	TellerActionCommand *nextCmd = teller_state->actionList;
	TellerAction *act = NULL;
	char *cmdStr = NULL, *matchStr, *tailStr;
	static const char *tellerNames[] = {"TELEVISION", "T_V"};
	static const int tellerNameCount = 2;
	int idx, count;

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

		// match against a command
		while(act == NULL && nextCmd != NULL) {
			matchStr = strstr(cmdStr, nextCmd->str);
			// if matched at beginning of command
			if(matchStr != NULL && matchStr == cmdStr) {

				tailStr = matchStr + strlen(nextCmd->str);
				if(nextCmd->countable) {
					if(*tailStr == '\0') {
						count = 1;
					}
					else {
						while(*tailStr == ' ') {
							tailStr++;
						}
						count = teller_parse_count(tailStr);	
					}
					if(count > 0) {
						act = teller_new_action(nextCmd);
						act->count = count;
					}
				}
				else {
					if(*tailStr == '\0') {
						act = teller_new_action(nextCmd);
					}
				}
			}
			nextCmd = nextCmd->next;
		}
	}
	
	return act;
}

int teller_hyp_score_is_actionable(int score) {
	return 1;
}

int teller_parse_count(const char *numStr) {
	static const char *numStrs[] = { "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "ELEVEN", "TWELVE", "THIRTEEN", "FOURTEEN", "FIFTEEN", "SIXTEEN", "SEVENTEEN", "EIGHTEEN", "NINETEEN", "TWENTY"};
	static const int numStrsAmt = 20;
	int idx = 0;

	while(idx < numStrsAmt) {
		if(strcmp(numStr, numStrs[idx]) == 0) {
			return idx + 1;
		}
		idx++;
	}

	return 0;
}
