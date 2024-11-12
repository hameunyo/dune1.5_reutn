/*
*   display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };         // 자원 상태
const POSITION map_pos = { 1, 0 };              // 맵 위치
const POSITION system_message_pos = { 19, 0 };  // 시스템 메시지 위치
const POSITION status_pos = { 1, 65 };          // 상태창 위치 (맵 오른쪽에 위치)
const POSITION commands_pos = { 19, 65 };       // 명령창 위치 (상태창 아래에 위치)

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

// 함수 정의
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_system_message(void);
void display_status(void);
void display_commands(void);

void display(
    RESOURCE resource,
    char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
    CURSOR cursor)
{
    // 자원 상태 출력
    display_resource(resource);

    // 맵 및 커서 출력
    display_map(map);
    display_cursor(cursor);

    // 시스템 메시지, 상태창, 명령창 각각 출력
    display_system_message();
    display_status();
    display_commands();
}

// 자원 상태 출력 함수
void display_resource(RESOURCE resource) {
    set_color(COLOR_RESOURCE);
    gotoxy(resource_pos);
    printf("스파이스 = %d/%d, 인구 = %d/%d\n",
        resource.spice, resource.spice_max,
        resource.population, resource.population_max
    );
}

// 시스템 메시지를 표시하는 함수 (하단 중앙)
void display_system_message(void) {
    set_color(COLOR_DEFAULT);
    gotoxy(system_message_pos);
    printf("[시스템 메시지] 모든 시스템이 정상 작동 중입니다\n"); // 예시 메시지
}

// 선택된 유닛/건물의 정보를 표시하는 함수 (오른쪽 상단)
void display_status(void) {
    set_color(COLOR_DEFAULT);
    gotoxy(status_pos);
    printf("[상태] 선택된 유닛: 없음\n"); // 선택된 유닛 정보
}

// 명령어를 표시하는 함수 (오른쪽 하단)
void display_commands(void) {
    set_color(COLOR_DEFAULT);
    gotoxy(commands_pos);
    printf("[명령어] 이동 | 공격 | 방어 | 자원 수집\n");
}

// 맵 데이터 병합 함수
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            for (int k = 0; k < N_LAYER; k++) {
                if (src[k][i][j] >= 0) {
                    dest[i][j] = src[k][i][j];
                }
            }
        }
    }
}

// 맵 출력 함수
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
    project(map, backbuf);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (frontbuf[i][j] != backbuf[i][j]) {
                POSITION pos = { i, j };
                printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
            }
            frontbuf[i][j] = backbuf[i][j];
        }
    }
}

// 커서 위치 표시 함수
void display_cursor(CURSOR cursor) {
    POSITION prev = cursor.previous;
    POSITION curr = cursor.current;

    char ch = frontbuf[prev.row][prev.column];
    printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

    ch = frontbuf[curr.row][curr.column];
    printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}
