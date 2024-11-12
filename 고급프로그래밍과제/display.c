/*
*   display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

// ����� ������� �»��(topleft) ��ǥ
const POSITION resource_pos = { 0, 0 };         // �ڿ� ����
const POSITION map_pos = { 1, 0 };              // �� ��ġ
const POSITION system_message_pos = { 19, 0 };  // �ý��� �޽��� ��ġ
const POSITION status_pos = { 1, 65 };          // ����â ��ġ (�� �����ʿ� ��ġ)
const POSITION commands_pos = { 19, 65 };       // ���â ��ġ (����â �Ʒ��� ��ġ)

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

// �Լ� ����
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
    // �ڿ� ���� ���
    display_resource(resource);

    // �� �� Ŀ�� ���
    display_map(map);
    display_cursor(cursor);

    // �ý��� �޽���, ����â, ���â ���� ���
    display_system_message();
    display_status();
    display_commands();
}

// �ڿ� ���� ��� �Լ�
void display_resource(RESOURCE resource) {
    set_color(COLOR_RESOURCE);
    gotoxy(resource_pos);
    printf("�����̽� = %d/%d, �α� = %d/%d\n",
        resource.spice, resource.spice_max,
        resource.population, resource.population_max
    );
}

// �ý��� �޽����� ǥ���ϴ� �Լ� (�ϴ� �߾�)
void display_system_message(void) {
    set_color(COLOR_DEFAULT);
    gotoxy(system_message_pos);
    printf("[�ý��� �޽���] ��� �ý����� ���� �۵� ���Դϴ�\n"); // ���� �޽���
}

// ���õ� ����/�ǹ��� ������ ǥ���ϴ� �Լ� (������ ���)
void display_status(void) {
    set_color(COLOR_DEFAULT);
    gotoxy(status_pos);
    printf("[����] ���õ� ����: ����\n"); // ���õ� ���� ����
}

// ��ɾ ǥ���ϴ� �Լ� (������ �ϴ�)
void display_commands(void) {
    set_color(COLOR_DEFAULT);
    gotoxy(commands_pos);
    printf("[��ɾ�] �̵� | ���� | ��� | �ڿ� ����\n");
}

// �� ������ ���� �Լ�
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

// �� ��� �Լ�
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

// Ŀ�� ��ġ ǥ�� �Լ�
void display_cursor(CURSOR cursor) {
    POSITION prev = cursor.previous;
    POSITION curr = cursor.current;

    char ch = frontbuf[prev.row][prev.column];
    printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

    ch = frontbuf[curr.row][curr.column];
    printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}
