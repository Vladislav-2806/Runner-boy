#undef UNICODE
#pragma warning(disable: 4996)
#pragma comment(lib, "Msimg32.lib")

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

HFONT VarOfFonts[210];

HBITMAP blackRectangleBmp;
HBITMAP playerBmp[4];
HBITMAP carsBmp[3];
HBITMAP treesBmp[3];
HBITMAP beaverBmp;
HBITMAP roadBmp;
HBITMAP backgroundBmp;
HBITMAP menuBackBmp;
HBITMAP buttonsBmp[2];
HBITMAP buttonsOnBmp[2];

int windowSize[2] = { 1200, 640 };
int cameraCoords[2] = { 0, 0 };
int playerVector = 0;
int playerJump = 0;
int playerCoords[2] = { 200, 350 };
int gameOver = 0;
int showMenu = 1;
int settings[2];
int mouseCoords[2];
int click = 0;
int goToGame = 0;
int gameClose = 0;
int playerScore = 0;
int bestScore = 0;
int disklaimerPart = 2;

float sizeMaster = 1;
float playerGravity = 0;

RECT rct;

LRESULT WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp) {
    if (message == WM_DESTROY) PostQuitMessage(0);
    else if (message == WM_MOUSEMOVE) {
        mouseCoords[0] = LOWORD(lp);
        mouseCoords[1] = HIWORD(lp);
    }
    else if (message == WM_KEYDOWN) {
        if (wp == 68) playerVector = 1;
        else if (wp == 32) playerJump = 1;
    }
    else if (message == WM_LBUTTONDOWN) {
        click = 1;
    }
    else if (message == WM_LBUTTONUP) {
        click = 0;
    }
    else if (message == WM_KEYUP) {
        if (wp == 68) playerVector = 0;
        else if (wp == 32) playerJump = 0;
    }
    else DefWindowProcA(hwnd, message, wp, lp);
}

void SaveAll() {
    FILE* datas = fopen("Images/saves.txt", "w");

    char strParams[50] = "", sym[20] = "";

    sprintf(sym, "%d", settings[0]);
    strcat(strParams, sym);

    sprintf(sym, "%d#", settings[1]);
    strcat(strParams, sym);

    sprintf(sym, "%d", bestScore);
    strcat(strParams, sym);

    fprintf(datas, "%s", strParams);

    fclose(datas);
}

int CheckFile(char readStr[50]) {
    if (readStr[0] != 48 && readStr[0] != 49 || readStr[1] != 48 && readStr[1] != 49 || readStr[2] != 35) return 0;

    for (int i = 3; i < 50; i++){
        if (!readStr[i]) break;
        if (readStr[i] < 48 || readStr[i]>57) return 0;
    }

    return 1;
}

void InitAlmost() {
    FILE* fptr = fopen("Images/saves.txt", "r");

    if (!fptr) {
        SaveAll();

        return 0;
    }

    char readStr[70] = "";
    int size = 0;

    fgets(readStr, 50, fptr);

    if (!CheckFile(readStr)) {
        MessageBox(NULL, "Даже если вы и захотели поменять файл saves.txt, вы должны знать как он устроен! Ваш прогресс потерян.", "Я все вижу!", MB_OK);
        
        fclose(fptr);

        SaveAll();
    }

    settings[0] = readStr[0] == 49;
    settings[1] = readStr[1] == 49;

    for (int i = 3; i < 50; i++){
        if (!readStr[i]) break;

        size += 1;
    }
    float score = 0;

    if (size > 5) {
        MessageBox(NULL, "Вы и вправду пробежали такое расстояние в игре? В любом случае я сброшу ваш прогресс, так как число слишком большое.", "Ничего себе!", MB_OK);
        fclose(fptr);
        bestScore = 0;
        SaveAll();

        return 0;
    }

    for (int i = size-1; i > -1; i--) {
        score += (readStr[i+3]-48)*pow(10, abs(i- size+1));
    }

    bestScore = score;

    fclose(fptr);
}

