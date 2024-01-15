#include "config.h" //EVERY SETTING (CONTROLS, VARIABLES, CONSTANCES etc.) ARE IN "config.h"

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	//READING SHADER SOURCES
	ifstream vshaderFile;
	vshaderFile.open("vertexShader.txt");
	string vertexShaderSource ;
	if (vshaderFile.is_open()) {
		string line = "";
		while (getline(vshaderFile, line)) {
			vertexShaderSource += line + "\n";
		}
	}
	const char* vertexShaderInput = vertexShaderSource.c_str();
	vshaderFile.close();

	ifstream fshaderFile("fragmentShader.txt");
	string fragmentShaderSource;
	if (fshaderFile.is_open()) {
		string line = "";
		while (getline(fshaderFile, line)) {
			fragmentShaderSource += line + "\n";
		}
	}
	const char* fragmentShaderInput = fragmentShaderSource.c_str();
	fshaderFile.close();


	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Engineer project", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Couldn't load opengl" << endl;
		glfwTerminate();
		return -1;
	}

	//BUFFER
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//CREATING SHADERS

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderInput, NULL);
	glCompileShader(vertexShader);

	//fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderInput, NULL);
	glCompileShader(fragmentShader);

	//check shaders
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) //vertex shader compilation status check
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		return -1;
	}
	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//END SHADERS

	//VERTICES AND SHIT
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 0. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//WINDOW
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  //black background

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //dont fill

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);

		//RENDER HERE
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * vertice_size));
		glBindVertexArray(0);
		//SOMETHING, idk
		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}