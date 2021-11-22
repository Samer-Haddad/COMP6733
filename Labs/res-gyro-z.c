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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rest-engine.h"
#include "board-peripherals.h"
#include "mpu-9250-sensor.h"
#include "sys/ctimer.h"


static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_gyro_z, "title=\"Gyroscope Z: ?n=1..\";rt=\"Gyroscope\"", res_get_handler, NULL, NULL, NULL);

int size_z = 5;

char buffer_z[]="";

int gyro_state_z = 0;
int gyro_counter_z = 5;
static struct ctimer gyro_timer_z;

static void scan_z(int reading, char* buffer) {
	char temp[16];
	sprintf(temp, "%d.%02d,", reading/100, abs(reading)%100);
	strcat(buffer, temp);
}

void gyro_sample_z() {
	if (gyro_counter_z != 0) {
		int value;
		value = mpu_9250_sensor.value(MPU_9250_SENSOR_TYPE_GYRO_Z);
		scan_z(value, buffer_z);
		--gyro_counter_z;
		ctimer_reset(&gyro_timer_z);
	}
	else {
		gyro_state_z = 0;
		ctimer_stop(&gyro_timer_z);
	}
}

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
	const char *n = NULL;
	if (REST.get_query_variable(request, "n", &n)) {
		size_z = atoi(n);
		gyro_counter_z = size_z;
	}
		
	if (gyro_state_z == 0) {
		gyro_state_z = 1;
		mpu_9250_sensor.configure(SENSORS_ACTIVE,MPU_9250_SENSOR_TYPE_GYRO_ALL);
		ctimer_set(&gyro_timer_z, 0.2*CLOCK_SECOND, gyro_sample_z, NULL);			
	}
	else {
		gyro_state_z = 0;
		ctimer_stop(&gyro_timer_z);
		int size = sizeof(buffer_z);
		if (size > REST_MAX_CHUNK_SIZE) {
			size = REST_MAX_CHUNK_SIZE;
		}
		memcpy(buffer, buffer_z, size);
		REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
		REST.set_header_etag(response, (uint8_t *)&size, 1);
		REST.set_response_payload(response, buffer, size);
	}

}
