#ifndef Objemodel_H
#define Objemodel_H

class ObjModel {
public:
  

    ObjModel(const std::string& filename) {
        load(filename);
        setupMesh();
    }
    void draw(Shader& shader, Camera& camera, glm::vec3 offset, glm::vec3 scale, glm::quat rotQuat) {
        shader.Activate();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), camera.Position);

        if (!camera.isroll&& !camera.isRot&&!camera.Cartwheell && !camera.Cartwheelr) {
            glm::vec3 camRight = glm::normalize(glm::vec3(camera.view[0].x, 0.0f, camera.view[0].z));
            glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 camDir = glm::cross(camUp, camRight);
            glm::quat horizontalRotation = glm::quatLookAt(camDir, camUp);
            model *= glm::inverse(glm::toMat4(horizontalRotation));
            model = glm::translate(model, offset);
        }
        else {
            model = glm::translate(glm::inverse(camera.view), offset);
        }

        // 平移offset

        // 应用缩放和额外的旋转
        model = glm::scale(model, scale);
        model *= glm::toMat4(rotQuat);


        // 更新着色器的uniform变量
        glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);  // 假设每个 face 有 3 个顶点
        glBindVertexArray(0);
    }
    const std::vector<glm::vec3>& getVertices() const { return vertices; }
    const std::vector<glm::vec2>& getTextureCoords() const { return textureCoords; }
    const std::vector<glm::vec3>& getNormals() const { return normals; }
    const std::vector<Vertex>& getFaces() const { return faces; }

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2>textureCoords;
    std::vector<glm::vec3> normals;
    std::vector<Vertex> faces;
    unsigned int VAO, VBO, EBO;
    void load(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream lineStream(line);
            std::string prefix;
            lineStream >> prefix;

            if (prefix == "v") {
                glm::vec3 vertex;
                lineStream >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            else if (prefix == "vt") {
                glm::vec2 texCoord;
                lineStream >> texCoord.x >> texCoord.y;
                textureCoords.push_back(texCoord);
            }
            else if (prefix == "vn") {
                glm::vec3 normal;
                lineStream >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            else if (prefix == "f") {
                int vIndex, vtIndex, vnIndex;
                char separator;
                while (lineStream >> vIndex >> separator >> vtIndex >> separator >> vnIndex) {
                    vIndex--;
                    vtIndex--;
                    vnIndex--;
                    faces.push_back(Vertex{ vertices[vIndex], normals[vnIndex], glm::vec3(141.f, 85.f, 36.f) / 255.0f, textureCoords[vtIndex] });
                }

            }
        }
    }
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, faces.size() * sizeof(Vertex), &faces[0], GL_STATIC_DRAW);
        std::vector<unsigned int> indices;
        for (size_t i = 1; i <= faces.size()+1; i += 4) {
            indices.push_back(i - 1);
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i - 1);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        // Vertex Colors
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
        // Vertex Texture Coords
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float)));
        glBindVertexArray(0);
    }
};
#endif