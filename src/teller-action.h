#ifndef TELLER_ACTION_H
#define TELLER_ACTION_H

#include "teller.h"

#define TELLER_ACTION_FILE KNOWLEDGEDIR "/actions.txt"
#define TELLER_MAX_ACTION_LENGTH 256
#define TELLER_MAX_IRCODE_LENGTH 64
#define TELLER_IGCLIENT_ARG_LENGTH 512

#define BIN_IRSEND "/usr/local/bin/irsend"
#define BIN_IGCLIENT "/usr/bin/igclient"

typedef struct TellerActionCommand_t {
	struct TellerActionCommand_t *next;
	char str[TELLER_MAX_ACTION_LENGTH];
	char irCode[TELLER_MAX_IRCODE_LENGTH];
} TellerActionCommand;

typedef struct TellerAction_t {
	TellerActionCommand *cmd;
} TellerAction;

void teller_load_actions(TellerActionCommand **head);
void teller_unload_actions(TellerActionCommand **head);
void teller_action_execute(TellerState *teller_state, const TellerActionCommand *aCmd);

#endif
