#include "wonderlands.h"

// Global variables
static Camera camera;
static SceneManager sceneManager;
static Renderer renderer;
static double lastTime = 0;
static bool keys[256];
static bool specialKeys[256];
static int mouseX = 0, mouseY = 0;
static bool firstMouse = true;
static bool paused = false;
static float timeOfDay = 0.0f;
static float timeFactor = 1.0f;
static WeatherType currentWeather = WEATHER_CLEAR;

// Function prototypes
void init(int argc, char **argv);
void initGL();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void specialKeyboardUp(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void passiveMotion(int x, int y);
void update();
void cleanup();

int main(int argc, char **argv) {
    // Initialize GLUT and create window
    init(argc, argv);
    
    // Initialize OpenGL
    initGL();
    
    // Initialize scene
    sceneManager_init(&sceneManager);
    renderer_init(&renderer);
    camera_init(&camera, (vec3){0, 15, 0}, (vec3){0, 0, -1}, (vec3){0, 1, 0});
    
    // Enter main loop
    glutMainLoop();
    
    // Clean up
    cleanup();
    
    return 0;
}

void init(int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    
    // Set OpenGL version
    #ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    #else
    glutInitContextVersion(4, 1);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    #endif
    
    // Create window
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutIdleFunc(update);
    
    // Initialize GLEW
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW initialization error: %s\n", glewGetErrorString(err));
        exit(1);
    }
    #endif
    
    // Hide cursor
    glutSetCursor(GLUT_CURSOR_NONE);
    
    // Initialize timer
    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

void initGL() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable antialiasing
    glEnable(GL_MULTISAMPLE);
    
    // Clear color
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
}

void display() {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // Render scene
    renderer_render(&renderer, &sceneManager, &camera, timeOfDay, currentWeather);
    
    // Swap buffers
    glutSwapBuffers();
}

void reshape(int width, int height) {
    // Update viewport
    glViewport(0, 0, width, height);
    
    // Update camera aspect ratio
    camera_updateProjection(&camera, width, height);
}

void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;
    
    // Handle special keys
    switch (key) {
        case 27: // ESC
            cleanup();
            exit(0);
            break;
        case 'p':
            paused = !paused;
            break;
        case '[':
            timeFactor *= 0.5f;
            if (timeFactor < 0.1f) timeFactor = 0.1f;
            break;
        case ']':
            timeFactor *= 2.0f;
            if (timeFactor > 10.0f) timeFactor = 10.0f;
            break;
        case 'c':
            currentWeather = WEATHER_CLEAR;
            break;
        case 'r':
            currentWeather = WEATHER_RAIN;
            break;
        case 'f':
            currentWeather = WEATHER_FOG;
            break;
        case 'l':
            renderer.showWireframe = !renderer.showWireframe;
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void specialKeyboard(int key, int x, int y) {
    specialKeys[key] = true;
}

void specialKeyboardUp(int key, int x, int y) {
    specialKeys[key] = false;
}

void mouse(int button, int state, int x, int y) {
    // Handle mouse button press/release
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Interact with scene
            renderer_pick(&renderer, &sceneManager, &camera, x, y);
        }
    }
}

void motion(int x, int y) {
    // Handle mouse movement with button pressed
    if (firstMouse) {
        mouseX = x;
        mouseY = y;
        firstMouse = false;
    }
    
    int deltaX = x - mouseX;
    int deltaY = mouseY - y; // Inverted for camera
    mouseX = x;
    mouseY = y;
    
    // Update camera look
    camera_rotate(&camera, deltaX * CAMERA_MOUSE_SENSITIVITY, deltaY * CAMERA_MOUSE_SENSITIVITY);
}

void passiveMotion(int x, int y) {
    // Handle mouse movement without button pressed
    if (firstMouse) {
        mouseX = x;
        mouseY = y;
        firstMouse = false;
    }
    
    int deltaX = x - mouseX;
    int deltaY = mouseY - y; // Inverted for camera
    mouseX = x;
    mouseY = y;
    
    // Update camera look
    camera_rotate(&camera, deltaX * CAMERA_MOUSE_SENSITIVITY, deltaY * CAMERA_MOUSE_SENSITIVITY);
    
    // Wrap cursor if it reaches screen edge
    if (x <= 0 || x >= WINDOW_WIDTH - 1 || y <= 0 || y >= WINDOW_HEIGHT - 1) {
        mouseX = WINDOW_WIDTH / 2;
        mouseY = WINDOW_HEIGHT / 2;
        glutWarpPointer(mouseX, mouseY);
    }
}

void update() {
    // Calculate delta time
    double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    float deltaTime = (float)(currentTime - lastTime);
    lastTime = currentTime;
    
    // Handle keyboard input for camera movement
    vec3 moveDir = {0, 0, 0};
    if (keys['w']) moveDir[2] -= 1.0f;
    if (keys['s']) moveDir[2] += 1.0f;
    if (keys['a']) moveDir[0] -= 1.0f;
    if (keys['d']) moveDir[0] += 1.0f;
    if (keys['q']) moveDir[1] -= 1.0f;
    if (keys['e']) moveDir[1] += 1.0f;
    
    // Update camera position
    camera_move(&camera, moveDir, deltaTime * CAMERA_MOVE_SPEED);
    
    // Update time of day
    if (!paused) {
        timeOfDay += deltaTime * timeFactor / DAY_LENGTH;
        if (timeOfDay >= 1.0f) timeOfDay -= 1.0f;
    }
    
    // Update scene
    sceneManager_update(&sceneManager, deltaTime, timeOfDay, currentWeather);
    
    // Redisplay
    glutPostRedisplay();
}

void cleanup() {
    // Cleanup resources
    renderer_cleanup(&renderer);
    sceneManager_cleanup(&sceneManager);
} 