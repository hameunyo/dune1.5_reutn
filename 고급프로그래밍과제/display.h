/*
*  display.h
* ȭ�鿡 ���� ������ ����ϱ� ���� �Լ����� ������ ����
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// ǥ���� ���� ����
#define COLOR_DEFAULT 15
#define COLOR_CURSOR  112
#define COLOR_RESOURCE 112

// display() �Լ����� ȣ���� �� ȭ�� ǥ�ÿ� �Լ����� ����
void display(
    RESOURCE resource,
    char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
    CURSOR cursor
);

void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_system_message(void);
void display_status(void);
void display_commands(void);

#endif