void WinInit() {
    for (int i = 1; i < 200; ++i) {
        VarOfFonts[i] = CreateFontW(i, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 1, 0, 0, 0, 0, "Calibri");
    }

    blackRectangleBmp = LoadImage(NULL, "Images/black.bmp", IMAGE_BITMAP, windowSize[0], windowSize[1], LR_LOADFROMFILE);

    playerBmp[0] = LoadImage(NULL, "Images/stand.bmp", IMAGE_BITMAP, 112, 171, LR_LOADFROMFILE);
    playerBmp[1] = LoadImage(NULL, "Images/animateRun1.bmp", IMAGE_BITMAP, 112, 171, LR_LOADFROMFILE);
    playerBmp[2] = LoadImage(NULL, "Images/animateRun2.bmp", IMAGE_BITMAP, 112, 171, LR_LOADFROMFILE);
    playerBmp[3] = LoadImage(NULL, "Images/animateRun3.bmp", IMAGE_BITMAP, 112, 171, LR_LOADFROMFILE);

    beaverBmp = LoadImage(NULL, "Images/beaver.bmp", IMAGE_BITMAP, 63, 100, LR_LOADFROMFILE);

    carsBmp[0] = LoadImage(NULL, "Images/taxi.bmp", IMAGE_BITMAP, 230, 200, LR_LOADFROMFILE);
    carsBmp[1] = LoadImage(NULL, "Images/redCar.bmp", IMAGE_BITMAP, 250, 200, LR_LOADFROMFILE);
    carsBmp[2] = LoadImage(NULL, "Images/greenCar.bmp", IMAGE_BITMAP, 250, 200, LR_LOADFROMFILE);

    treesBmp[0] = LoadImage(NULL, "Images/tree1.bmp", IMAGE_BITMAP, 217, 474, LR_LOADFROMFILE);
    treesBmp[1] = LoadImage(NULL, "Images/tree2.bmp", IMAGE_BITMAP, 217, 318, LR_LOADFROMFILE);
    treesBmp[2] = LoadImage(NULL, "Images/tree3.bmp", IMAGE_BITMAP, 217, 438, LR_LOADFROMFILE);

    roadBmp = LoadImage(NULL, "Images/road.bmp", IMAGE_BITMAP, 700, 300, LR_LOADFROMFILE);
    backgroundBmp = LoadImage(NULL, "Images/sunny.bmp", IMAGE_BITMAP, windowSize[0], windowSize[1], LR_LOADFROMFILE);

    menuBackBmp = LoadImage(NULL, "Images/menuBack.bmp", IMAGE_BITMAP, windowSize[0], windowSize[1], LR_LOADFROMFILE);

    buttonsBmp[0] = LoadImage(NULL, "Images/exitButton.bmp", IMAGE_BITMAP, 400, 80, LR_LOADFROMFILE);
    buttonsBmp[1] = LoadImage(NULL, "Images/playButton.bmp", IMAGE_BITMAP, 400, 80, LR_LOADFROMFILE);

    buttonsOnBmp[0] = LoadImage(NULL, "Images/on.bmp", IMAGE_BITMAP, 50, 30, LR_LOADFROMFILE);
    buttonsOnBmp[1] = LoadImage(NULL, "Images/off.bmp", IMAGE_BITMAP, 50, 30, LR_LOADFROMFILE);

    InitAlmost();

    if (!settings[0]) {
        disklaimerPart = 0;
    }
    else disklaimerPart = 2;

    showMenu = !settings[1];

    if (!blackRectangleBmp || !playerBmp[0] || !playerBmp[1] || !playerBmp[2] || !playerBmp[3] || !beaverBmp || !carsBmp[0] || !carsBmp[1] || !carsBmp[2] || 
        !treesBmp[0] || !treesBmp[1] || !treesBmp[2] || !roadBmp || !backgroundBmp || !menuBackBmp || !buttonsBmp[0] || !buttonsBmp[1] || !buttonsOnBmp[0] || !buttonsOnBmp[1]) {
        MessageBox(NULL, "Файлы игры были изменены или удалены. Просьба не трогать их, во избежание потери ваших данных. Для решения проблемы обратитесь к разработчку.", "Внимение!", MB_OK);
        exit(1);
    }
}

void MyRectangle(HDC dc, int x1, int y1, int x2, int y2) {
    Rectangle(dc, cameraCoords[0] + x1 * sizeMaster, cameraCoords[1] + y1 * sizeMaster, cameraCoords[0] + x2 * sizeMaster, cameraCoords[1] + y2 * sizeMaster);
}

void MyEllipse(HDC dc, int x1, int y1, int x2, int y2) {
    Ellipse(dc, cameraCoords[0] + x1 * sizeMaster, cameraCoords[1] + y1 * sizeMaster, cameraCoords[0] + x2 * sizeMaster, cameraCoords[1] + y2 * sizeMaster);
}

void MyLine(HDC dc, int x1, int y1, int x2, int y2) {
    MoveToEx(dc, cameraCoords[0] + x1 * sizeMaster, cameraCoords[1] + y1 * sizeMaster, 0);
    LineTo(dc, cameraCoords[0] + x2 * sizeMaster, cameraCoords[1] + y2 * sizeMaster);
}

