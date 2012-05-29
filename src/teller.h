#ifndef TELLER_H
#define TELLER_H

#include <pocketsphinx.h>

#define SAMPLEDIR BASEDIR "/lib/pocketsphinx-0.7/test/data"
#define KNOWLEDGEDIR BASEDIR "/knowledge"
#define REMOTE_CTRL_DIR BASEDIR "/rc"
#define TELLER_HMM MODELDIR "/hmm/en_US/hub4wsj_sc_8k"
/*
#define TELLER_LM MODELDIR "/lm/en/turtle.DMP"
#define TELLER_DICT MODELDIR "/lm/en/turtle.dic"
*/
#define TELLER_LM KNOWLEDGEDIR "/8060.lm"
#define TELLER_DICT KNOWLEDGEDIR "/8060.dic"
#define TELLER_DEFAULT_REMOTE "default-remote"

typedef struct TellerActionCommand_t TellerActionCommand;

typedef struct TellerState_t {
	ps_decoder_t *ps;
	cmd_ln_t *config;
	const char *logPath;
	int logfd;
	TellerActionCommand *actionList;
	char *remoteName;
} TellerState;

void teller_init(TellerState *teller_state);
void teller_deinit(TellerState *teller_state);

#endif
