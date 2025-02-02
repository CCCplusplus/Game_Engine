/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0.
*/

#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

// -----------------------------------------------------------------------------
// Definición de niveles de verbosidad y módulos para DebugLog
// -----------------------------------------------------------------------------
typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

typedef enum {
    MODULE_RENDER,
    MODULE_INPUT,
    MODULE_AUDIO,
    MODULE_PHYSICS,
    MODULE_FILES,
    MODULE_NETWORK
} Module;

LogLevel currentLogLevel = LOG_LEVEL_DEBUG;

void SetLogLevel(LogLevel level) {
    currentLogLevel = level;
}

void DebugLog(LogLevel level, Module module, const char* message) {
    if (level <= currentLogLevel) {
        const char* levelStr;
        switch (level) {
        case LOG_LEVEL_ERROR: levelStr = "ERROR"; break;
        case LOG_LEVEL_WARNING: levelStr = "WARNING"; break;
        case LOG_LEVEL_INFO: levelStr = "INFO"; break;
        case LOG_LEVEL_DEBUG: levelStr = "DEBUG"; break;
        default: levelStr = "UNKNOWN"; break;
        }

        const char* moduleStr;
        switch (module) {
        case MODULE_RENDER: moduleStr = "RENDER"; break;
        case MODULE_INPUT: moduleStr = "INPUT"; break;
        case MODULE_AUDIO: moduleStr = "AUDIO"; break;
        case MODULE_NETWORK: moduleStr = "NETWORK"; break;
        case MODULE_PHYSICS: moduleStr = "PHYSICS"; break;
        case MODULE_FILES: moduleStr = "FILES"; break;
        default: moduleStr = "UNKNOWN"; break;
        }

        // Imprime en la consola
        printf("[%s] [%s] %s\n", levelStr, moduleStr, message);

        // Se añade al archivo debug.log
        FILE* logFile = fopen("debug.log", "a");
        if (logFile != NULL) {
            fprintf(logFile, "[%s] [%s] %s\n", levelStr, moduleStr, message);
            fclose(logFile);
        }
    }
}

// -----------------------------------------------------------------------------
// Estructura y función para cargar configuración desde un archivo INI
// -----------------------------------------------------------------------------
typedef struct {
    int resX;
    int resY;
    bool fullscreen;
    bool vsync;
} VideoConfig;

void LoadConfig(const char* filename, VideoConfig* config) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se puede abrir el archivo %s\n", filename);
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "resx=%d", &config->resX) == 1) continue;
        if (sscanf(line, "resy=%d", &config->resY) == 1) continue;
        if (sscanf(line, "fullscreen=%d", (int*)&config->fullscreen) == 1) continue;
        if (sscanf(line, "vsync=%d", (int*)&config->vsync) == 1) continue;
    }

    fclose(file);
}

// -----------------------------------------------------------------------------
// Función para dibujar un cubo con textura
// -----------------------------------------------------------------------------
void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    rlSetTexture(texture.id);

    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);
    // Front Face
    rlNormal3f(0.0f, 0.0f, 1.0f);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
    // Back Face
    rlNormal3f(0.0f, 0.0f, -1.0f);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
    // Top Face
    rlNormal3f(0.0f, 1.0f, 0.0f);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
    // Bottom Face
    rlNormal3f(0.0f, -1.0f, 0.0f);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
    // Right Face
    rlNormal3f(1.0f, 0.0f, 0.0f);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
    // Left Face
    rlNormal3f(-1.0f, 0.0f, 0.0f);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
    rlEnd();

    rlSetTexture(0);
}

