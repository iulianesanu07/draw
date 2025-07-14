#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stb/stb_image.h>


#include "EBO.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "shaderClass.hpp"
#include "camera.hpp"
#include "texture.hpp"

#define GL_SILENCE_DEPRECATION

/*
// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
     -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Lower left corner
     -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Upper left corner
      0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Upper right corner
      0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] = 
{
    0, 2, 1, // Upper triangle
    0, 3, 2
};
*/


// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main() {
  
  int height = 1000;
  int width = 1000;
  int windowScaler = 1;


  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Spécifique macOS
#endif

  GLFWwindow *window =
      glfwCreateWindow(height, width, "Test OpenGL + GLAD", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGL(glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  glViewport(0, 0, height * windowScaler, width * windowScaler);

  Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));
  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  // Texture
  Texture pop_cat("res/images/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  pop_cat.texUnit(shaderProgram, "tex0", 0);

  glEnable(GL_DEPTH_TEST);

  
  // Camera
  Camera camera(width, height, glm::vec3(0.0f, 5.0f, 0.0f));

  while (!glfwWindowShouldClose(window)) {
   
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.Activate();

    camera.Inputs(window);
    camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

    pop_cat.Bind();
    VAO1.Bind();

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  pop_cat.Delete();
  shaderProgram.Delete();

  glfwDestroyWindow(window);

  glfwTerminate();
  return 0;
}
