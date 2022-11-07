//#include <GL/glew.h> //用来管理函数指针的
//#include <GLFW/glfw3.h> //
//#include <iostream>
//
//
//int main(void)
//{
//  GLFWwindow* window;
//
//  /* Initialize the library */
//  if (!glfwInit())
//    return -1;
//  
//  /* Create a windowed mode window and its OpenGL context */
//  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//  if (!window)
//  {
//    glfwTerminate();
//    return -1;
//  }
//
//  /* Make the window's context current */
//  glfwMakeContextCurrent(window);
//
//
//  if (glewInit() != GLEW_OK) {
//    std::cout << "# Hey, error!";
//  }
//
//  std::cout << glGetString(GL_VERSION) << '\n';
//
//  //用一个变量来存储我们要传入VRAM的data，且这个变量是在堆栈上创建的可以用sizeof
//  float positions[6] = {
//    -0.5f, -0.5f,
//     0.0f,  0.5f,
//     0.5f, -0.5f
//  };
//
//
//  // 生成一个缓存区来放我们的data，
//  // 第二个参数是一个整数（指针），然后函数把id写入这个整数的内存
//  // 传入变量buffer的地址就是生成的缓冲区的id
//  // 在OpenGL中生成的所有东西都有一个id号
//  unsigned int buffer;
//  glGenBuffers(1, &buffer);  
//  //选择生成的buffer
//  //在OpenGL中称为绑定
//  glBindBuffer(GL_ARRAY_BUFFER, buffer);
//  //然后 把数据传入buffer
//  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
//  //到此为止，OpenGL的缓存区只是接收了一堆字节，它并不知道怎么解释这些字节数据
//
//
//
//  /* Loop until the user closes the window */
//  while (!glfwWindowShouldClose(window))
//  {
//    /* Render here */
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    //实际上绘制三角形的一行代码
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//
//    /* Swap front and back buffers */
//    glfwSwapBuffers(window);
//
//    /* Poll for and process events */
//    glfwPollEvents();
//  }
//
//  glfwTerminate();
//
//  return 0;
//}