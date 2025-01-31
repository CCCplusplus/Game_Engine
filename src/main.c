/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

// Definici�n de niveles de verbosidad
typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

// Definici�n de m�dulos
typedef enum {
    MODULE_RENDER,
    MODULE_INPUT,
    MODULE_AUDIO,
	MODULE_PHYSICS,
	MODULE_FILES,
    MODULE_NETWORK
} Module;

// Variables globales para los filtros
LogLevel currentLogLevel = LOG_LEVEL_DEBUG;

// Funci�n para establecer el nivel de verbosidad
void SetLogLevel(LogLevel level) {
    currentLogLevel = level;
}

// Funci�n de DebugLog con filtros
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

        printf("[%s] [%s] %s\n", levelStr, moduleStr, message);
    }
}

typedef struct {
    int resX;
    int resY;
    bool fullscreen;
    bool vsync;
} VideoConfig;

// Funci�n para leer la configuraci�n desde un archivo INI
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

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // Vertex data transformation can be defined with the commented lines,
    // but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
    //rlPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        //rlTranslatef(2.0f, 0.0f, 0.0f);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(2.0f, 2.0f, 2.0f);

    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);
    // Front Face
    rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Top Left Of The Texture and Quad
    // Back Face
    rlNormal3f(0.0f, 0.0f, -1.0f);     // Normal Pointing Away From Viewer
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Bottom Left Of The Texture and Quad
    // Top Face
    rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    // Bottom Face
    rlNormal3f(0.0f, -1.0f, 0.0f);     // Normal Pointing Down
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    // Right face
    rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    // Left Face
    rlNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlEnd();
    //rlPopMatrix();

    rlSetTexture(0);
}

int main(int argc, char** argv)
{

    VideoConfig config = { 1024, 800, false, false };  // Valores predeterminados

    LoadConfig("config.ini", &config);

    FILE* configFile = fopen("config.ini", "r");
    if (configFile == NULL) {
        DebugLog(LOG_LEVEL_ERROR, MODULE_FILES, "config.ini not found");
    } else {
        fclose(configFile);
    }

    // Configurar la ventana seg�n la configuraci�n le�da
    if (config.vsync) {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }
    if (config.fullscreen) {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }

    // Crear la ventana y el contexto OpenGL
    InitWindow(config.resX, config.resY, "Hello Raylib");
    if (config.fullscreen) {
        ToggleFullscreen();
    }
	// Tell the window to use vsync and work on high DPI displays
	//SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

 //   int resX = 1280;
 //   int resY = 800;
 //   bool wantsFullscreen = false;
 //   if (argc > 1)
 //   {
 //       for (int i = 0; i < argc; i++)
 //       { 
 //           //std::cout << "arg " << i << argv[i] << std::endl;
 //           fprintf(stderr, "arg %i : %s \n", i, argv[i]);
 //           if (strcmp(argv[i], "-resx") == 0) 
 //           {
 //               resX = atoi(argv[i + 1]);
 //           }
 //           if (strcmp(argv[i], "-resy") == 0)
 //           {
 //               resY = atoi(argv[i + 1]);
 //           }
 //           if (strcmp(argv[i], "-fullscreen") == 0)
 //           {
 //               //wantsFullscreen = true;
 //           }
 //       }
 //   }


     // Establecer filtros
    SetLogLevel(LOG_LEVEL_DEBUG);

    // Ejemplos de uso de DebugLog
    DebugLog(LOG_LEVEL_INFO, MODULE_RENDER, "Render module initialized.");
    DebugLog(LOG_LEVEL_WARNING, MODULE_INPUT, "Input module warning.");
    DebugLog(LOG_LEVEL_ERROR, MODULE_AUDIO, "Audio module error.");
    DebugLog(LOG_LEVEL_DEBUG, MODULE_RENDER, "Render module debug message.");

    printf("Iniitializing game's sub system.\n");

    Model model = LoadModel("resources/cottage_obj.obj");                 // Load model
    Texture2D texture = LoadTexture("resources/cottage_diffuse.png"); // Load model texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Set map diffuse texture

    Vector3 position = { 0.0f, 0.0f, 0.0f };

    BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);


	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");


	Texture cubetext = LoadTexture("wood.png");


	Camera3D camera = { 0 };
	camera.position = (Vector3){ 4, 0, 2 };
	camera.target = (Vector3){ 0, 0, 0 };
	camera.up = (Vector3){ 0,1,0 };
	camera.fovy = 45;
	camera.projection = CAMERA_PERSPECTIVE;

	float cubeX = 0;
	float cubeY = 0;
	float cubeZ = 0;

	float gravity = 0.5;

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		UpdateCamera(&camera, CAMERA_FREE);


        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();

            if (droppedFiles.count == 1) // Only support one file dropped
            {
                if (IsFileExtension(droppedFiles.paths[0], ".obj") ||
                    IsFileExtension(droppedFiles.paths[0], ".gltf") ||
                    IsFileExtension(droppedFiles.paths[0], ".glb") ||
                    IsFileExtension(droppedFiles.paths[0], ".vox") ||
                    IsFileExtension(droppedFiles.paths[0], ".iqm") ||
                    IsFileExtension(droppedFiles.paths[0], ".m3d"))       // Model file formats supported
                {
                    UnloadModel(model);                         // Unload previous model
                    model = LoadModel(droppedFiles.paths[0]);   // Load new model
                    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set current map diffuse texture

                    bounds = GetMeshBoundingBox(model.meshes[0]);

                    // TODO: Move camera position from target enough distance to visualize model properly
                }
                else if (IsFileExtension(droppedFiles.paths[0], ".png"))  // Texture file formats supported
                {
                    // Unload current model texture and load new one
                    UnloadTexture(texture);
                    texture = LoadTexture(droppedFiles.paths[0]);
                    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                }
            }

            UnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
        }


		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		//DrawText("Hello Raylib", 200,200,20,WHITE);

		// draw our texture to the screen
		//DrawTexture(wabbit, 400, 200, WHITE);

		

		BeginMode3D(camera);

        DrawModel(model, position, 1.0f, WHITE);

		//DrawCube((Vector3) { cubeX, cubeY, cubeZ }, 5,5,5, RED);
        DrawCubeTexture(cubetext, (Vector3) { cubeX, cubeY, cubeZ }, 5, 5, 5, RAYWHITE);

		if (cubeY != 0.0)
			cubeY -= gravity;
		
		DrawGrid(20, 10);

		if (IsKeyDown(KEY_W)) cubeX+= 0.5;
		if (IsKeyDown(KEY_S)) cubeX -= 0.5;
		if (IsKeyDown(KEY_A)) cubeZ -= 0.5;
		if (IsKeyDown(KEY_D)) cubeZ += 0.5;

		if(cubeY != 10)
		if (IsKeyPressed(KEY_SPACE)) cubeY += 15;

		EndMode3D();

		
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

    UnloadTexture(texture); 
    UnloadTexture(cubetext);
    UnloadModel(model);        


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
