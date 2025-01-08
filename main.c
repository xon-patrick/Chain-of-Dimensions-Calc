#include <raylib.h>
#include <stdlib.h>

// Convert text to float; function needed for raygui
float TextToFloat(const char *text) {
    return atof(text);
}

#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define MAX_DIMENSIONS 5

typedef struct {
    float nominal;
    float upper_deviation;
    float lower_deviation;
} Dimension;

typedef struct {
    char* abatere_superioara;
    char* cota_maritoare;
    char* abatere_inferioara;
    char* cote_micsoratoare;
    char* cota_micsoratoare;
    char* calculate;
    char* cota_calculata;
} Language;

Language romanian = {
    "Abatere superioara",
    "Cota maritoare",
    "Abatere inferioara",
    "Cote micsoratoare",
    "Cota micsoratoare",
    "Calculeaza",
    "Cota Calculata (L)"
};

Language english = {
    "Upper Deviation",
    "Increasing Dimension",
    "Lower Deviation",
    "Reducing Dimensions",
    "Reducing Dim",
    "Calculate",
    "Calculated Dimension (L)"
};

Language german = {
    "Obere Abweichung",
    "Erhöhende Dimension",
    "Untere Abweichung",
    "Reduzierende Dimensionen",
    "Reduzierende Dim",
    "Berechnen",
    "Berechnete Dimension (L)"
};

Language* currentLanguage = &romanian;

Dimension dimensions[MAX_DIMENSIONS];
int dimension_count = 0;

int activeTextBox = -1; // active text box

float L_nominal = 0;
float max_deviation = 0;
float min_deviation = 0;

void calculateLength(float C1/*increasing dim*/, float dev_C1_max, float dev_C1_min, int num_reducing_dimensions, float reducing_dims[], float dev_reducing_dims[], float* L_nominal, float* max_deviation, float* min_deviation) {
    float sum_reducing_dims = 0;
    float min_dev_reducing_dims = 0;
    float max_dev_reducing_dims = 0;

    for (int i = 0; i < num_reducing_dimensions; i++) {
        sum_reducing_dims += reducing_dims[i];
        min_dev_reducing_dims += dev_reducing_dims[i * 2 + 1];  // abaterile inferioare
        max_dev_reducing_dims += dev_reducing_dims[i * 2];  // abaterile superioare
    }

    // valoare nominala a cotei dorite
    *L_nominal = C1 - sum_reducing_dims;

    // abatere maxima
    *max_deviation = dev_C1_max - min_dev_reducing_dims;

    // abatere minima
    *min_deviation = dev_C1_min - max_dev_reducing_dims;
}

void DrawRepresentation(float C1, float reducing_dims[], int num_reducing_dimensions, float L, float totalWidth, float barHeight) {
    Color segmentColors[] = {DARKBLUE, DARKGREEN, DARKPURPLE, ORANGE, DARKBROWN};
    Color lColor = RED;

    float sum_reducing_dims = 0;
    for (int i = 0; i < num_reducing_dimensions; i++) {
        sum_reducing_dims += reducing_dims[i];
    }

    // vallidare proportii
    float total = sum_reducing_dims + L;
    if (total <= 0 || C1 <= 0) return; // Avoid division by zero or negative values

    // starting coords
    float startX = 400;
    float startY = 450;

    // segmente prop
    for (int i = 0; i < num_reducing_dimensions; i++) {
        float segmentWidth = (reducing_dims[i] / C1) * totalWidth;
        DrawRectangle(startX, startY, segmentWidth, barHeight, segmentColors[i % 5]); // Cycle through colors

        // Label
        DrawText(TextFormat("c%d", i + 1), 
                 startX + segmentWidth / 2 - MeasureText(TextFormat("c%d", i + 1), 20) / 2, 
                 startY - 25, 
                 20, BLACK);

        startX += segmentWidth;
    }

    // L
    float LWidth = (L / C1) * totalWidth;
    DrawRectangle(startX, startY, LWidth, barHeight, RED);

    // cotat maritoare
    DrawRectangle(400, 500, totalWidth, 30, DARKGRAY);
    // outline
    DrawRectangleLinesEx((Rectangle){400, 450, totalWidth, 80}, 3, BLACK);

    //labels
    DrawText("L", startX + LWidth / 2 - MeasureText("L", 20) / 2, startY - 25, 20, BLACK);
    DrawText(currentLanguage->cota_maritoare, 400 + totalWidth / 2 - MeasureText(currentLanguage->cota_maritoare, 20) / 2, 540, 20, BLACK);
}