void MyText(HDC dc, int x1, int y1, char text[100], int size) {
    size *= sizeMaster;
    SelectObject(dc, VarOfFonts[intLimit(size, 100)]);
    TextOut(dc, cameraCoords[0] + x1 * sizeMaster, cameraCoords[1] + y1 * sizeMaster, text, strlen(text));
}

void MyPhoto(HDC dc, int x, int y, int sizeX, int sizeY, HDC photoDC) {
    BitBlt(dc, cameraCoords[0] + x * sizeMaster, cameraCoords[1] + y * sizeMaster, sizeX, sizeY, photoDC, 0, 0, SRCCOPY);
}

void MyRoundRect(HDC dc, int x1, int y1, int x2, int y2, int wd, int hd) {
    RoundRect(dc, cameraCoords[0] + x1 * sizeMaster, cameraCoords[1] + y1 * sizeMaster, cameraCoords[0] + x2 * sizeMaster, cameraCoords[1] + y2 * sizeMaster, wd * sizeMaster, hd * sizeMaster);
}

void MyAlphaBlend(HDC dc, int x, int y, int sizeX, int sizeY, HDC objDC, BLENDFUNCTION fnc) {
    AlphaBlend(dc, cameraCoords[0] + x * sizeMaster, cameraCoords[1] + y * sizeMaster, sizeX, sizeY, objDC, 0, 0, sizeX, sizeY, fnc);
}

int intLimit(int number, int maxNumber) {
    if (number > maxNumber) return maxNumber;
    return number;
}

void CreateBeautyText(HDC dc, int x, int y, int textSize, int splitSize, char text[2000]) {
    int numString = 0, numSplit = 0;
    char helpText[2000] = "";
    char stringText[200] = "", sym[5] = "", sym2[5] = "";
    char splitText[200][200];
    strcpy(helpText, text);
    strcpy(splitText[0], "");

    for (int i = 0; i < strlen(helpText); i++) {
        sprintf(sym, "%c", helpText[i]);
        sprintf(sym2, "%c", helpText[i + 1]);

        int check = helpText[i];

        if (check == 10) {
            numString += 1;
            strcpy(splitText[numString], "!s");
            numString += 1;
            strcpy(splitText[numString], "");
            continue;
        }

        if (strcmp(sym, " ") == 0) {
            numString += 1;
            strcpy(splitText[numString], "");
            continue;
        }
        strcat(splitText[numString], sym);
    }

    for (int k = 0; k < numString + 1; k++) {
        if (!strcmp(splitText[k], "!s")) {
            if (!numSplit) continue;
            MyText(dc, x, y + textSize * numSplit, stringText, textSize);
            numSplit += 1;
            strcpy(stringText, "");
            continue;
        }

        strcat(stringText, splitText[k]);
        strcat(stringText, " ");
        if (k < numString && strlen(stringText) + strlen(splitText[k + 1])>splitSize) {
            MyText(dc, x, y + textSize * numSplit, stringText, textSize);
            numSplit += 1;
            strcpy(stringText, "");
        }
        else if (k >= numString) MyText(dc, x, y + textSize * numSplit, stringText, textSize);
    }
}

int ShowBeforeParticles(HDC dc) {
    static int pointsParams[50][2], minusColor = 1, vectorColor = 1, countRepears = 0;

    if (!pointsParams[0][0]) {
        for (int i = 0; i < 30; i++) {
            pointsParams[i][0] = ((rand()+time(NULL)) % windowSize[0]);
            pointsParams[i][1] = ((rand()+time(NULL)) % windowSize[1]);
        }
    }

    if (minusColor > 0) {
        minusColor += 30*vectorColor;
        if (minusColor >= 255) vectorColor = -vectorColor;
    }
    else if (!countRepears) {
        countRepears += 1;
        minusColor = 1;
        vectorColor = 1;

        for (int i = 0; i < 30; i++) {
            pointsParams[i][0] = ((rand()+time(NULL)) % windowSize[0]);
            pointsParams[i][1] = ((rand()+time(NULL)) % windowSize[1]);
        }
    }
    else{
        return 1;
    }

    for (int i = 0; i < 30; i++) {
        for (int j = 30; j > 0; j--) {
            int color = intLimit((minusColor+30)*(4.0/j/j), 255);

            SelectObject(dc, GetStockObject(DC_BRUSH));
            SetDCBrushColor(dc, RGB(color, color, color));
            SelectObject(dc, GetStockObject(DC_PEN));
            SetDCPenColor(dc, RGB(color, color, color));

            MyEllipse(dc, pointsParams[i][0]-j, pointsParams[i][1]-j, pointsParams[i][0]+j, pointsParams[i][1]+j);
        }
    }

    return 0;
}

