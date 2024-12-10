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
void cursor_move(DIRECTION dir);
void handle_selection(void); // 선택 처리 함수 선언
void sample_obj_move(void);
POSITION sample_obj_next_position(void);
void display_selection(void); // 선택 상태 출력 함수 선언

// ================= control ===================
int sys_clock = 0;       // system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };

// 선택 상태를 관리하기 위한 구조체
typedef struct {
    bool is_selected;   // 선택 여부
    POSITION pos;       // 선택된 위치
    char repr;          // 선택된 객체의 문자
} SELECTED_STATE;

SELECTED_STATE selected_state = { false, { -1, -1 }, '\0' };

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

// ================= main() ===================
int main(void) {
    srand((unsigned int)time(NULL));

    init();
    intro();
    display(resource, map, cursor);

    while (1) {
        KEY key = get_key();

        if (is_arrow_key(key)) {
            cursor_move(ktod(key));
        }
        else {
            switch (key) {
            case k_space: handle_selection(); break; // Space 키 입력 처리
            case k_quit: outro(); break;
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

void cursor_move(DIRECTION dir) {
    POSITION curr = cursor.current;
    POSITION new_pos;

    // 더블클릭 감지
    static clock_t last_click_time = 0;
    clock_t current_time = clock();
    bool is_double_click = (current_time - last_click_time) <= CLOCKS_PER_SEC / 5; // 200ms 이내 더블클릭
    last_click_time = current_time;

    if (is_double_click) {
        // 더블클릭 시 5칸 이동
        new_pos = curr;
        for (int i = 0; i < 5; i++) {
            new_pos = pmove(new_pos, dir);
            // 맵 경계를 초과하지 않도록 확인
            if (!(1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
                1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2)) {
                break;
            }
        }
    }
    else {
        // 일반 클릭 시 1칸 이동
        new_pos = pmove(curr, dir);
    }

    // 이동한 위치가 유효한 경우 업데이트
    if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
        1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {
        cursor.previous = cursor.current;
        cursor.current = new_pos;
    }
}


void handle_selection(void) {
    POSITION curr = cursor.current;
    char repr = map[0][curr.row][curr.column]; // 현재 커서 위치의 객체

    if (!selected_state.is_selected) {
        // 선택되지 않은 상태에서 Space 키 입력
        selected_state.is_selected = true;
        selected_state.pos = curr;
        selected_state.repr = repr;
    }
    else {
        // 이미 선택된 상태에서 Space 키 입력
        // 같은 위치의 유닛을 다시 선택한 경우 상태를 유지
        if (selected_state.pos.row == curr.row && selected_state.pos.column == curr.column) {
            selected_state.is_selected = false;
        }
        else {
            // 다른 위치의 유닛 선택
            selected_state.pos = curr;
            selected_state.repr = repr;
        }
    }

    // 선택 상태를 즉시 상태창에 출력
    display_selection();
}
    
void display_selection(void) {
    gotoxy(status_pos);
    if (selected_state.is_selected) {
        char repr = selected_state.repr;
        if (repr == ' ') {
            printf("[상태] 선택된 지형: 사막\n");
        }
        else if (repr == 'B') {
            printf("[상태] 선택된 유닛: 본부 (Base)\n[명령어] 이동 | 공격 | 방어\n");
        }
        else if (repr == 'H') {
            printf("[상태] 선택된 유닛: 하베스터 (Harvester)\n[명령어] 자원 수집 | 이동\n");
        }
        else if (repr == '5') {
            printf("[상태] 선택된 지형: 스파이스 매장지\n");
        }
        else {
            printf("[상태] 알 수 없는 객체: [%c]\n", repr);
        }
    }
    else {
        printf("[상태] 선택된 유닛: 없음\n");
    }
}



POSITION sample_obj_next_position(void) {
    POSITION diff = psub(obj.dest, obj.pos);
    DIRECTION dir;

    if (diff.row == 0 && diff.column == 0) {
        if (obj.dest.row == 1 && obj.dest.column == 1) {
            obj.dest = (POSITION){ MAP_HEIGHT - 2, MAP_WIDTH - 2 };
        }
        else {
            obj.dest = (POSITION){ 1, 1 };
        }
        return obj.pos;
    }

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