void DrawGUI() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20); // Set text size globally

    static int language = 0;
    static bool dropdownEditMode = false;
    const char* languages[] = {"RO ", "ENG ", "DE "};

    if (GuiDropdownBox((Rectangle){ GetScreenWidth() - 110, 10, 80, 30 }, "RO;ENG;DE", &language, dropdownEditMode)) {
        dropdownEditMode = !dropdownEditMode;
    }

    if (language == 0) {
        currentLanguage = &romanian;
    } else if(language == 1){
        currentLanguage = &english;
    } else {
        currentLanguage = &german;
    }

    GuiLabel((Rectangle){ 210, 90, 200, 30 }, currentLanguage->abatere_superioara);
    static char abatere_superioara[16] = "";
    if (GuiTextBox((Rectangle){ 130, 90, 70, 30 }, abatere_superioara, 16, activeTextBox == 1)) activeTextBox = 1;

    GuiLabel((Rectangle){ 20, 20, 200, 20 }, currentLanguage->cota_maritoare);
    static char cota_maritoare[16] = "";
    if (GuiTextBox((Rectangle){ 30, 120, 100, 40 }, cota_maritoare, 16, activeTextBox == 0)) activeTextBox = 0;

    GuiLabel((Rectangle){ 210, 160, 200, 20 }, currentLanguage->abatere_inferioara);
    static char abatere_inferioara[16] = "";
    if (GuiTextBox((Rectangle){ 130, 160, 70, 30 }, abatere_inferioara, 16, activeTextBox == 2)) activeTextBox = 2;

    GuiLabel((Rectangle){ 20, 210, 230, 30 }, currentLanguage->cote_micsoratoare);
    if (GuiButton((Rectangle){ 240, 210, 40, 40 }, "+")) {
        if (dimension_count < MAX_DIMENSIONS) {
            dimension_count++;
        }
    }
    if (GuiButton((Rectangle){ 290, 210, 40, 40 }, "-")) {
        if (dimension_count > 0) {
            dimension_count--;
        }
    }

    static char nominal[MAX_DIMENSIONS][16] = {0};
    static char upper_dev[MAX_DIMENSIONS][16] = {0};
    static char lower_dev[MAX_DIMENSIONS][16] = {0};

    float reducing_dims[MAX_DIMENSIONS] = {0};
    float dev_reducing_dims[MAX_DIMENSIONS * 2] = {0};

    for (int i = 0; i < dimension_count; i++) {
        GuiLabel((Rectangle){ 120, 270 + i * 150, 200, 30 }, TextFormat("%s %d", currentLanguage->cota_micsoratoare, i + 1));
        if (GuiTextBox((Rectangle){ 230, 300 + i * 150, 70, 40 }, upper_dev[i], 16, activeTextBox == (3 + i * 3))) activeTextBox = 3 + i * 3;
        if (GuiTextBox((Rectangle){ 130, 340 + i * 150, 100, 40 }, nominal[i], 16, activeTextBox == (4 + i * 3))) activeTextBox = 4 + i * 3;
        if (GuiTextBox((Rectangle){ 230, 380 + i * 150, 70, 40 }, lower_dev[i], 16, activeTextBox == (5 + i * 3))) activeTextBox = 5 + i * 3;

        reducing_dims[i] = TextToFloat(nominal[i]);
        dev_reducing_dims[i * 2] = TextToFloat(upper_dev[i]);
        dev_reducing_dims[i * 2 + 1] = TextToFloat(lower_dev[i]);
    }

    float c1 = TextToFloat(cota_maritoare);
    float dev_c1_max = TextToFloat(abatere_superioara);
    float dev_c1_min = TextToFloat(abatere_inferioara);
    
    if (GuiButton((Rectangle){ 600, 300, 120, 50 }, currentLanguage->calculate)) {
        calculateLength(c1, dev_c1_max, dev_c1_min, dimension_count, reducing_dims, dev_reducing_dims, &L_nominal, &max_deviation, &min_deviation);
    }

    DrawRepresentation(c1, reducing_dims, dimension_count, L_nominal, 450, 30);

    GuiLabel((Rectangle){ 550, 10, 300, 40 }, currentLanguage->cota_calculata);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiLabel((Rectangle){ 650, 100, 300, 40 }, TextFormat("%.3f ", max_deviation));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiLabel((Rectangle){ 575, 100, 300, 100 }, TextFormat("%.2f mm", L_nominal));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiLabel((Rectangle){ 650, 160, 300, 40 }, TextFormat("%.3f ", min_deviation));

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
}

int main(void) {
    InitWindow(900, 1000, "Aflarea unei cote dintr-un lant de dimensiuni");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            activeTextBox = -1; // Reset active text box when clicking outside
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGUI();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
