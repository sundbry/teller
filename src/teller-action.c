#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "teller.h"
#include "teller-action.h"

void teller_load_actions(TellerActionCommand **head) {
	FILE *actionFile = fopen(TELLER_ACTION_FILE, "r");
	char *line = alloca(TELLER_MAX_ACTION_LENGTH), *buf;
	TellerActionCommand *cmd, *tail;
	int len;

	*head = NULL;
	tail = NULL;
	
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
		strcpy(cmd->str, buf);

		if(tail == NULL) {
			*head = cmd;	
		}
		else {
			tail->next = cmd;
		}

		tail = cmd;
	}

	fclose(actionFile);
}

void teller_unload_actions(TellerActionCommand **head) {
	TellerActionCommand *cmd, *next = *head;

	while(next != NULL) {
		cmd = next;
		next = cmd->next;
		//free(cmd->str);
		free(cmd);
	}

	*head = NULL;
}

void teller_action_execute(TellerState *teller_state, const TellerActionCommand *aCmd) {
	char arg[TELLER_IGCLIENT_ARG_LENGTH];
	int status, pid;
	FILE *test;

	snprintf(arg, TELLER_IGCLIENT_ARG_LENGTH, "%s/%s/%s",
		REMOTE_CTRL_DIR, teller_state->remoteName, aCmd->irCode); 	

	test = fopen(arg, "r");
	if(test == NULL) {
		fprintf(stderr, "Remote signal file not found: '%s'\n",
			arg);
	}
	else {
		fclose(test);
	}

	snprintf(arg, TELLER_IGCLIENT_ARG_LENGTH, "--send=%s/%s/%s",
		REMOTE_CTRL_DIR, teller_state->remoteName, aCmd->irCode); 	
	
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
	}
}

