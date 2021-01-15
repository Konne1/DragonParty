#include "playground.h"

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Include chrono for sleep command
#include <chrono>
#include <thread>

#include <common/shader.hpp>
#include <common/objloader.hpp>

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  bool vertexBuffer2 = initializeVertexbuffer2();
  if (!vertexBuffer2) return -1;

  bool vertexBuffer3 = initializeVertexbuffer3();
  if (!vertexBuffer3) return -1;

  bool vertexBuffer4 = initializeVertexbuffer4();
  if (!vertexBuffer4) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("vertexShader.vertexshader", "fragmentShader.fragmentshader");

  // Get a handle for our "MVP" and "MV" uniforms and update them for initalization 
  MatrixIDMV = glGetUniformLocation(programID, "MV");
  MatrixID = glGetUniformLocation(programID, "MVP");
  updateMVPTransformation;

  //LightMatrixIDMV = glGetUniformLocation(programID, "MV");
  //LightID = glGetUniformLocation(programID, "MVP");

  //initialize pose variables
  curr_x = 0;
  curr_y = 0;
  curr_angle = 0;
  curr_rotate = 5;
  curr_rotate2 = 5;
  curr_rotate3 = 5;
  curr_rotate4 = 5;

  up = true;

  curr_translate = 0;

  curr_lightAngle = 10;

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

	//start animation loop until escape key is pressed
	do{

    updateAnimationLoop();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}

void updateAnimationLoop()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

  //the camera movement
  processPlayerInput();

  // Update the MVP transformation with the new values
  updateMVPTransformation();

  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform and also the "MV" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
  glVertexAttribPointer(
    1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  //Change the color  of the shader in dependency of the passed time
  float timeValue = glfwGetTime();
  float redValue = (sin(timeValue) / 2.0f) + 0.5f;
  float greenValue = (cos(timeValue) / 2.0f) + 0.5f;
  float blueValue = (tan(timeValue) / 2.0f) + 0.5f;
  float lastValue = (sin(timeValue) / 2.0f) + 0.5f;
  int vertexColorLocation = glGetUniformLocation(programID, "ourColor");
  glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, lastValue);

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

  //glUseProgram(lightShader);

  //updateLightTransformation();

  rotate();

  timeValue = glfwGetTime();
  redValue = (tan(timeValue) / 2.0f) + 0.5f;
  greenValue = (sin(timeValue) / 2.0f) + 0.5f;
  blueValue = (cos(timeValue) / 2.0f) + 0.5f;
  lastValue = (sin(timeValue) / 2.0f) + 0.5f;
  //vertexColorLocation = glGetUniformLocation(programID, "ourColor");
  glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, lastValue);

  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform and also the "MV" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2[0]);
  glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2[1]);
  glVertexAttribPointer(
      1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size2); // 3 indices starting at 0 -> 1 triangle

  rotate2();

  timeValue = glfwGetTime();
  redValue = (cos(timeValue) / 2.0f) + 0.5f;
  greenValue = (tan(timeValue) / 2.0f) + 0.5f;
  blueValue = (sin(timeValue) / 2.0f) + 0.5f;
  lastValue = (sin(timeValue) / 2.0f) + 0.5f;
  //vertexColorLocation = glGetUniformLocation(programID, "ourColor");
  glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, lastValue);

  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform and also the "MV" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3[0]);
  glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3[1]);
  glVertexAttribPointer(
      1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size3); // 3 indices starting at 0 -> 1 triangle

  rotate3();

  timeValue = glfwGetTime();
  redValue = (tan(timeValue) / 2.0f) + 0.5f;
  greenValue = (sin(timeValue) / 2.0f) + 0.5f;
  blueValue = (tan(timeValue) / 2.0f) + 0.5f;
  lastValue = (sin(timeValue) / 2.0f) + 0.5f;
  //vertexColorLocation = glGetUniformLocation(programID, "ourColor");
  glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, lastValue);

  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform and also the "MV" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4[0]);
  glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4[1]);
  glVertexAttribPointer(
      1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size4); // 3 indices starting at 0 -> 1 triangle

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}


