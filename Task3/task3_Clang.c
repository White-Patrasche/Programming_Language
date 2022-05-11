#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

// ----- precede define -----//
#define TRUE 1
#define FALSE 0
#define LOCATION_NUMBER 6
#define ROOM_NUMBER 5
#define NAME_SIZE 30

// ----- function prototype start ----- //
// 임시적으로 주석처리 함 -> header.h에 저장
/*
void load_file();
void select_screen();
void ProgramEnd();
void Admin();
// void User();
void printEOF();
int isCancel(char*);
void flush();
*/

// ----- structure start ----- //
typedef struct {
    int max_people;
    char name[NAME_SIZE];
    int floor;
    int desk;
    int computer;
} study_room;

typedef struct {
    study_room room_number[ROOM_NUMBER];
} study_spot;

typedef struct {
    study_spot loc[LOCATION_NUMBER];
} study_cafe;

typedef struct {
    int location;
    int room_num;
    int max_people;
    char name[NAME_SIZE];
    int floor;
    int desk;
    int computer;
} study_room_data;

// ----- global variable ----- //
study_cafe* cafe;
study_room* default_room;

// ----- function implements ----- //
void load_file() {
    // Study_Reserve.txt 파일 읽어오기
    FILE* fp;
    if ((fp = fopen("./Study_Reserve.txt", "w+")) == NULL) {
        fprintf(stderr, "Error in fopen!\n");
        exit(1);
    }
    cafe = (study_cafe*)malloc(sizeof(study_cafe));
    //초기 값은 모두 -1로 지정
    memset(cafe, -1, sizeof(study_cafe));
    // Study_Reserve.txt파일을 읽으면서 study_cafe 구조체 저장할 변수 data 정의
    study_room_data* data = (study_room_data*)malloc(sizeof(study_room_data));
    //하나씩 읽으면서 cafe에 저장
    while (fread(data, sizeof(study_room_data), 1, fp) != 0) {
        cafe->loc[data->location].room_number[data->room_num].max_people =
            data->max_people;
        strcpy(cafe->loc[data->location].room_number[data->room_num].name,
               data->name);
        cafe->loc[data->location].room_number[data->room_num].floor =
            data->floor;
        cafe->loc[data->location].room_number[data->room_num].desk = data->desk;
        cafe->loc[data->location].room_number[data->room_num].computer =
            data->computer;
    }
    fclose(fp);
    // default room 초기화
    default_room = (study_room*)malloc(sizeof(study_room));
    default_room->max_people = 5;
    strcpy(default_room->name, "상현");
    default_room->floor = 4;
    default_room->desk = 3;
    default_room->computer = 1;
}

void select_screen() {
    int select = 0;
    printEOF();
    printf("----- Select Your Mod -----\n");
    printf("1. Admin Mod\n");
    printf("2. User Mod\n");
    printf("3. Program Exit\n");
    scanf("%d", &select);
    switch (select) {
        case 1:
            Admin();
            break;
        case 2:
            // User();
            break;
        case 3:
            ProgramEnd();
            break;
        default:
            break;
    }
    flush();
    select_screen();
}

void ProgramEnd() {
    printf("----- Program Ending-----\n");
    FILE* fp = fopen("./Study_Reserve.txt", "w");
    //-1 혹은 0으로 저장된 값은 없는 공간이라 제외하고 저장하기 위한 init
    study_room* zeroInit = (study_room*)malloc(sizeof(study_room));
    memset(zeroInit, 0, sizeof(study_room));
    study_room* minusInit = (study_room*)malloc(sizeof(study_room));
    memset(minusInit, -1, sizeof(study_room));

    study_room* roomptr;
    study_room_data* data = malloc(sizeof(study_room_data));
    for (int i = 0; i < LOCATION_NUMBER; i++) {
        for (int j = 0; j < ROOM_NUMBER; j++) {
            roomptr = &(cafe->loc[i].room_number[j]);
            //해당 스터디룸이 비었을 경우
            if (/* memcmp(roomptr, zeroInit, sizeof(study_room)) == 0 || */
                memcmp(roomptr, minusInit, sizeof(study_room)) == 0) {
                //해당 값은 건너뜀
            } else {
                data->location = i;
                data->room_num = j;
                data->max_people = roomptr->max_people;
                strcpy(data->name, roomptr->name);
                data->floor = roomptr->floor;
                data->desk = roomptr->desk;
                data->computer = roomptr->computer;
                fwrite(data, sizeof(study_room), 1, fp);
            }
        }
    }
    free(data);
    free(zeroInit);
    free(minusInit);
    fclose(fp);
    free(cafe);
    free(default_room);
    printf("----- Save Done! -----\n");
    exit(0);
}

