#ifndef VBO_CLASS_H
#define VBO_CLASS_H

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};
struct collobj {
	glm::vec3 postion;
};
class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	// Binds the VBO
	void Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}
	// Unbinds the VBO
	void Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	// Deletes the VBO
	void Delete()
	{
		glDeleteBuffers(1, &ID);
	}
};

#endif
