#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 색상 정의
#define COLOR_DEFAULT 15  // 회색
#define COLOR_CURSOR 112  // 커서 색상
#define COLOR_RESOURCE 14 // 주황색 (스파이스)
#define COLOR_ATREIDES 9  // 푸른색 (아트레이디스 본부)
#define COLOR_HARKONNEN 12 // 빨간색 (하코넨 하베스터)
#define COLOR_SANDWORM 6   // 황토색 (샌드웜)
#define COLOR_PLATE 0      // 검은색 (장판)
#define COLOR_ROCK 8       // 회색 (바위)

// Windows 색상 코드 정의
#define COLOR_BLUE     1  // 파란색
#define COLOR_GREEN    2  // 녹색
#define COLOR_RED      4  // 빨간색
#define COLOR_WHITE    15 // 흰색
#define COLOR_YELLOW   14 // 노란색
#define COLOR_BLACK    0  // 검정색
#define COLOR_GRAY     8  // 회색
#define COLOR_BROWN    6  // 갈색

// 출력 위치 상수 (외부 참조 가능)
extern const POSITION resource_pos;        // 자원 상태
extern const POSITION map_pos;             // 맵 위치
extern const POSITION system_message_pos;  // 시스템 메시지 위치
extern const POSITION status_pos;          // 상태창 위치
extern const POSITION commands_pos;        // 명령창 위치

// 화면 출력 함수 선언
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
