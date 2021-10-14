/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Example resource
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include "contiki.h"

#if PLATFORM_HAS_LEDS

#include <string.h>
#include "contiki.h"
#include "rest-engine.h"
#include "dev/leds.h"

#include "sys/ctimer.h"

///////////////////////////////////////////

static void res_post_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_led_red, "title=\"RED LED\";rt=\"Control\"", NULL, res_post_handler, NULL, NULL);

int led_red_state = 0;
static struct ctimer led_red_timer;
static struct ctimer led_red_off_timer;

void led_red_toggle();

void led_red_toggle_off() {
	leds_off(LEDS_RED);
	ctimer_set(&led_red_timer, 0.2*CLOCK_SECOND, led_red_toggle, NULL);
}

void led_red_toggle() {
	if(led_red_state == 0) {
		leds_off(LEDS_RED);	
	}
	else if(led_red_state == 1) {
		leds_off(LEDS_RED);
		leds_toggle(LEDS_RED);
		ctimer_set(&led_red_off_timer, 0.2*CLOCK_SECOND, led_red_toggle_off, NULL); //turn off after 1 sec
	}
}

static void res_post_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
	led_red_state = !led_red_state;
	ctimer_stop(&led_red_off_timer);
	ctimer_set(&led_red_timer, 0.2*CLOCK_SECOND, led_red_toggle, NULL);
}
///////////////////////////////////////////

#endif /* PLATFORM_HAS_LEDS */