void processPlayerInput() {
            
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float cameraSpeed = 100.0f * deltaTime;
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


 void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

 void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
 {
     fov -= (float)yoffset;
     if (fov < 1.0f)
         fov = 1.0f;
     if (fov > 45.0f)
         fov = 45.0f;
 }

bool initializeWindow()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Example: 3D Model with basic shading", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Mouse capturing
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

  //Mouse zoom
  glfwSetScrollCallback(window, scroll_callback);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  return true;
}

bool updateMVPTransformation()
{
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  Projection = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 10000.0f);

  // Camera matrix (modify this to let the camera move)
  View = glm::lookAt(
      cameraPos, cameraPos+cameraFront,cameraUp
  );

  // Model matrix, example with 3 parameters (curr_angle, curr_x, curr_y)
  glm::mat4 Model = glm::mat4(1.0f); //start with identity matrix

  //Model = glm::translate(Model, glm::vec3(curr_x, curr_y, 0.0f)); //apply translation
  Model = glm::rotate(Model, curr_angle, glm::vec3(0.0f, 0.0f, 70.0f)); //apply orientation (last parameter: axis)
  Model = glm::translate(Model, glm::vec3(curr_x, curr_y, curr_translate * 1.0f));

  curr_angle += 0.03;

  if (curr_translate <= -50.f)
  {
      up = true;
  }
  else if (curr_translate >= 50.f)
  {
      up = false;
  }

  if (up)
  {
      curr_translate += 0.5;
  }
  else
  {
      curr_translate -= 0.5;
  }


  // Our ModelViewProjection : multiplication of our 3 matrices
  MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
  MV = View * Model; //We also need MV in the shader to transform the light position

  return true;
}

bool rotate()
{
    // Model matrix, example with 3 parameters (curr_angle, curr_x, curr_y)
    glm::mat4 Model = glm::mat4(1.0f); //start with identity matrix

    Model = glm::translate(Model, glm::vec3(-200.f, curr_y, 0.0f)); //apply translation
    Model = glm::rotate(Model, curr_rotate, glm::vec3(0.0f, 0.0f, -70.0f)); //apply orientation (last parameter: axis)
    Model = glm::translate(Model, glm::vec3(200.f, curr_y, 0.0f)); //apply translation

    curr_rotate += 0.04;

    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
    MV = View * Model; //We also need MV in the shader to transform the light position

    return true;
}

bool rotate2()
{
    // Model matrix, example with 3 parameters (curr_angle, curr_x, curr_y)
    glm::mat4 Model = glm::mat4(1.0f); //start with identity matrix

    //Model = glm::translate(Model, glm::vec3(100.f, curr_y, 0.0f)); //apply translation
    Model = glm::rotate(Model, curr_rotate2, glm::vec3(70.0f, 0.0f, 0.0f)); //apply orientation (last parameter: axis)
    //Model = glm::translate(Model, glm::vec3(-100.f, curr_y, 0.0f)); //apply translation

    curr_rotate2 += 0.04;

    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
    MV = View * Model; //We also need MV in the shader to transform the light position

    return true;
}

bool rotate3()
{
    // Model matrix, example with 3 parameters (curr_angle, curr_x, curr_y)
    glm::mat4 Model = glm::mat4(1.0f); //start with identity matrix

    Model = glm::translate(Model, glm::vec3(280.f, curr_y, 0.0f)); //apply translation
    Model = glm::rotate(Model, curr_rotate4, glm::vec3(0.0f, 1.0f, 0.0f)); //apply orientation (last parameter: axis)
    Model = glm::translate(Model, glm::vec3(-280.f, curr_y, 0.0f)); //apply translation
    Model = glm::rotate(Model, -curr_rotate3, glm::vec3(0.0f, 0.0f, 10.0f)); //apply orientation (last parameter: axis)

    curr_rotate3 += 0.04;
    curr_rotate4 += 0.04;

    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
    MV = View * Model; //We also need MV in the shader to transform the light position

    return true;
}

