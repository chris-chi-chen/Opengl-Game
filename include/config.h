#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <vector>
#include<cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include<shader.h>
#include<VBO.h>
#include<EBO.h>
#include<VAO.h>
#include<texture.h>
#include<json.h>
#include<Collision_detection.h>
#include<Narrow_Col.h>
#include<camera.h>
#include<Mesh.h>
#include<Model.h>
#include<Skycube.h>
#include<Objmodel.h>