int ShowDisclaimer(HDC dc) {
    static int minusColor = 255, waitDelay = 0, animateState = 0;

    SetTextColor(dc, RGB(236, 28, 36));
    SetBkColor(dc, RGB(0, 0, 0));

    MyText(dc, 380, 80, "Disclaimer", 100);

    SetTextColor(dc, RGB(255, 255, 255));

    CreateBeautyText(dc, 250, 250, 40, 40, "Управление в игре осуществляется клавишами D и Space. Игра предназначена для детей от 6 лет. Все права пренадлежат Глебу, бла, бла, бла, короче кто это читает?");

    MyText(dc, 500, 560, "Copyright Bread", 25);

    BLENDFUNCTION fnc;
    fnc.BlendOp = AC_SRC_OVER;
    fnc.BlendFlags = 0;
    fnc.SourceConstantAlpha = minusColor;
    fnc.AlphaFormat = 0;

    HDC blackDC = CreateCompatibleDC(dc);
    SelectObject(blackDC, blackRectangleBmp);

    MyAlphaBlend(dc, 0, 0, windowSize[0], windowSize[1], blackDC, fnc);

    DeleteDC(blackDC);

    if (minusColor > 10 && !animateState) minusColor -= 10;
    else if (waitDelay < 100) {
        minusColor = 0;
        waitDelay += 1;
        animateState = 1;
    }
    else if (minusColor < 245) minusColor += 10;
    else return 1;

    return 0;
}

void ShowPlayer(HDC dc) {
    static int animateState = 0;

    HDC playerDC = CreateCompatibleDC(dc);

    if (!playerVector) SelectObject(playerDC, playerBmp[0]);
    else {
        playerCoords[0] += 3;
        playerScore += 1;

        SelectObject(playerDC, playerBmp[1 + (animateState / 10)]);
        animateState += 1;
        if (animateState >= 30) animateState = 0;
    }

    if (playerJump && !playerGravity) {
        playerJump = 0;
        playerGravity = -10;
    }

    if (playerGravity || playerCoords[1] < 350) {
        playerGravity += 0.2;
        playerCoords[1] += playerGravity;

        if (playerCoords[1] > 350) {
            playerCoords[1] = 350;
            playerGravity = 0;
        }
    }

    TransparentBlt(dc, 300, playerCoords[1], 112, 171, playerDC, 0, 0, 112, 171, RGB(14, 209, 69));

    if (gameOver) {
        animateState = 0;
        playerJump = 0;
        playerCoords[0] = 200;
        playerCoords[1] = 350;
        playerGravity = 0;
        playerScore = 0;
    }

    DeleteDC(playerDC);
}

void ShowRoad(HDC dc) {
    static int roadCoords[4];

    HDC roadDC = CreateCompatibleDC(dc);
    SelectObject(roadDC, roadBmp);

    for (int i = 0; i < 4; i++){
        int x = roadCoords[i] - playerCoords[0];

        if (!roadCoords[i]) {
            roadCoords[i] = 1 + i * 700;
        }
        else if(x < -700) {
            for (int j = 0; j < 4; j++) {
                if (roadCoords[i] < roadCoords[j]) {
                    roadCoords[i] = roadCoords[j];
                }
            }
            roadCoords[i] += 700;
        }

        BitBlt(dc, x, 305, 700, 300, roadDC, 0, 0, SRCCOPY);
    }

    if (gameOver) {
        roadCoords[0] = 0;
        roadCoords[1] = 0;
        roadCoords[2] = 0;
        roadCoords[3] = 0;
    }

    DeleteDC(roadDC);
}

