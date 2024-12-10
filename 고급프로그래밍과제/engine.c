#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

// 초기화 함수 선언
void init(void);
void intro(void);
void outro(void);
void cursor_move_with_double_click(KEY key);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);

// ================= control ===================
int sys_clock = 0;       // system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };

// ================= game data ===================
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };

RESOURCE resource = {
    .spice = 0,
    .spice_max = 0,
    .population = 0,
    .population_max = 0
};

OBJECT_SAMPLE obj = {
    .pos = {1, 1},
    .dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
    .repr = 'o',
    .move_period = 300,
    .next_move_time = 300
};

// 키 입력 상태 저장
typedef struct {
    clock_t last_time;  // 마지막 키 입력 시간
    KEY last_key;       // 마지막 입력된 키
} KEY_STATE;

KEY_STATE key_state = { 0, k_none };

// ================= main() ===================
int main(void) {
    srand((unsigned int)time(NULL));

    init();
    intro();
    display(resource, map, cursor);

    while (1) {
        // loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
        KEY key = get_key();

        // 키 입력이 있으면 처리
        if (is_arrow_key(key)) {
            cursor_move_with_double_click(key);
        }
        else {
            // 방향키 외의 입력
            switch (key) {
            case k_quit: outro();
            case k_none:
            case k_undef:
            default: break;
            }
        }

        // 샘플 오브젝트 동작
        sample_obj_move();

        // 화면 출력
        display(resource, map, cursor);
        Sleep(TICK);
        sys_clock += 10;
    }
}

// ================= subfunctions ===================
void intro(void) {
    printf("DUNE 1.5\n");
    Sleep(2000);
    system("cls");
}

void outro(void) {
    printf("exiting...\n");
    exit(0);
}

void init(void) {
    // 맵 경계 설정 (테두리)
    for (int j = 0; j < MAP_WIDTH; j++) {
        map[0][0][j] = '#';
        map[0][MAP_HEIGHT - 1][j] = '#';
    }

    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        map[0][i][0] = '#';
        map[0][i][MAP_WIDTH - 1] = '#';
        for (int j = 1; j < MAP_WIDTH - 1; j++) {
            map[0][i][j] = ' ';  // 빈 칸은 기본 지형(사막)으로 설정
        }
    }

    // layer 1(map[1]) 초기화 (비워 두기)
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[1][i][j] = -1;  // 유닛이 없는 곳은 -1로 초기화
        }
    }

    // 좌하단 본부 (2x2), 하베스터, 스파이스, 장판
    map[0][MAP_HEIGHT - 3][1] = 'B';  // 본부
    map[0][MAP_HEIGHT - 3][2] = 'B';
    map[0][MAP_HEIGHT - 2][1] = 'B';
    map[0][MAP_HEIGHT - 2][2] = 'B';

    map[1][MAP_HEIGHT - 4][1] = 'H';  // 하베스터
    map[0][MAP_HEIGHT - 6][1] = '5';  // 스파이스 매장지

    map[0][MAP_HEIGHT - 3][3] = 'P';  // 장판 (2x2)
    map[0][MAP_HEIGHT - 3][4] = 'P';
    map[0][MAP_HEIGHT - 2][3] = 'P';
    map[0][MAP_HEIGHT - 2][4] = 'P';

    // 우상단 본부 (2x2), 하베스터, 스파이스, 장판
    map[0][1][MAP_WIDTH - 3] = 'B';  // 본부
    map[0][1][MAP_WIDTH - 2] = 'B';
    map[0][2][MAP_WIDTH - 3] = 'B';
    map[0][2][MAP_WIDTH - 2] = 'B';

    map[1][3][MAP_WIDTH - 2] = 'H';  // 하베스터
    map[0][5][MAP_WIDTH - 2] = '5';  // 스파이스 매장지

    map[0][1][MAP_WIDTH - 5] = 'P';  // 장판 (2x2)
    map[0][1][MAP_WIDTH - 4] = 'P';
    map[0][2][MAP_WIDTH - 5] = 'P';
    map[0][2][MAP_WIDTH - 4] = 'P';

    // 중립 샌드웜 2마리 배치
    map[0][2][8] = 'W';
    map[1][13][40] = 'W';

    // 바위 적당히 배치 (1x1, 2x2)
    map[0][3][24] = 'R';
    map[0][3][25] = 'R';
    map[0][4][24] = 'R';
    map[0][4][25] = 'R';

    map[0][4][47] = 'R';
    map[0][12][13] = 'R';

    map[0][14][30] = 'R';
    map[0][14][31] = 'R';
    map[0][15][30] = 'R';
    map[0][15][31] = 'R';
    map[0][15][55] = 'R';
}

// 방향키 더블클릭 감지 함수
bool is_double_click(KEY key) {
    clock_t current_time = clock();
    bool double_click = false;

    if (key == key_state.last_key) {
        // 200ms 이내에 동일한 키가 두 번 입력되었는지 확인
        if ((current_time - key_state.last_time) <= CLOCKS_PER_SEC / 5) {
            double_click = true;
        }
    }

    // 키 상태 업데이트
    key_state.last_time = current_time;
    key_state.last_key = key;

    return double_click;
}

// 방향키 입력 처리 함수
void cursor_move_with_double_click(KEY key) {
    DIRECTION dir = ktod(key);
    POSITION new_pos = cursor.current;

    if (is_double_click(key)) {
        // 더블클릭 시 두 칸 이동
        new_pos = pmove(pmove(cursor.current, dir), dir);
    }
    else {
        // 일반 클릭 시 한 칸 이동
        new_pos = pmove(cursor.current, dir);
    }

    // 경계 검사
    if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
        1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {

        // 이전 위치 초기화 및 새 위치 업데이트
        cursor.previous = cursor.current;
        cursor.current = new_pos;
    }
}

// 샘플 오브젝트 동작
POSITION sample_obj_next_position(void) {
    POSITION diff = psub(obj.dest, obj.pos);
    DIRECTION dir;

    // 목적지 도착 시 왕복
    if (diff.row == 0 && diff.column == 0) {
        if (obj.dest.row == 1 && obj.dest.column == 1) {
            obj.dest = (POSITION){ MAP_HEIGHT - 2, MAP_WIDTH - 2 };
        }
        else {
            obj.dest = (POSITION){ 1, 1 };
        }
        return obj.pos;
    }

    // 이동 방향 결정
    if (abs(diff.row) >= abs(diff.column)) {
        dir = (diff.row >= 0) ? d_down : d_up;
    }
    else {
        dir = (diff.column >= 0) ? d_right : d_left;
    }

    POSITION next_pos = pmove(obj.pos, dir);
    if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
        1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
        map[1][next_pos.row][next_pos.column] < 0) {

        return next_pos;
    }
    else {
        return obj.pos;
    }
}

void sample_obj_move(void) {
    if (sys_clock <= obj.next_move_time) {
        return;
    }

    map[1][obj.pos.row][obj.pos.column] = -1;
    obj.pos = sample_obj_next_position();
    map[1][obj.pos.row][obj.pos.column] = obj.repr;

    obj.next_move_time = sys_clock + obj.move_period;
}
