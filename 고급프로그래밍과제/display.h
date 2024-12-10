#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// ���� ����
#define COLOR_DEFAULT 15  // ȸ��
#define COLOR_CURSOR 112  // Ŀ�� ����
#define COLOR_RESOURCE 14 // ��Ȳ�� (�����̽�)
#define COLOR_ATREIDES 9  // Ǫ���� (��Ʈ���̵� ����)
#define COLOR_HARKONNEN 12 // ������ (���ڳ� �Ϻ�����)
#define COLOR_SANDWORM 6   // Ȳ��� (�����)
#define COLOR_PLATE 0      // ������ (����)
#define COLOR_ROCK 8       // ȸ�� (����)

// Windows ���� �ڵ� ����
#define COLOR_BLUE     1  // �Ķ���
#define COLOR_GREEN    2  // ���
#define COLOR_RED      4  // ������
#define COLOR_WHITE    15 // ���
#define COLOR_YELLOW   14 // �����
#define COLOR_BLACK    0  // ������
#define COLOR_GRAY     8  // ȸ��
#define COLOR_BROWN    6  // ����

// ��� ��ġ ��� (�ܺ� ���� ����)
extern const POSITION resource_pos;        // �ڿ� ����
extern const POSITION map_pos;             // �� ��ġ
extern const POSITION system_message_pos;  // �ý��� �޽��� ��ġ
extern const POSITION status_pos;          // ����â ��ġ
extern const POSITION commands_pos;        // ���â ��ġ

// ȭ�� ��� �Լ� ����
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