void ShowTrees(HDC dc) {
    static int treesCoords[20][3], treesCoords2[20][3];
    static int treesSizes[3] = { 474, 318, 438 };

    HDC treeDC = CreateCompatibleDC(dc);

    for (int i = 0; i < 20; i++) {
        int x = treesCoords2[i][0] - (playerCoords[0])/1.5;

        if (!treesCoords2[i][0]) {
            treesCoords2[i][0] = 1 + i * 200;
            treesCoords2[i][2] = (rand()+time(NULL)) % 3;
            treesCoords2[i][1] = 1 - 102 * (treesCoords2[i][2] == 2) - 142 * (treesCoords2[i][2] == 0);
        }
        else if (x < -400) {
            for (int j = 0; j < 20; j++) {
                if (treesCoords2[i][0] < treesCoords2[j][0]) {
                    treesCoords2[i][0] = treesCoords2[j][0];
                }
            }
            treesCoords2[i][0] += 200;
            treesCoords2[i][2] = (rand()+time(NULL)) % 3;
            treesCoords2[i][1] = 1 - 102 * (treesCoords2[i][2] == 2) - 142 * (treesCoords2[i][2] == 0);
        }
        SelectObject(treeDC, treesBmp[treesCoords2[i][2]]);
        TransparentBlt(dc, x, treesCoords2[i][1], 217, treesSizes[treesCoords2[i][2]], treeDC, 0, 0, 217, treesSizes[treesCoords2[i][2]], RGB(14, 209, 69));
    }

    BLENDFUNCTION fnc;
    fnc.BlendOp = AC_SRC_OVER;
    fnc.BlendFlags = 0;
    fnc.SourceConstantAlpha = 150;
    fnc.AlphaFormat = 0;

    HDC blackDC = CreateCompatibleDC(dc);
    SelectObject(blackDC, blackRectangleBmp);

    MyAlphaBlend(dc, 0, 0, windowSize[0], 310, blackDC, fnc);

    DeleteDC(blackDC);

    for (int i = 0; i < 20; i++){
        int x = treesCoords[i][0] - playerCoords[0];

        if (!treesCoords[i][0]) {
            treesCoords[i][0] = 1 + i * 200;
            treesCoords[i][2] = (rand()+time(NULL)) % 3;
            treesCoords[i][1] = 1-102 * (treesCoords[i][2] == 2) - 142 * (treesCoords[i][2] == 0);
        }
        else if (x<-400) {
            for (int j = 0; j < 20; j++){
                if (treesCoords[i][0] < treesCoords[j][0]) {
                    treesCoords[i][0] = treesCoords[j][0];
                }
            }
            treesCoords[i][0] += 200;
            treesCoords[i][2] = (rand()+time(NULL)) % 3;
            treesCoords[i][1] = 1 - 102 * (treesCoords[i][2] == 2) - 142 * (treesCoords[i][2] == 0);
        }
        SelectObject(treeDC, treesBmp[treesCoords[i][2]]);
        TransparentBlt(dc, x, treesCoords[i][1], 217, treesSizes[treesCoords[i][2]], treeDC, 0, 0, 217, treesSizes[treesCoords[i][2]], RGB(14, 209, 69));
    }

    if (gameOver) {
        for (int i = 0; i < 20; i++){
            treesCoords[i][0] = 0;
            treesCoords[i][1] = 0;
            treesCoords[i][2] = 0;

            treesCoords2[i][0] = 0;
            treesCoords2[i][1] = 0;
            treesCoords2[i][2] = 0;
        }
    }

    DeleteDC(treeDC);
}

void ShowCars(HDC dc) {
    static int carsCoords[20][3], carsJump = 1;
    static int carsSizes[3] = { 230, 250, 250 };

    if (gameOver) {
        for (int i = 0; i < 20; ++i) {
            carsCoords[i][0] = 0;
            carsCoords[i][1] = 0;
            carsCoords[i][2] = 0;
        }
        return 0;
    }

    HDC carDC = CreateCompatibleDC(dc);

    for (int i = 0; i < 20; i++) {
        int x = carsCoords[i][0] - playerCoords[0];

        if (!carsCoords[i][0]) {
            carsCoords[i][0] = 2000 + i * 800-(rand()+time(NULL))%400;
            carsCoords[i][2] = (rand()+time(NULL)) % 3;
            carsCoords[i][1] = 350;
        }
        else if (x < -400) {
            for (int j = 0; j < 20; j++) {
                if (carsCoords[i][0] < carsCoords[j][0]) {
                    carsCoords[i][0] = carsCoords[j][0];
                }
            }
            carsCoords[i][0] += 800 - (rand()+time(NULL)) % 400;
            carsCoords[i][2] = (rand()+time(NULL)) % 3;
            carsCoords[i][1] = 350;
        }
        if (300 > x+60 && 300 < x + carsSizes[carsCoords[i][2]]-50 && playerCoords[1] + 171 > 350 && playerCoords[1] + 171 < 550 || 
            412 > x+60 && 412 < x + carsSizes[carsCoords[i][2]]-50 && playerCoords[1] + 171 > 350 && playerCoords[1] + 171 < 550) {
            playerGravity = 0;
            if (playerCoords[1] + 171 > 360 && !gameClose) {
                gameClose = 1;
                SaveAll();
            }
        }

        carsCoords[i][0] -= 2;
        carsCoords[i][1] += carsJump * 3;

        SelectObject(carDC, carsBmp[carsCoords[i][2]]);
        TransparentBlt(dc, x, carsCoords[i][1], carsSizes[carsCoords[i][2]], 200, carDC, 0, 0, carsSizes[carsCoords[i][2]], 200, RGB(14, 209, 69));
    }

    carsJump = -carsJump;

    DeleteDC(carDC);
}

