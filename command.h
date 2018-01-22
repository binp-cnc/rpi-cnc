#pragma once

#include <stdlib.h>

#define CMD_NONE       0x00
#define CMD_WAIT       0x01
#define CMD_MOVE       0x02

typedef struct {
	
} CmdNone;

typedef struct {
	uint32_t duration;
} CmdWait;

typedef struct {
	int32_t steps;
	float speed;
	float accel;
} CmdMove;

typedef struct {
	int type;
	union {
		CmdNone      none;
		CmdWait      wait;
		CmdMove      move;
	};
} Cmd;

Cmd cmd_none() {
	Cmd cmd;
	cmd.type = CMD_NONE;
	return cmd;
}

Cmd cmd_wait(uint32_t duration) {
	Cmd cmd;
	cmd.type = CMD_WAIT;
	cmd.wait.duration = duration;
	return cmd;
}

Cmd cmd_move(int32_t steps, float speed, float accel) {
	Cmd cmd;
	cmd.type = CMD_MOVE;
	cmd.move.steps = steps;
	cmd.move.speed = speed;
	cmd.move.accel = accel;
	return cmd;
}