#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include <pigpio.h>

#include "command.h"
#include "generator.h"
#include "axis.h"
#include "device.h"

typedef struct {
	int32_t pos;
	int32_t vel;
	int32_t acc;
} Chan;

typedef struct {
	Chan chan[2];
} Cookie;

Cmd move_cmd(int ax, void *userdata) {
	Cookie *cookie = (Cookie*) userdata;
	Cmd cmd = cmd_none();
	if (ax == 0 || ax == 1) {
		if (cookie->chan[ax].pos != 0) {
			cmd = cmd_move(
				cookie->chan[ax].pos,
				cookie->chan[ax].vel,
				cookie->chan[ax].acc
			);
			cookie->chan[ax].pos = 0;
		}
	}
	return cmd;
}


Generator gen;
Device dev;

int cnc_init() {
	printf("start\n");
	if (gpioInitialise() < 0) {
		printf("error gpio init\n");
		return 1;
	}

	gen_init(&gen, 0x10);
	dev_init(&dev, 2);

	Axis *axis_x = &dev.axes[0];
	Axis *axis_y = &dev.axes[1];
	axis_init(axis_x,  6, 13, 19, 16);
	axis_init(axis_y, 27, 22, 12,  5);

	return 0;
}

int cnc_quit() {
	Axis *axis_x = &dev.axes[0];
	Axis *axis_y = &dev.axes[1];
	axis_free(axis_x);
	axis_free(axis_y);

	dev_free(&dev);
	gen_free(&gen);

	gpioTerminate();
	
	printf("stop\n");

	return 0;
}

int cnc_scan_x() {
	Axis *axis_x = &dev.axes[0];
	axis_scan(axis_x, &gen, 2000, 10000);
	printf("length: %d\n", axis_x->length);
	return axis_x->length;
}

int cnc_scan_y() {
	Axis *axis_y = &dev.axes[1];
	axis_scan(axis_y, &gen, 2000, 10000);
	printf("length: %d\n", axis_y->length);
	return axis_y->length;
}

int cnc_move(int32_t px, int32_t py, int32_t vx, int32_t vy, int32_t ax, int32_t ay) {
	Cookie cookie;
	cookie.chan[0].pos = px;
	cookie.chan[1].pos = py;
	cookie.chan[0].vel = vx;
	cookie.chan[1].vel = vy;
	cookie.chan[0].acc = ax;
	cookie.chan[1].acc = ay;

	dev_run(&dev, &gen, move_cmd, (void*) &cookie);
	gen_clear(&gen);
	dev_clear(&dev);

	return 0;
}

int main() {
	return 0;
}
