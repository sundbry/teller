#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "teller.h"
#include "teller-action.h"

static TellerState *s_teller_state = NULL;

void teller_load_actions(TellerState *teller_state) {
	FILE *actionFile = fopen(TELLER_ACTION_FILE, "r");
	char *line = alloca(TELLER_MAX_ACTION_LENGTH), *buf;
	TellerActionCommand *cmd, *tail;
	int len;

	s_teller_state = teller_state;

	teller_state->actionList = NULL;
	tail = NULL;

	// skip first line
	fgets(line, TELLER_MAX_ACTION_LENGTH, actionFile);
	
	while(fgets(line, TELLER_MAX_ACTION_LENGTH, actionFile) != NULL) {
		len = strlen(line);
		if(line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}

		cmd = (TellerActionCommand *) malloc(sizeof(TellerActionCommand));	
		cmd->next = NULL;
		buf = strtok(line, "\t");
		strcpy(cmd->irCode, buf);

		buf = strtok(NULL, "\t");
		cmd->countable = (*buf == '1');

		buf = strtok(NULL, "\t");
		strcpy(cmd->str, buf);

		if(tail == NULL) {
			teller_state->actionList = cmd;	
		}
		else {
			tail->next = cmd;
		}

		tail = cmd;
	}

	fclose(actionFile);
}

void teller_unload_actions(TellerState *teller_state) {
	TellerActionCommand *cmd, *next = teller_state->actionList;

	while(next != NULL) {
		cmd = next;
		next = cmd->next;
		//free(cmd->str);
		free(cmd);
	}

	teller_state->actionList = NULL;

	s_teller_state= NULL;
}

void teller_action_queue(TellerAction *act) {
	teller_action_execute(s_teller_state, act);
}

void teller_action_execute(TellerState *teller_state, TellerAction *act) {
	char arg[TELLER_IGCLIENT_ARG_LENGTH];
	int status, pid, count;
	FILE *test;

	snprintf(arg, TELLER_IGCLIENT_ARG_LENGTH, "%s/%s/%s",
		REMOTE_CTRL_DIR, teller_state->remoteName, act->cmd->irCode); 	

	test = fopen(arg, "r");
	if(test == NULL) {
		fprintf(stderr, "Remote signal file not found: '%s'\n",
			arg);
	}
	else {
		fclose(test);
	}

	snprintf(arg, TELLER_IGCLIENT_ARG_LENGTH, "--send=%s/%s/%s",
		REMOTE_CTRL_DIR, teller_state->remoteName, act->cmd->irCode); 	

	count = act->count;

	while(count > 0) {
		pid = fork();

		if(pid == 0) {
			printf("exec %s %s\n", BIN_IGCLIENT, arg);
			fflush(stdout);
			execl(BIN_IGCLIENT, BIN_IGCLIENT, arg, (char *) NULL);
			exit(EXIT_FAILURE);
		}
		else {
			if(pid == -1) {
				perror("teller_action_execute fork");	
				return;
			}
			do {
				waitpid(pid, &status, 0);
			}
			while(!WIFEXITED(status));
			printf("exit status: %d\n", WEXITSTATUS(status));
			count--;
			if(count > 0) {
				usleep(TELLER_MIN_IR_INTERVAL);
			}
		}
	}

	teller_delete_action(act);
}

TellerAction *teller_new_action(TellerActionCommand *aCmd) {
	TellerAction *act = (TellerAction *) malloc(sizeof(TellerAction));
	act->cmd = aCmd;
	act->count = 1;

	return act;
}

void teller_delete_action(TellerAction *act) {
	free(act);
}
