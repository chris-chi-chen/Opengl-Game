//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
#include "config.h"
const unsigned int width = 1200;
const unsigned int height = 800;
std::vector<collobj> objectcontent;
// object create===================================================================================================================================
//sky box
float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
}; 
unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};
//plane
// Vertices coordinates
std::vector<Vertex> vertices=
{ //               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-105.0f, -3.0f,  210.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(-105.0f, -3.0f, -100.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(195.0f, -3.0f, -100.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
	Vertex{glm::vec3(195.0f, -3.0f,  210.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}
};

// Indices for vertices order
std::vector<GLuint> indices =
{
	0, 1, 2,
	0, 2, 3
};
// object create===================================================================================================================================
void keyCallback(GLFWwindow* window, int key, int, int action, int) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}
}
int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	glfwSetKeyCallback(window, keyCallback);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	std::string texPath = "../obj/";
	std::string textPath = "../texture/";

	// Texture data

	// Original code from the tutorial
	/*Texture textures[]
	{
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};*/
	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("../vert/default.vs", "../frag/default.fs");
	Shader skybox("../vert/skybox.vs", "../frag/skybox.fs");
	Shader handshader("../vert/hand.vs", "../frag/hand.fs");
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(-105.f, 300.f, 210.f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	//light
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	handshader.Activate();
	glUniform4f(glGetUniformLocation(handshader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(handshader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	//===================================================================


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 50.f, 3.0f));
	Model model((texPath + "industrial_building/scene.gltf").c_str(),0);
	Model model2((texPath + "building/scene.gltf").c_str(), 0); 
	Model model3((texPath + "building_pack/scene.gltf").c_str(), 0);
	Model model4((texPath + "old_barrel/scene.gltf").c_str(), 0);
	Model City((texPath + "Cyber_City/city.gltf").c_str(), 33);
	ObjModel hand_r((texPath + "hand/hand_l.obj").c_str());
	ObjModel leg_r((texPath + "leg/f_leg.obj").c_str());
	/*Model roadsec1((texPath + "road_pack/scene.gltf").c_str(),16);
	Model roadsec2((texPath + "road_pack/scene.gltf").c_str(), 17);
	Model road((texPath + "road_pack/scene.gltf").c_str(), 0);*/
    std::vector<glm::vec3> all;
	std::vector<unsigned int> allind;
	float planeX_m = 0;
	float planeX_mx = 0;
	for (unsigned int i = 0; i < size(City.meshes); i++) {
		for (const auto& point : City.meshes[i].vertices) {
			all.push_back(glm::vec3(City.matricesMeshes[i] *glm::vec4(point.position,1)));
		}
	}
	int current = 0;
	for (unsigned int i = 0; i < size(City.meshes); i++) {
		for (const auto& point : City.meshes[i].indices) {
			allind.push_back(point + current);
		}
		current += City.meshes[i].vertices.size();
	}
	/*Model plane((texPath + "ground/scene.gltf").c_str());*/
	std::vector<Texture> textures
	{
		Texture((textPath + "desertTexture.bmp").c_str(), "diffuse", 0)
	};

	std::vector<std::string> texs = 
	{
		"../texture/scene1/px.png", "../texture/scene1/nx.png", "../texture/scene1/py.png",
						"../texture/scene1/ny.png", "../texture/scene1/pz.png", "../texture/scene1/nz.png"
	};
	Sky sky(skyboxVertices, skyboxIndices, texs);
	Mesh land(vertices, indices, textures,1.0f,1.0f);
	// Creates camera object
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, &camera);
	//adjust section==================================
	glm::quat rotationX = glm::angleAxis(glm::radians(135.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat rotationY = glm::angleAxis(glm::radians(170.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rotationZ = glm::angleAxis(glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat correctRotation = rotationX * rotationY * rotationZ;
	glm::quat correctRotationL = glm::angleAxis(glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat correctRotationLr = glm::angleAxis(glm::radians(75.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat rotation = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f)) ;
	glm::quat rotation2 = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rotation4 = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat rotation3 = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat rotation1 = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//adjust section==================================
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		// Handles camera inputs
		camera.Inputs(window,all,allind);
		// Draws different meshes
		City.Draw(shaderProgram, camera, 1, glm::vec3(0.0f, 0.0f, 0.0f), rotation, glm::vec3(1.0f, 1.0f, 1.0f));
		/*model.Draw(shaderProgram, camera, 1, glm::vec3(0.0f, 0.0f, 0.0f), rotation,glm::vec3(1.0f,1.0f,1.0f));
		model2.Draw(shaderProgram, camera, 1, glm::vec3(0.0f, 2.5f, -0.6f), rotation3, glm::vec3(0.5f, 0.5f, 0.5f));
		model3.Draw(shaderProgram, camera, 1, glm::vec3(0.0f, 0.45f, 3.f), rotation2, glm::vec3(0.1f, 0.1f, 0.1));
		model4.Draw(shaderProgram, camera, 1, glm::vec3(10.0f, 10.f, 10.f), rotation, glm::vec3(1.f, 1.f, 1.f));*/
		//road.Draw(shaderProgram, camera, 1, glm::vec3(1219.f, 1.5f, 400.0f), rotation2, glm::vec3(0.1f, 0.1f, 0.1f), 0, size(road.poss));
		////intersection
		//roadsec1.Draw(shaderProgram, camera, 1, glm::vec3(0.f, 0.f, 0.f), rotation, glm::vec3(1.0f, 1.0f, 1.0f),0,size(roadsec1.poss));//建完馬路
		//roadsec2.Draw(shaderProgram, camera, 1, glm::vec3(0.f, 0.f, 0.f), rotation, glm::vec3(1.0f, 1.0f, 1.0f), 0, size(roadsec2.poss));//建完馬路
		/*intersection*/
		land.Draw(shaderProgram, camera,1,glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		sky.draw(skybox, camera);
		glm::vec3 offset_r =  glm::vec3(-0.8f, 0.0f, 0.0f) * 0.05f + glm::vec3(0.f, -3.5f, 0.0f) * 0.05f + glm::vec3(0.f, 0.0f, 0.5f) * 0.05f;
		glm::vec3 offset_l = glm::vec3(0.8f, 0.0f, 0.0f) * 0.05f + glm::vec3(0.f, -3.5f, 0.0f) * 0.05f + glm::vec3(0.f, 0.0f, 0.5f) * 0.05f;
		offset_r -= glm::vec3(0.0f, 1.0f, 0.0f) * (camera.hand_r_control*0.005f);
		offset_l -= glm::vec3(0.0f, 1.0f, 0.0f) * (camera.hand_l_control*0.005f);
		hand_r.draw(handshader, camera, offset_r, glm::vec3(0.001f, 0.001f, 0.001f), correctRotation);
		if (camera.isroll||camera.isRot) {
			leg_r.draw(handshader, camera, glm::vec3(0.2f, camera.rollleg * -0.00139f, -1.0f) * 0.12f , glm::vec3(0.005f, 0.005f, 0.005f), correctRotationLr);
			leg_r.draw(handshader, camera, glm::vec3(-0.2f, camera.rollleg * -0.00139f, -1.0f) * 0.12f , glm::vec3(-0.005f, 0.005f, 0.005f), correctRotationLr);
		}
		else {
			leg_r.draw(handshader, camera, glm::vec3(0.2f, -0.65f, -0.3f) * 0.12f - glm::vec3(0.0f, 0.5f, 1.0f) * (camera.leg_r_control * 0.0015f), glm::vec3(0.005f, 0.005f, 0.005f), correctRotationL);
			leg_r.draw(handshader, camera, glm::vec3(-0.2f, -0.65f, -0.3f) * 0.12f - glm::vec3(0.0f, 0.5f, 1.0f) * (camera.leg_l_control * 0.0015f), glm::vec3(-0.005f, 0.005f, 0.005f), correctRotationL);
		}
		hand_r.draw(handshader, camera, offset_l, glm::vec3(-0.001f, 0.001f, 0.001f), correctRotation);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
	}




	// Delete all the objects we've created
	shaderProgram.Delete();
	handshader.Delete();
	skybox.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}