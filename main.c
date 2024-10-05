#include <raylib.h>

#define MAX_COLORS_COUNT 23
#define CANVAS_COUNT 4

int main(void)
{

    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");


    Color colors[MAX_COLORS_COUNT] = {
        RAYWHITE, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,
        SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
        LIGHTGRAY, GRAY, DARKGRAY, BLACK };

    Rectangle colorsRecs[MAX_COLORS_COUNT] = { 0 };

    for (int i = 0; i < MAX_COLORS_COUNT; i++)
    {
        colorsRecs[i].x = 10 + 30.0f*i + 2*i;
        colorsRecs[i].y = 10;
        colorsRecs[i].width = 30;
        colorsRecs[i].height = 30;
    }

    Rectangle canvasRecs[CANVAS_COUNT] = { 0 };

    for (int i = 0; i < CANVAS_COUNT; i++)
    {
        canvasRecs[i].x = 10 + 100.0f * i;
        canvasRecs[i].y = 60;               
        canvasRecs[i].width = 80;           
        canvasRecs[i].height = 30;          
    }
        
    int brushSize = 20.0f;
    int currentColor = 0;
    int colorMouseHover = 0;
    int canvasMouseHover = 0;
    int prevColor = currentColor;
    int selectedTool = 0; //for later use
    int selectedCanvas = 0;
    bool mouseWasPressed = false;
    Vector2 ballPosition; 

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    BeginTextureMode(target);
    ClearBackground(colors[0]);
    EndTextureMode();

    RenderTexture2D target1[CANVAS_COUNT];
    for (int i = 0; i < CANVAS_COUNT; i++) {
        target1[i] = LoadRenderTexture(screenWidth, screenHeight);
        BeginTextureMode(target1[i]);
        ClearBackground(colors[0]); // Clear each screen to the default color
        EndTextureMode();
    }
    

    SetTargetFPS(600);

    while (!WindowShouldClose())
    {
        // Update
        ballPosition = GetMousePosition();


        // chnage brush size with mouse wheel
        brushSize += GetMouseWheelMove()*5;
        if (brushSize < 2) brushSize = 2;
        if (brushSize > 250) brushSize = 250;

        // top bar
        for (int i = 0; i < MAX_COLORS_COUNT; i++)
        {
            if (CheckCollisionPointRec(ballPosition, colorsRecs[i]))
            {
                colorMouseHover = i;
                break;
            }
            else colorMouseHover = -1;
        }

        for (int i = 0; i < CANVAS_COUNT; i++)
        {
            if (CheckCollisionPointRec(ballPosition, canvasRecs[i]))
            {
                canvasMouseHover = i;
                break;
            }
            else canvasMouseHover = -1;
        }
        // select color with mouse
        if ((colorMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            currentColor = colorMouseHover;
            prevColor = currentColor;
        }
        if((canvasMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            selectedCanvas = canvasMouseHover;
        }


        if (IsKeyPressed(KEY_ONE))   selectedCanvas = 0;
        if (IsKeyPressed(KEY_TWO))   selectedCanvas = 1;
        if (IsKeyPressed(KEY_THREE)) selectedCanvas = 2;
        if (IsKeyPressed(KEY_FOUR))  selectedCanvas = 3;


        // draw with mousse
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            BeginTextureMode(target1[selectedCanvas]);
            DrawCircleV(ballPosition, brushSize, colors[currentColor]);
            EndTextureMode();
        }

        // remove with mouse

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            if (!mouseWasPressed)
            {
                prevColor = currentColor;
                currentColor = 0;
            }
            mouseWasPressed = true;
        }
        else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && mouseWasPressed)
        {
            currentColor = prevColor;
            mouseWasPressed = false;
        }

        // clear screen
        if(IsKeyDown(KEY_C)){
            BeginTextureMode(target);
            ClearBackground(colors[0]);
            EndTextureMode();
        }

        // chnage color
        if(IsKeyPressed(KEY_RIGHT)){
            currentColor++;
        }
        if(IsKeyPressed(KEY_LEFT)){
            currentColor--;
        }
        if (currentColor >= MAX_COLORS_COUNT) currentColor = MAX_COLORS_COUNT - 1;
        else if (currentColor < 0) currentColor = 0;
        //----------------------------------------------------------------------------------



        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureRec(target1[selectedCanvas].texture, (Rectangle){0, 0, target1[selectedCanvas].texture.width,
             -target1[selectedCanvas].texture.height}, (Vector2){0, 0}, WHITE);

            // mouse circle and the whole drawing
            if(selectedTool == 0){
                DrawCircleV(ballPosition, brushSize, colors[currentColor]);
            }

            // top bar for colors
            DrawRectangle(0, 0, GetScreenWidth(), 50, RAYWHITE);
            DrawLine(0, 50, GetScreenWidth(), 50, LIGHTGRAY);

            // top bar for canvas
            DrawRectangle(0, 50, GetScreenWidth(), 50, RAYWHITE);
            DrawLine(50, 50, GetScreenWidth(), 50, LIGHTGRAY);


            // CANVAS BUTTONS RENDER
            DrawLine(0,100, GetScreenWidth(),100,LIGHTGRAY);

            for (int i = 0; i < CANVAS_COUNT; i++) {
                DrawRectangleRec(canvasRecs[i], (i == selectedCanvas) ? DARKGRAY : LIGHTGRAY);
                DrawRectangleLinesEx(canvasRecs[i], 1, BLACK);
                DrawText(TextFormat("SCR: %d", i+1), canvasRecs[i].x + 5, canvasRecs[i].y + 5, 20, BLACK);
            }

            if (canvasMouseHover >= 0) {
                DrawRectangleRec(canvasRecs[canvasMouseHover], Fade(WHITE, 0.3f));
            }

            DrawRectangleLinesEx((Rectangle){ canvasRecs[selectedCanvas].x - 2, canvasRecs[selectedCanvas].y - 2,
                canvasRecs[selectedCanvas].width + 4, canvasRecs[selectedCanvas].height + 4 }, 2, BLACK);

            DrawRectangleLines(10, 10, 30, 30, LIGHTGRAY);


            /*
            #######################
            ####################### 
            #######################
            */

            // color selection
            for (int i = 0; i < MAX_COLORS_COUNT; i++) DrawRectangleRec(colorsRecs[i], colors[i]);
            DrawRectangleLines(10, 10, 30, 30, LIGHTGRAY);

            if (colorMouseHover >= 0) DrawRectangleRec(colorsRecs[colorMouseHover], Fade(WHITE, 0.6f));

            DrawRectangleLinesEx((Rectangle){ colorsRecs[currentColor].x - 2, colorsRecs[currentColor].y - 2,
                colorsRecs[currentColor].width + 4, colorsRecs[currentColor].height + 4 }, 2, BLACK);

            DrawRectangleLines(10, 10, 30, 30, LIGHTGRAY);

            /*
            ########################
            ########################
            ########################
            */
        EndDrawing();   
        //----------------------------------------------------------------------------------
    }
    UnloadRenderTexture(target);  

    CloseWindow();
    return 0;
}