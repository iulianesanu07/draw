#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.hpp"

class Camera
{
public: 
  glm::vec3 Position;
  glm::vec3 Orientation = glm::vec3(0.0f, -1.0f, 0.0f);
  glm::vec3 Up = glm::vec3(1.0f, 0.0f, 0.0f);
  double lastX, lastY;

  bool firstClick = true;

  int width;
  int height;
  
  float speed = 0.1f;
  float sensitivity = 20.0f;

  Camera(int width, int height, glm::vec3 position);

  void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
  void Inputs(GLFWwindow* window);

};


#endif // !CAMERA_CLASS_H