void Admin() {
    int select = 0;
    printf("----- Select Your Setting-----\n");
    printf("1. Location Manage\n");
    printf("2. Location Space Manage\n");
    scanf("%d", &select);
    switch (select) {
        case 1:
            location_manage_select();  //지점 관리
            break;
        case 2:
            // location_space_manage();  //지점별 스터디 공간 관리
            break;
        case -1:
            select_screen();
            break;
        default:
            outOfInput();
            Admin();
            break;
    }
    //마지막에 select_screen으로 돌아가게 된다.(모든 함수 종료 시)
    select_screen();
}

void location_manage_select() {
    int select = 0;
    printf("----- Select Your Action -----\n");
    printf("1. 스터디 지점 plus\n");
    printf("2. 스터디 지점 edit/delete\n");
    scanf("%d", &select);
    if (select == EOF) {  //입력 취소
        select_screen();
    } else if (select == 1) {  //스터디 지점 추가
        location_manage_add();
    } else if (select == 2) {  //스터디 지점 수정
        location_manage_edit();
    } else {
        outOfInput();
        location_manage_select();
    }
    return;
}

void location_manage_add() {
    int select = 0;
    int validCheck[LOCATION_NUMBER];
    study_spot* temp = (study_spot*)malloc(sizeof(study_spot));
    memset(temp, -1, sizeof(study_spot));
    study_spot* tempptr;
    printf("----- Select Location -----\n");
    for (int i = 0; i < LOCATION_NUMBER; i++) {
        tempptr = &(cafe->loc[i]);  //해당 메모리에 값을 확인
        //-1로 이루어진 메모리와 일치한다면, 해당 스터디 지점은 비어있는 것
        if (memcmp(tempptr, temp, sizeof(study_spot)) == 0) {
            printf("%d. %d -> 스터디 지점 empty\n", i + 1, i + 1);
            validCheck[i] = -1;
        } else {  //메모리가 -1이 아님으로, 스터디 지점이 이미 존재
            printf("%d. %d -> 스터디 지점 already\n", i + 1, i + 1);
            validCheck[i] = 1;
        }
    }
    free(temp);

    scanf("%d", &select);
    //실제 데이터는 0~5까지 존재함으로 -1으로 실제 메모리에 접근
    if (select-- == EOF)
        select_screen();
    else if (select >= 0 && select < ROOM_NUMBER) {
        //비어있는 곳 고름
        if (validCheck[select] == -1) {
            tempptr = &(cafe->loc[select]);
            // studyroom malloc하고, for문으로 해당 index에 스터디룸 정보가
            // 존재하면 넘어가고 아니면 memset 0으로 지정(-1이 아닌 저장)
            memset(tempptr, 0, sizeof(study_spot));
        }
        //이미 있는 곳 고름
        else {
            printf("경고!\n이미 존재하는 공간에 대한 추가 요청을 했습니다!\n");
            printf("기능 선택 화면으로 돌아갑니다.\n");
            select_screen();
        }
    } else {  //이외의 것 선택 -> 재선택
        outOfInput();
        location_manage_add();
    }
    return;
}

