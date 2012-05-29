#ifndef TELLER_ACTION_H
#define TELLER_ACTION_H

#include "teller.h"

#define TELLER_ACTION_FILE KNOWLEDGEDIR "/actions.txt"
#define TELLER_MAX_ACTION_LENGTH 256
#define TELLER_MAX_IRCODE_LENGTH 64
#define TELLER_IGCLIENT_ARG_LENGTH 512
// min interval in usec between ir buttons
#define TELLER_MIN_IR_INTERVAL 50000

#define BIN_IRSEND "/usr/local/bin/irsend"
#define BIN_IGCLIENT "/usr/bin/igclient"

typedef struct TellerActionCommand_t {
	struct TellerActionCommand_t *next;
	char irCode[TELLER_MAX_IRCODE_LENGTH];
	int countable;
	char str[TELLER_MAX_ACTION_LENGTH];
} TellerActionCommand;

typedef struct TellerAction_t {
	struct TellerAction_t *next;
	TellerActionCommand *cmd;
	int count;
} TellerAction;

void teller_load_actions(TellerState *teller_state);
void teller_unload_actions(TellerState *teller_state);
void teller_action_queue(TellerAction *act);
void teller_action_execute(TellerState *teller_state, TellerAction *aCmd);

TellerAction *teller_new_action(TellerActionCommand *aCmd);
void teller_delete_action(TellerAction *act);

#endif