int ShowDeadText(HDC dc) {
    static int minusColor = 255, lineSize = 0, timerDelay = 0, minusColor2 = 0;
    static float lineGravity = 50;

    SetTextColor(dc, RGB(255, 0, 0));
    SetBkColor(dc, RGB(0, 0, 0));

    MyText(dc, 450, 150, "Помер", 200);
    MyText(dc, 520, 560, "Плаки плаки", 25);

    BLENDFUNCTION fnc;
    fnc.BlendOp = AC_SRC_OVER;
    fnc.BlendFlags = 0;
    fnc.SourceConstantAlpha = minusColor;
    fnc.AlphaFormat = 0;

    HDC blackDC = CreateCompatibleDC(dc);
    SelectObject(blackDC, blackRectangleBmp);

    MyAlphaBlend(dc, 0, 0, windowSize[0], windowSize[1], blackDC, fnc);

    for (int i = 50; i > 0; i--) {
        int color = 255.0 / i;

        SelectObject(dc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(dc, RGB(color, 0, 0));
        SelectObject(dc, GetStockObject(DC_PEN));
        SetDCPenColor(dc, RGB(color, 0, 0));

        MyRectangle(dc, 0, 320 - i, lineSize, 323 + i);
    }

    fnc.SourceConstantAlpha = minusColor2;
    MyAlphaBlend(dc, 0, 0, windowSize[0], windowSize[1], blackDC, fnc);

    DeleteDC(blackDC);

    if (lineSize < windowSize[0]) {
        lineSize += lineGravity;
        lineGravity -= 1;
    }
    else if (minusColor > 10) minusColor -= 10;
    else if (timerDelay < 50) {
        minusColor = 0;
        timerDelay += 1;
    }
    else if (minusColor2 < 220) minusColor2 += 30;
    else {
        minusColor2 = 0;
        minusColor = 255;
        lineSize = 0;
        timerDelay = 0;
        lineGravity = 50;
        return 1;
    }

    return 0;
}

void ShowScore(HDC dc) {
    SetTextColor(dc, RGB(255, 255, 255));
    SetBkMode(dc, TRANSPARENT);

    char sym[10], scoreText[50] = "Расстояние: ";
    int score = playerScore / 10.0;

    sprintf(sym, "%d", score);
    strcat(scoreText, sym);
    strcat(scoreText, " м");

    MyText(dc, 0, 578, scoreText, 24);

    if (score > bestScore) {
        bestScore = score;
    }
}

void ShowRain(HDC dc) {
    static int rainActivate = 0, minusColor = 0, beaverActivate = 0, rainGo = 0, timerDelay = 0;
    static int rainPoints[50][3], beaverCoords[2];
    COLORREF colors[2] = {RGB(155, 218, 218), RGB(0, 0, 255)};

    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(255, 255, 255));

    if (rainActivate && minusColor < 100) {
        minusColor += 10;
    }

    if (!rainGo && (rand()+time(NULL)) % 5000 == 4) {
        rainActivate = 1;
        rainGo = 1;
    }
    if (timerDelay > 1000) {
        timerDelay = 0;
        rainActivate = 0;
    }
    if (rainActivate) {
        timerDelay += 1;
    }

    if (rainActivate && (rand()+time(NULL)) % 500 == 3 && !beaverActivate) {
        beaverActivate = 1;
        beaverCoords[0] = (rand()+time(NULL)) % windowSize[0];
        beaverCoords[1] = -300;
    }
    if (beaverActivate) {
        if (beaverCoords[1] < windowSize[1]) beaverCoords[1] += 10;
        else beaverActivate = 0;

        HDC beaverDC = CreateCompatibleDC(dc);

        SelectObject(beaverDC, beaverBmp);
        TransparentBlt(dc, beaverCoords[0], beaverCoords[1], 63, 100, beaverDC, 0, 0, 63, 100, RGB(14, 209, 69));

        DeleteDC(beaverDC);
    }

    if (rainGo) {
        int min = windowSize[1];
        for (int i = 0; i < 50; i++){
            if (!rainPoints[i][0]) {
                rainPoints[i][0] = (rand()+time(NULL)) % windowSize[0];
                rainPoints[i][1] = -i*50;
                rainPoints[i][2] = (rand()+time(NULL))%2;
            }
            if (rainPoints[i][1] > windowSize[1] && rainActivate) {
                for (int j = 0; j < 50; j++){
                    if (rainPoints[j][1] < rainPoints[i][1]) {
                        rainPoints[i][1] = rainPoints[j][1];
                    }
                }
                rainPoints[i][1] -= 50;
                rainPoints[i][0] = (rand()+time(NULL)) % windowSize[0];
                rainPoints[i][2] = (rand()+time(NULL)) % 2;
            }
            if (min> rainPoints[i][1]) {
                min = rainPoints[i][1];
            }
            rainPoints[i][1] += 10;
            
            SelectObject(dc, GetStockObject(DC_BRUSH));
            SetDCBrushColor(dc, colors[rainPoints[i][2]]);

            MyRectangle(dc, rainPoints[i][0], rainPoints[i][1], rainPoints[i][0]+15, rainPoints[i][1]+30);
        }
        if (!rainActivate && min > windowSize[1]-50) {
            rainGo = 0;
            for (int i = 0; i < 50; i++) {
                rainPoints[i][0] = 0;
                rainPoints[i][1] = 0;
                rainPoints[i][2] = 0;
            }
        }
    }

    if (gameOver) {
        rainActivate = 0;
        minusColor = 0;
        beaverActivate = 0;
        rainGo = 0;
        timerDelay = 0;

        beaverCoords[0] = 0;
        beaverCoords[1] = 0;
        for (int i = 0; i < 50; i++) {
            rainPoints[i][0] = 0;
            rainPoints[i][1] = 0;
            rainPoints[i][2] = 0;
        }
    }
}

