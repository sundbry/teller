#include <signal.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/time.h>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>

#include "teller.h"
#include "teller-listen.h"
#include "teller-hyp.h"

void teller_listen_file(TellerState *teller_state, const char *file) {
	FILE *fh;
	int samples;
	TellerHyp *hyp = teller_new_hyp();
	TellerAction *act;

	// SAMPLEDIR "/goforward.raw"
	fh = fopen(file, "rb");
	if(fh == NULL) {
		fprintf(stderr, "Failed to open '%s'\n", file);
		return;
	}

	// decode with utterance id speech, -1 -> until end of file
	samples = ps_decode_raw(teller_state->ps, fh, "speech", -1);
	if(samples < 0) {
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

	act = teller_parse_hyp(teller_state, hyp);
	if(act != NULL) {
		teller_action_queue(act);
	}
	teller_delete_hyp(hyp);
}

void teller_listen_mic(TellerState *teller_state) {
    ad_rec_t *ad;
    int16 adbuf[4096]; int32 k, ts, rem;
    cont_ad_t *cont;
	TellerHyp *hyp;
	TellerAction *act;

	const char *deviceName = cmd_ln_str_r(teller_state->config, "-adcdev");
	int sampRate = (int) cmd_ln_float32_r(teller_state->config, "-samprate");

	printf("Opening device '%s' with sample rate %d Hz\n",
		deviceName, sampRate);

    if((ad = ad_open_dev(deviceName, sampRate)) == NULL) {
        E_FATAL("Failed top open audio device\n");
	}

    /* Initialize continuous listening module */
    if((cont = cont_ad_init(ad, ad_read)) == NULL) {
        E_FATAL("Failed to initialize voice activity detection\n");
	}
    if(ad_start_rec(ad) < 0) {
        E_FATAL("Failed to start recording\n");
	}
    if(cont_ad_calib(cont) < 0) {
        E_FATAL("Failed to calibrate voice activity detection\n");
	}

    for(;;) {
        /* Indicate listening for next utterance */
        printf("READY....\n");
        fflush(stdout);
        fflush(stderr);

        /* Wait data for next utterance */
        while((k = cont_ad_read(cont, adbuf, 4096)) == 0) {
            usleep(100000);
		}

        if(k < 0) {
            E_FATAL("Failed to read audio\n");
		}

        /*
         * Non-zero amount of data received; start recognition of new utterance.
         * NULL argument to uttproc_begin_utt => automatic generation of utterance-id.
         */
        if(ps_start_utt(teller_state->ps, NULL) < 0) {
            E_FATAL("Failed to start utterance\n");
		}
        ps_process_raw(teller_state->ps, adbuf, k, FALSE, FALSE);
        printf("Listening...\n");
        fflush(stdout);

        /* Note timestamp for this first block of data */
        ts = cont->read_ts;

        /* Decode utterance until end (marked by a "long" silence, >1sec) */
        for(;;) {
            /* Read non-silence audio data, if any, from continuous listening module */
            if((k = cont_ad_read(cont, adbuf, 4096)) < 0) {
                E_FATAL("Failed to read audio\n");
			}
            if(k == 0) {
                /*
                 * No speech data available; check current timestamp with most recent
                 * speech to see if more than 1 sec elapsed.  If so, end of utterance.
                 */
                if((cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC) {
                    break;
				}
            }
            else {
                /* New speech data received; note current timestamp */
                ts = cont->read_ts;
            }

            /*
             * Decode whatever data was read above.
             */
            rem = ps_process_raw(teller_state->ps, adbuf, k, FALSE, FALSE);

            /* If no work to be done, sleep a bit */
            if((rem == 0) && (k == 0)) {
                usleep(20000);
			}
        }

        /*
         * Utterance ended; flush any accumulated, unprocessed A/D data and stop
         * listening until current utterance completely decoded
         */
        ad_stop_rec(ad);
        while (ad_read(ad, adbuf, 4096) >= 0);
        cont_ad_reset(cont);

        //printf("Stopped listening, please wait...\n");
        //fflush(stdout);
        /* Finish decoding, obtain and print result */
        ps_end_utt(teller_state->ps);

		hyp = teller_new_hyp();
		hyp->str = ps_get_hyp(teller_state->ps, &hyp->score, &hyp->uttid);

		if(hyp->str == NULL) {
			fprintf(stderr, "Error making hypothesis\n");
			return;
		}

		act = teller_parse_hyp(teller_state, hyp);
		if(act != NULL) {
			teller_action_queue(act);
		}
		teller_delete_hyp(hyp);

		/*
        hyp = ps_get_hyp(ps, NULL, &uttid);
        printf("%s: %s\n", uttid, hyp);
        fflush(stdout);
		*/

        /* Exit if the first word spoken was GOODBYE */
		/*
        if (hyp) {
            sscanf(hyp, "%s", word);
            if (strcmp(word, "goodbye") == 0)
                break;
        }
		*/

        /* Resume A/D recording for next utterance */
        if(ad_start_rec(ad) < 0) {
            E_FATAL("Failed to start recording\n");
		}
    }

    cont_ad_close(cont);
    ad_close(ad);
}
