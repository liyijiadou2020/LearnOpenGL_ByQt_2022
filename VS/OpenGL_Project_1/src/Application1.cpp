//#include <GL/glew.h> //����������ָ���
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
//  //��һ���������洢����Ҫ����VRAM��data��������������ڶ�ջ�ϴ����Ŀ�����sizeof
//  float positions[6] = {
//    -0.5f, -0.5f,
//     0.0f,  0.5f,
//     0.5f, -0.5f
//  };
//
//
//  // ����һ���������������ǵ�data��
//  // �ڶ���������һ��������ָ�룩��Ȼ������idд������������ڴ�
//  // �������buffer�ĵ�ַ�������ɵĻ�������id
//  // ��OpenGL�����ɵ����ж�������һ��id��
//  unsigned int buffer;
//  glGenBuffers(1, &buffer);  
//  //ѡ�����ɵ�buffer
//  //��OpenGL�г�Ϊ��
//  glBindBuffer(GL_ARRAY_BUFFER, buffer);
//  //Ȼ�� �����ݴ���buffer
//  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
//  //����Ϊֹ��OpenGL�Ļ�����ֻ�ǽ�����һ���ֽڣ�������֪����ô������Щ�ֽ�����
//
//
//
//  /* Loop until the user closes the window */
//  while (!glfwWindowShouldClose(window))
//  {
//    /* Render here */
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    //ʵ���ϻ��������ε�һ�д���
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