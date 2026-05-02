#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "engine/world.h"
#include "graphics/renderer.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

class Game {
public:
    Game() : window(nullptr), world(nullptr) {}
    
    bool init() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Voxel Game", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create window" << std::endl;
            return false;
        }
        
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        
        // Initialize renderer
        Renderer::getInstance().init(WINDOW_WIDTH, WINDOW_HEIGHT);
        
        // Create world
        world = new World();
        
        // Setup projection matrix
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)WINDOW_WIDTH / WINDOW_HEIGHT,
            0.1f,
            1000.0f
        );
        Renderer::getInstance().setProjection(projection);
        
        return true;
    }
    
    void run() {
        glm::vec3 playerPos(0.0f, 100.0f, 0.0f);
        glm::vec3 playerVel(0.0f, 0.0f, 0.0f);
        float yaw = 0.0f, pitch = 0.0f;
        
        while (!glfwWindowShouldClose(window)) {
            Renderer::getInstance().beginFrame();
            
            // Update camera view
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            
            glm::mat4 view = glm::lookAt(
                playerPos,
                playerPos + front,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
            Renderer::getInstance().setView(view);
            
            // Handle input
            handleInput(playerPos, playerVel, yaw, pitch);
            
            // Update world
            world->update(playerPos);
            
            // Render
            world->render();
            
            Renderer::getInstance().endFrame();
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    
    void cleanup() {
        delete world;
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
private:
    GLFWwindow* window;
    World* world;
    
    void handleInput(glm::vec3& pos, glm::vec3& vel, float& yaw, float& pitch) {
        const float MOVE_SPEED = 0.5f;
        const float MOUSE_SENSITIVITY = 0.1f;
        
        // Keyboard input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos.z -= MOVE_SPEED;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos.z += MOVE_SPEED;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos.x -= MOVE_SPEED;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos.x += MOVE_SPEED;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) pos.y += MOVE_SPEED;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
        
        // Mouse input
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        static double lastMouseX = mouseX, lastMouseY = mouseY;
        
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            yaw += (float)(mouseX - lastMouseX) * MOUSE_SENSITIVITY;
            pitch -= (float)(mouseY - lastMouseY) * MOUSE_SENSITIVITY;
            
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }
        
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
};

int main() {
    Game game;
    
    if (!game.init()) {
        return -1;
    }
    
    game.run();
    game.cleanup();
    
    return 0;
}