bool initializeVertexbuffer()
{
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  //create vertex and normal data
  std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
  std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();
  std::vector< glm::vec2 > uvs = std::vector< glm::vec2 >();

  loadSTLFile(vertices, normals, "Ancient_Red_Dragon.stl", 0.f, 0.f, 0.f);
  //loadSTLFile(vertices, normals, "Red_Dragon_Adult_Flying_Updated");

  vertexbuffer_size = vertices.size() * sizeof(glm::vec3);
  
  glGenBuffers(2, vertexbuffer); //generate two buffers, one for the vertices, one for the normals
  
  //fill first buffer (vertices)
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //fill second buffer (normals)
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  return true;
}

bool initializeVertexbuffer2()
{
    glGenVertexArrays(1, &VertexArrayID2);
    glBindVertexArray(VertexArrayID2);

    //create vertex and normal data
    std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
    std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();

    loadSTLFile(vertices, normals, "loubie_aria_dragon.stl", -200.f, 0.f, 0.f);

    vertexbuffer_size2 = vertices.size() * sizeof(glm::vec3);

    glGenBuffers(2, vertexbuffer2); //generate two buffers, one for the vertices, one for the normals

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}

bool initializeVertexbuffer3()
{
    glGenVertexArrays(1, &VertexArrayID3);
    glBindVertexArray(VertexArrayID3);

    //create vertex and normal data
    std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
    std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();

    loadSTLFile(vertices, normals, "wyvern_complete.stl", 200.f, 0.f, 0.f);

    vertexbuffer_size3 = vertices.size() * sizeof(glm::vec3);

    glGenBuffers(2, vertexbuffer3); //generate two buffers, one for the vertices, one for the normals

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}

bool initializeVertexbuffer4()
{
    glGenVertexArrays(1, &VertexArrayID4);
    glBindVertexArray(VertexArrayID4);

    //create vertex and normal data
    std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
    std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();

    loadSTLFile(vertices, normals, "Dragon_flying.stl", 280.f, 0.f, 0.f);
    //loadSTLFile(vertices, normals, "Dragon_flying.stl", -560.f, 0.f, 0.f);

    vertexbuffer_size4 = vertices.size() * sizeof(glm::vec3);

    glGenBuffers(2, vertexbuffer4); //generate two buffers, one for the vertices, one for the normals

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteVertexArrays(1, &VertexArrayID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

void loadSTLFile(std::vector< glm::vec3 >& vertices, 
                 std::vector< glm::vec3 >& normals, 
                 std::string stl_file_name, float posX, float posY, float posZ)
{
  stl::stl_data info = stl::parse_stl(stl_file_name);
  std::vector<stl::triangle> triangles = info.triangles;
  for (int i = 0; i < info.triangles.size(); i++) {
    stl::triangle t = info.triangles.at(i);
    glm::vec3 triangleNormal = glm::vec3(t.normal.x, 
                                         t.normal.y, 
                                         t.normal.z);
    //add vertex and normal for point 1:
    vertices.push_back(glm::vec3(t.v1.x + posX, t.v1.y + posY, t.v1.z + posZ));
    normals.push_back(triangleNormal);
    //add vertex and normal for point 2:
    vertices.push_back(glm::vec3(t.v2.x + posX, t.v2.y + posY, t.v2.z + posZ));
    normals.push_back(triangleNormal);
    //add vertex and normal for point 3:
    vertices.push_back(glm::vec3(t.v3.x + posX, t.v3.y + posY, t.v3.z + posZ));
    normals.push_back(triangleNormal);
  }
}
