#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "EBO.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "shaderClass.hpp"

#define GL_SILENCE_DEPRECATION

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Clic détecté aux coordonnées écran : (" << xpos << ", "
              << ypos << ")\n";
  }
}

std::vector<float> generateCircleVertices(float centerX, float centerY,
                                          float radius, int segments = 20) {
  std::vector<float> vertices;

  // Triangle fan: centre + segments
  vertices.push_back(centerX);
  vertices.push_back(centerY);

  for (int i = 0; i <= segments; ++i) {
    float angle = (2.0f * M_PI * i) / segments;
    float x = centerX + radius * cos(angle);
    float y = centerY + radius * sin(angle);
    vertices.push_back(x);
    vertices.push_back(y);
  }

  return vertices;
}

int main() {
  
  int height = 1000;
  int weigth = 1000;
  int windowScaler = 2;


  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  // On force le Core Profile OpenGL 4.1 (macOS support max)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Spécifique macOS
#endif

  GLfloat vertices[] = {
      -0.5f,     -0.5f * float(sqrt(3)) / 3,    // Lower left corner
      0.5f,      -0.5f * float(sqrt(3)) / 3,    // Lower right corner
      0.0f,      0.5f * float(sqrt(3)) * 2 / 3, // Upper corner
      -0.5f / 2, 0.5f * float(sqrt(3)) / 6,     // Inner left
      0.5f / 2,  0.5f * float(sqrt(3)) / 6,     // Inner right
      0.0f,      -0.5f * float(sqrt(3)) / 3     // Inner down
  };

  GLuint indices[] = {0, 3, 5, 3, 2, 4, 5, 4, 1};

  std::vector<glm::vec2> clickPositions;

  GLFWwindow *window =
      glfwCreateWindow(height, weigth, "Test OpenGL + GLAD", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  if (!gladLoadGL(glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));
  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkAttrib(VBO1, 0, 2, GL_FLOAT, 2 * sizeof(float), (void *)0);

  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  shaderProgram.Activate();
  VAO1.Bind();

  while (!glfwWindowShouldClose(window)) {
    /*
    glEnable(GL_SCISSOR_TEST); // active la découpe par zone

    // === Zone principale (gauche, gris)
    glViewport(0, 0, 600, 450);
    glScissor(0, 0, 600, 450); // Limite le clear à cette zone
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.Activate();
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    // === Panneau latéral (droite, moins gris)
    glViewport(600, 0, 200, 450);
    glScissor(600, 0, 200, 450);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST); // désactivation optionnelle
    */

    glViewport(0, 0, height * windowScaler, weigth * windowScaler);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "colorUni"), 1.0f, 1.0f,
                0.0f);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);

      int width, height;
      glfwGetWindowSize(window, &width, &height);

      float oglX = (xpos / width) * 2.0f - 1.0f;
      float oglY = -((ypos / height) * 2.0f - 1.0f);

      std::cout << "Clic maintenu - Coordonnées OpenGL : (" << oglX << ", "
                << oglY << ")\n";

      clickPositions.emplace_back(oglX, oglY);
    }

    for (const auto &pos : clickPositions) {
      auto circleVerts =
          generateCircleVertices(pos.x, pos.y, 0.02f); // petit rayon

      // Crée VBO/VAO à la volée (pas optimal mais simple)
      VAO circleVAO;
      circleVAO.Bind();
      VBO circleVBO(circleVerts.data(), circleVerts.size() * sizeof(float));
      circleVAO.LinkAttrib(circleVBO, 0, 2, GL_FLOAT, 2 * sizeof(float),
                           (void *)0);

      glDrawArrays(GL_TRIANGLE_FAN, 0, circleVerts.size() / 2);

      circleVBO.Delete();
      circleVAO.Delete();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  shaderProgram.Delete();

  glfwDestroyWindow(window);

  glfwTerminate();
  return 0;
}
