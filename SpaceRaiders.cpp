#include"mesh.h" 
#include"extraFunctions.h" 
#include"meshDrawTools.h" 
#include"relativeResPath.h" 

const unsigned int width = 900;
const unsigned int height = 900; 

const glm::mat4 iden4 = glm::mat4(1.0f); 
const int noFallingObjects = 50; 

class Entity {
public:
    glm::vec2 position = glm::vec2(0.0f); 
    glm::vec2 velocity = glm::vec2(0.0f); 
    glm::vec3 color; 
    float radius; 
    int resolution; 
    float deltaG = 0.0f;
    Mesh mesh; 

    // Constructor creates a circle mesh using the passed parameter values
    Entity(float r, int res, glm::vec3 col, glm::vec2 pos = glm::vec2(0.0f)) 
        : radius(r), resolution(res), color(col), position(pos), 
          mesh(drt::createCircle(r, res, col)) {}
};

void PositionChange(GLFWwindow* window, Entity& player, float addRate, float dt){
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        player.position.y += addRate * dt; 
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        player.position.y -= addRate * dt; 
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        player.position.x += addRate * dt; 
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        player.position.x -= addRate * dt; 
    } 
}

void Reset(std::vector<Entity>& thisEntity, Entity& player){
    for (Entity& thisone : thisEntity){
        thisone.position.y = 1.0f; 
        thisone.velocity = glm::vec2(0.0f); 
    }
    player.position = glm::vec2(0.0f); 
}

int main(){ 
    rd::randseed(); 

    GLFWwindow* window = et::initialiseGLFW(width, height, "Window");   

    Shader GeneralShader(rrp::getVertexPath() + "modelmat.vert.txt", rrp::getFragmentPath() + "default.frag.txt"); 

    // 1. Create falling entities (radius: 0.09f, resolution: 18)
    std::vector<Entity> fallingObjects; 
    fallingObjects.reserve(noFallingObjects); 
    
    for(int i = 0; i < noFallingObjects; ++i){
        fallingObjects.emplace_back(0.09f, 18, glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)); 
        fallingObjects.back().position.x = rd::randsignf(); 
        fallingObjects.back().deltaG = -1.0f * rd::randf(); 
    }

    // 2. Create player entity (radius: 0.04f, resolution: 18)
    Entity playerBall(0.04f, 18, glm::vec3(0.12f, 0.56f, 1.00f), glm::vec2(0.0f, 0.0f)); 

    double prevTime = glfwGetTime(); 
    double previousTime = prevTime;   
    bool GameOver = false; 
    int frameRate = 0; 

    float dt = 1.0f / 60.0f; 
    float angle = 0.0f; 

    int Score = 0; 

    float BoundaryMinusPlayerRad = 1.0f - playerBall.radius;  

    glm::mat4 translationMat; 

    while (!glfwWindowShouldClose(window)){ 

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double crntTime = glfwGetTime(); 
        frameRate += 1; 
        Score += 1; 

        if (crntTime - prevTime >= 1.0f){
            std::cout << "Frame Rate: " << frameRate << std::endl;  
            std::cout << "Score: " << Score << std::endl; 
            prevTime = crntTime; 
            frameRate = 0;   
        }

        if (!GameOver && crntTime - previousTime >= dt){ 
            PositionChange(window, playerBall, 0.9f, dt); 
            previousTime = crntTime; 
            angle += 0.1f; 

            for(Entity& obj : fallingObjects){
                obj.velocity.y += obj.deltaG * dt; 
                obj.position += obj.velocity * dt; 
                if(obj.position.y < -1.0f){ 
                    obj.deltaG = -1.0f * rd::randf();  
                    obj.position.x = rd::randsignf(); 
                    obj.position.y = 1.0f; 
                    obj.velocity.y = 0.0f;
                }
                if(glm::distance(obj.position, playerBall.position) < obj.radius + playerBall.radius - 0.01){
                    std::cout << "Game Over" << std::endl; 
                    GameOver = true; 
                }
            }
        } 

        if(GameOver && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
            Reset(fallingObjects, playerBall); 
            Score = 0; 
            GameOver = false; 
        }

        GeneralShader.Activate(); 

        // Draw falling objects
        for(Entity& obj : fallingObjects){
            translationMat = glm::translate(iden4, glm::vec3(obj.position, 0.0f));
            
            glUniformMatrix4fv(glGetUniformLocation(GeneralShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(translationMat));
            obj.mesh.Draw(GeneralShader, GL_TRIANGLES); 
        } 

        // Draw player
        if(playerBall.position.x > BoundaryMinusPlayerRad){
            playerBall.position.x = BoundaryMinusPlayerRad; 
        }
        if(playerBall.position.x < -BoundaryMinusPlayerRad){
            playerBall.position.x = -BoundaryMinusPlayerRad; 
        }
        if(playerBall.position.y > BoundaryMinusPlayerRad){
            playerBall.position.y = BoundaryMinusPlayerRad; 
        }
        if(playerBall.position.y < -BoundaryMinusPlayerRad){
            playerBall.position.y = -BoundaryMinusPlayerRad; 
        }
       
        translationMat = glm::translate(iden4, glm::vec3(playerBall.position, 0.0f)); 
        glUniformMatrix4fv(glGetUniformLocation(GeneralShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(translationMat));
        playerBall.mesh.Draw(GeneralShader, GL_TRIANGLES); 

        glfwSwapBuffers(window);
        et::processInput(window); 
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0; 
}