void location_manage_edit() {
    int select = 0;
    int validCheck[LOCATION_NUMBER];
    study_spot* temp = (study_spot*)malloc(sizeof(study_spot));
    memset(temp, -1, sizeof(study_spot));
    study_spot* tempptr;
    printf("----- Select Location -----\n");
    for (int i = 0; i < LOCATION_NUMBER; i++) {
        tempptr = &(cafe->loc[i]);  //해당 메모리에 값을 확인
        //-1로 이루어진 메모리와 일치한다면, 해당 스터디 지점은 비어있는 것
        if (memcmp(tempptr, temp, sizeof(study_spot)) == 0) {
            printf("%d. %d -> 스터디 지점 empty\n", i + 1, i + 1);
            validCheck[i] = -1;
        } else {  //메모리가 -1이 아님으로, 스터디 지점이 이미 존재
            printf("%d. %d -> 스터디 지점 edit/delete 가능\n", i + 1, i + 1);
            validCheck[i] = 1;
        }
    }
    free(temp);

    scanf("%d", &select);
    //실제 데이터는 0~5까지 존재함으로 -1으로 실제 메모리에 접근
    if (select-- == EOF)
        select_screen();
    else if (select >= 0 && select < LOCATION_NUMBER) {
        //비어있는 곳 고름
        if (validCheck[select] == -1) {
            printf(
                "경고!\n존재하지 않는 공간에 대한 수정/삭제 요청을 "
                "했습니다!\n");
            printf("기능 선택 화면으로 돌아갑니다.\n");
            select_screen();
        }
        //이미 있는 곳 고름
        else {
            spot_manage(select);
        }
    } else {  //이외의 것 선택 -> 재선택
        outOfInput();
        location_manage_edit();
    }
    return;
}

void spot_manage(int select) {
    int select_RoomNum = 0;
    int validCheck[ROOM_NUMBER];
    printf("----- Select Study Room -----\n");
    study_room* tempptr;
    for (int i = 0; i < ROOM_NUMBER; i++) {
        tempptr = &(cafe->loc[select].room_number[i]);
        //해당 스터디 공간(룸)이 비어있음
        if (memcmp(tempptr, 0, sizeof(study_room)) == 0) {
            printf("%d. %d -> empty(추가 가능)\n", i + 1, i + 1);
            validCheck[i] = -1;
        }
        //해당 스터디 공간(룸)이 이미 존재(삭제/수정 가능)
        else {
            printf("%d. %d -> exist(수정/삭제 가능)\n", i + 1, i + 1);
            validCheck[i] = 1;
        }
    }
    scanf("%d", &select_RoomNum);
    if (select_RoomNum-- == EOF) {
        select_screen();
    } else if (select_RoomNum >= 0 && select_RoomNum < ROOM_NUMBER) {
        if (validCheck[select_RoomNum] == -1) {
            spot_add(select, select_RoomNum);
        } else {
            spot_edit(select, select_RoomNum);
        }
    } else {
        outOfInput();
        spot_manage(select_RoomNum);
    }
    return;
}

void spot_add(int select_LocNum, int select_RoomNum) {
    printf("----- 지점 추가 -----\n");
    int people = 0;
    study_room* stdroom = (study_room*)malloc(sizeof(stdroom));
    memcpy(stdroom, default_room, sizeof(study_room));
    printf("허용 인원 설정(1~10인실) : ");
    scanf("%d", &people);
    if (people == -1) {
        return;
    } else if (people >= 1 && people <= 10) {
        stdroom->max_people = people;
        memcpy(&cafe->loc[select_LocNum].room_number[select_RoomNum], stdroom,
               sizeof(study_room));
    } else {
        outOfInput();
        spot_add(select_LocNum, select_RoomNum);
    }
}

void spot_edit(int select_LocNum, int select_RoomNum) {
    int select = 0;
    printf("----- 지점 수정/삭제 ------");
    printf("1. Edit\n2. Delete\n");
    scanf("%d", &select);
    if (select == EOF)
        return;
    else if (select == 1) {
    } else if (select == 2) {
    } else {
        outOfInput();
        spot_edit(select_LocNum, select_RoomNum);
    }
}

void printEOF() {
    printf("-----If you want to cancel your input, please type EOF-----\n");
}

int isCancel(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == EOF) {
            return TRUE;
        }
    }
    return FALSE;
}

//다른 값이 들어온 경우 다시 입력 받음
void flush() {
    while (getchar() != '\n') {
    }
}

void outOfInput() {
    printf("Your input is not available.\n");
    printf("Please type again!\n");
    flush();
}

int main() {
    load_file();
    select_screen();
}