void MainGame(HDC dc) {
    static int blackState = 0;

    if (!blackState) {
        HDC backDC = CreateCompatibleDC(dc);

        SelectObject(backDC, backgroundBmp);
        BitBlt(dc, 0, 0, windowSize[0], windowSize[1], backDC, 0, 0, SRCCOPY);

        DeleteDC(backDC);

        ShowRoad(dc);

        ShowTrees(dc);

        ShowPlayer(dc);

        ShowCars(dc);

        ShowRain(dc);

        ShowScore(dc);
    }

    static int minusColor = 255;

    BLENDFUNCTION fnc;
    fnc.BlendOp = AC_SRC_OVER;
    fnc.BlendFlags = 0;
    fnc.SourceConstantAlpha = minusColor;
    fnc.AlphaFormat = 0;

    HDC blackDC = CreateCompatibleDC(dc);
    SelectObject(blackDC, blackRectangleBmp);

    MyAlphaBlend(dc, 0, 0, windowSize[0], windowSize[1], blackDC, fnc);

    DeleteDC(blackDC);

    MyAlphaBlend(dc, 0, 0, windowSize[0], windowSize[1], blackDC, fnc);

    DeleteDC(blackDC);

    if (gameClose) {
        if (minusColor < 215) minusColor += 40;
        else {
            minusColor = 255;
            if (!gameOver) {
                gameOver = 1;
                return 0;
            }
        }

        if (minusColor == 255) {
            blackState = 1;
            if (ShowDeadText(dc)) {
                showMenu = 1;
                gameOver = 0;
                gameClose = 0;
                blackState = 0;
            }
        }
    }
    else if (minusColor > 40) {
        minusColor -= 40;
    }
    else minusColor = 0;
}

