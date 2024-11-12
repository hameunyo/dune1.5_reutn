/*
*  display.h
* 화면에 게임 정보를 출력하기 위한 함수들의 선언을 포함
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 표시할 색상 정의
#define COLOR_DEFAULT 15
#define COLOR_CURSOR  112
#define COLOR_RESOURCE 112

// display() 함수에서 호출할 각 화면 표시용 함수들을 선언
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