// -----------------------------------------------------------------------------
// Función principal
// -----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    VideoConfig config = { 1024, 800, false, false };  // Valores predeterminados
    LoadConfig("config.ini", &config);

    

    
    FILE* configFile = fopen("config.ini", "r");
    if (configFile == NULL) {
        DebugLog(LOG_LEVEL_ERROR, MODULE_FILES, "config.ini not found");
    }
    else {
        fclose(configFile);
    }

    if (config.vsync) SetConfigFlags(FLAG_VSYNC_HINT);
    if (config.fullscreen) SetConfigFlags(FLAG_FULLSCREEN_MODE);

    // Crear la ventana y el contexto OpenGL
    InitWindow(config.resX, config.resY, "Hello Raylib");
    if (config.fullscreen) ToggleFullscreen();

    // ***********************
    // Cargar el modelo y la textura principal usando rutas relativas que funcionen
    // (sin haber cambiado el directorio de trabajo aún)
    // ***********************
    Model model = LoadModel("resources/cottage_obj.obj");      // Carga el modelo
    Texture2D texture = LoadTexture("resources/cottage_diffuse.png"); // Carga la textura
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    Vector3 position = { 0.0f, 0.0f, 0.0f };

    // Verificar que el modelo se haya cargado correctamente
    if (model.meshCount > 0) {
        BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);
    }
    else {
        DebugLog(LOG_LEVEL_ERROR, MODULE_FILES, "Error: No se pudieron cargar los meshes del modelo");
    }

    // ***********************
    // Ahora se cambia el directorio de trabajo para cargar otros recursos (p. ej. la textura "wood.png")
    // Nota: Si cambias el directorio, asegúrate de ajustar las rutas en consecuencia.
    // En este ejemplo, se asume que "wood.png" se encuentra en la carpeta "resources"
    // ***********************
    SearchAndSetResourceDir("resources");

    // Descargar y cargar la marca de agua (watermark) sin usar libcurl
    if (!FileExists("watermark.png"))
    {
    #if defined(_WIN32)
        system("powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://avatars.githubusercontent.com/u/139177589?s=96&v=4', 'watermark.png')\"");
    #else
        system("wget -q \"https://avatars.githubusercontent.com/u/139177589?s=96&v=4\" -O watermark.png");
    #endif
    }

    Image watermarkImage = LoadImage("watermark.png");    // Carga la imagen descargada
    Texture2D watermarkTexture = LoadTextureFromImage(watermarkImage);
    UnloadImage(watermarkImage);

    // Cargar la textura para el cubo
    Texture cubetext = LoadTexture("wood.png");

    // Configurar la cámara 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 4, 0, 2 };
    camera.target = (Vector3){ 0, 0, 0 };
    camera.up = (Vector3){ 0, 1, 0 };
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    float cubeX = 0;
    float cubeY = 0;
    float cubeZ = 0;
    float gravity = 0.5f;

    SetLogLevel(LOG_LEVEL_DEBUG);
    DebugLog(LOG_LEVEL_INFO, MODULE_RENDER, "Render module initialized.");
    DebugLog(LOG_LEVEL_WARNING, MODULE_INPUT, "Input module warning.");
    DebugLog(LOG_LEVEL_ERROR, MODULE_AUDIO, "Audio module error.");
    DebugLog(LOG_LEVEL_DEBUG, MODULE_RENDER, "Render module debug message.");

    // Calcula la posición para que la imagen quede en la esquina inferior derecha
   


    // Bucle principal
    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();
            if (droppedFiles.count == 1)
            {
                if (IsFileExtension(droppedFiles.paths[0], ".obj") ||
                    IsFileExtension(droppedFiles.paths[0], ".gltf") ||
                    IsFileExtension(droppedFiles.paths[0], ".glb") ||
                    IsFileExtension(droppedFiles.paths[0], ".vox") ||
                    IsFileExtension(droppedFiles.paths[0], ".iqm") ||
                    IsFileExtension(droppedFiles.paths[0], ".m3d"))
                {
                    UnloadModel(model);
                    model = LoadModel(droppedFiles.paths[0]);
                    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                }
                else if (IsFileExtension(droppedFiles.paths[0], ".png"))
                {
                    UnloadTexture(texture);
                    texture = LoadTexture(droppedFiles.paths[0]);
                    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                }
            }
            UnloadDroppedFiles(droppedFiles);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        DrawModel(model, position, 1.0f, WHITE);
        DrawCubeTexture(cubetext, (Vector3) { cubeX, cubeY, cubeZ }, 5, 5, 5, RAYWHITE);
        if (cubeY != 0.0f) cubeY -= gravity;
        DrawGrid(20, 10);
        if (IsKeyDown(KEY_W)) cubeX += 0.5f;
        if (IsKeyDown(KEY_S)) cubeX -= 0.5f;
        if (IsKeyDown(KEY_A)) cubeZ -= 0.5f;
        if (IsKeyDown(KEY_D)) cubeZ += 0.5f;
        if ((cubeY != 10) && IsKeyPressed(KEY_SPACE)) cubeY += 15;
        EndMode3D();

        // Dibujar la marca de agua en la esquina inferior derecha
        if (watermarkTexture.id != 0)
        {
            float scale = 0.5f;  // Escala: 0.5 equivale al 50% del tamaño original
            int margin = 10;
            int screenWidth = GetScreenWidth();
            int screenHeight = GetScreenHeight();
            Vector2 watermarkPos = {
                    screenWidth - (watermarkTexture.width * scale) - margin,
                    screenHeight - (watermarkTexture.height * scale) - margin
            };
            DrawTextureEx(watermarkTexture, watermarkPos, 0.0f, scale, WHITE);
        }

        EndDrawing();
    }

    // Liberar recursos
    UnloadTexture(texture);
    UnloadTexture(cubetext);
    UnloadModel(model);
    if (watermarkTexture.id != 0) UnloadTexture(watermarkTexture);

    CloseWindow();
    return 0;
}