void MainMenu(HDC dc) {
    static int buttonsParams[2] = { 0, 0 };

    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(0, 255, 0));
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 255, 0));

    Rectangle(dc, 0, 0, windowSize[0], windowSize[1]);

    HDC backDC = CreateCompatibleDC(dc);

    SelectObject(backDC, menuBackBmp);
    BitBlt(dc, 0, 0, windowSize[0], windowSize[1], backDC, 0, 0, SRCCOPY);

    SelectObject(backDC, buttonsBmp[0]);
    BitBlt(dc, 400+10* buttonsParams[0], 250, 400, 80, backDC, 0, 0, SRCCOPY);

    SelectObject(backDC, buttonsBmp[1]);
    BitBlt(dc, 400 + 10 * buttonsParams[1], 400, 400, 80, backDC, 0, 0, SRCCOPY);

    SelectObject(backDC, buttonsOnBmp[settings[0]]);
    TransparentBlt(dc, 1110, 530, 50, 30, backDC, 0, 0, 50, 30, RGB(0, 0, 0));

    SelectObject(backDC, buttonsOnBmp[settings[1]]);
    TransparentBlt(dc, 1110, 560, 50, 30, backDC, 0, 0, 50, 30, RGB(0, 0, 0));

    DeleteDC(backDC);

    if (mouseCoords[0] > 1100 && mouseCoords[0] < 1160 && mouseCoords[1]>530 && mouseCoords[1] < 560 && click) {
        settings[0] = !settings[0];
        SaveAll();
        click = 0;
    }
    else if (mouseCoords[0] > 1100 && mouseCoords[0] < 1160 && mouseCoords[1]>560 && mouseCoords[1] < 590 && click) {
        settings[1] = !settings[1];
        SaveAll();
        click = 0;
    }

    if (mouseCoords[0] > 400 && mouseCoords[0] < 800 && mouseCoords[1]>250 && mouseCoords[1] < 330) {
        buttonsParams[0] = 1;
        if (click) {
            click = 0;
            goToGame = 1;
        }
    }
    else buttonsParams[0] = 0;

    if (mouseCoords[0] > 400 && mouseCoords[0] < 800 && mouseCoords[1]>400 && mouseCoords[1] < 460) {
        buttonsParams[1] = 1;
        if (click) {
            exit(1);
        }
    }
    else buttonsParams[1] = 0;

    SetTextColor(dc, RGB(255, 255, 255));
    SetBkMode(dc, TRANSPARENT);

    char sym[10], scoreText[50] = "Лучшее расстояние: ";
    sprintf(sym, "%d", bestScore);
    strcat(scoreText, sym);
    strcat(scoreText, " м");

    MyText(dc, 3, 0, scoreText, 30);

    MyText(dc, 850, 530, "Показывать заставку: ", 30);
    MyText(dc, 740, 560, "Показывать меню при запуске: ", 30);

    MyText(dc, 370, 50, "Runner boy", 170);

    static int minusColor = 255;

    if (goToGame) {
        if (minusColor < 235) minusColor += 30;
        else {
            minusColor = 255;
            goToGame = 0;
            showMenu = 0;
        }
    }
    else {
        if (minusColor > 30) minusColor -= 30;
        else {
            minusColor = 0;
        }
    }

    BLENDFUNCTION fnc;
    fnc.BlendOp = AC_SRC_OVER;
    fnc.BlendFlags = 0;
    fnc.SourceConstantAlpha = minusColor;
    fnc.AlphaFormat = 0;

    HDC blackDC = CreateCompatibleDC(dc);
    SelectObject(blackDC, blackRectangleBmp);

    MyAlphaBlend(dc, 0, 0, windowSize[0], windowSize[1], blackDC, fnc);

    DeleteDC(blackDC);
}

void ClearDisplay(HDC dc) {
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(0, 0, 0));
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));

    Rectangle(dc, 0, 0, windowSize[0], windowSize[1]);
}

int WinShow(HDC dc) {
    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM = CreateCompatibleBitmap(dc, windowSize[0], windowSize[1]);
    SelectObject(memDC, memBM);
       
    ClearDisplay(memDC);

    if (!disklaimerPart && ShowBeforeParticles(memDC)) {
        disklaimerPart = 1;
    }
    else if(disklaimerPart ==1 && ShowDisclaimer(memDC)) {
        disklaimerPart = 2;
    }  
    else if (disklaimerPart == 2) {
        if (showMenu) MainMenu(memDC);
        else MainGame(memDC);
    }

    BitBlt(dc, 0, 0, windowSize[0], windowSize[1], memDC, 0, 0, SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(memBM);
}

int WinMain() {
    WNDCLASSA wcl;
    memset(&wcl, 0, sizeof(WNDCLASSA));

    wcl.lpszClassName = "my Window";
    wcl.lpfnWndProc = WndProc;

    RegisterClassA(&wcl);

    HWND hwnd = CreateWindow("my Window", "Runner boy", WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX),
        100, 100, windowSize[0], windowSize[1], NULL, NULL, NULL, NULL);

    ShowWindow(hwnd, SW_SHOWNORMAL);

    HDC dc = GetDC(hwnd);

    WinInit();

    MSG msg;

    while (1) {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
            DispatchMessage(&msg);
            TranslateMessage(&msg);
            if (msg.message == WM_QUIT) break;
        }
        else {
            WinShow(dc);
            Sleep(1);
        }
    }
}
