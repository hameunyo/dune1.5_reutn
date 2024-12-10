    #include "display.h"
    #include "io.h"

    // ��� ��ġ ���
    const POSITION resource_pos = { 0, 0 };
    const POSITION map_pos = { 1, 0 };
    const POSITION system_message_pos = { 19, 0 };
    const POSITION status_pos = { 1, 65 };
    const POSITION commands_pos = { 19, 65 };


    char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
    char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

    // ���� �׸��� ���� ���� �Լ���
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
        CURSOR cursor
    ) {
        display_resource(resource);
        display_map(map);
        display_cursor(cursor);
        display_system_message();
        display_commands();
        display_selection(); // ���� ���� ���
    }

    // �ڿ� ���� ��� �Լ�
    void display_resource(RESOURCE resource) {
        set_color(COLOR_RESOURCE);  // �����̽� ����
        gotoxy(resource_pos);
        printf("�����̽� = %d/%d, �α� = %d/%d\n", resource.spice, resource.spice_max, resource.population, resource.population_max);
    }

    // �ý��� �޽����� ǥ���ϴ� �Լ� (�ϴ� �߾�)
    void display_system_message(void) {
        set_color(COLOR_DEFAULT);  // ȸ�� (�⺻ �ؽ�Ʈ ��)
        gotoxy(system_message_pos);
        printf("[�ý��� �޽���] ��� �ý����� ���� �۵� ���Դϴ�\n");
    }

    // ���õ� ����/�ǹ��� ������ ǥ���ϴ� �Լ� (������ ���)
    void display_status(void) {
        set_color(COLOR_DEFAULT);  // ȸ�� (�⺻ �ؽ�Ʈ ��)
        gotoxy(status_pos);
        printf("[����] ���õ� ����: ����\n");
    }

    // ��ɾ ǥ���ϴ� �Լ� (������ �ϴ�)
    void display_commands(void) {
        set_color(COLOR_DEFAULT);  // ȸ�� (�⺻ �ؽ�Ʈ ��)
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
                    int color = COLOR_DEFAULT;

                    // Ư�� ĳ���Ϳ� ���� ���� ����
                    switch (backbuf[i][j]) {
                    case 'B': color = COLOR_BLUE; break; // ���θ� �Ķ������� ǥ��
                    case 'H': color = COLOR_BLUE; break;
                    case '5': color = COLOR_RED; break;
                    case 'P': color = COLOR_WHITE; break;
                    case 'W': color = COLOR_YELLOW; break;
                    case 'R': color = COLOR_GRAY; break;
                    case '#': color = COLOR_WHITE; break;
                    default: color = COLOR_DEFAULT; break;
                    }

                    printc(padd(map_pos, pos), backbuf[i][j], color);
                }
                frontbuf[i][j] = backbuf[i][j];
            }
        }
    }


    // Ŀ�� ��ġ ǥ�� �Լ�
    void display_cursor(CURSOR cursor) {
        POSITION prev = cursor.previous;
        POSITION curr = cursor.current;

        // ���� ��ġ�� ĳ���Ϳ� ���� ���� ����
        char prev_ch = frontbuf[prev.row][prev.column];
        int prev_color = COLOR_DEFAULT;

        // ĳ���Ϳ� ���� ���� ����
        switch (prev_ch) {
        case 'B': prev_color = COLOR_BLUE; break;  // ����
        case 'H': prev_color = COLOR_BLUE; break;  // �Ϻ�����
        case '5': prev_color = COLOR_RED; break;   // �����̽� ������
        case 'P': prev_color = COLOR_WHITE; break; // ����
        case 'W': prev_color = COLOR_YELLOW; break; // �����
        case 'R': prev_color = COLOR_GRAY; break;   // ����
        case '#': prev_color = COLOR_WHITE; break;  // �׵θ�
        default: prev_color = COLOR_DEFAULT; break;
        }

        // ���� ��ġ�� ����� ���� ����
        printc(padd(map_pos, prev), prev_ch, prev_color);

        // ���� Ŀ�� ��ġ ǥ��
        char curr_ch = frontbuf[curr.row][curr.column];
        printc(padd(map_pos, curr), curr_ch, COLOR_CURSOR);
    }