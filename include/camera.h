#ifndef CAMERA_H
#define CAMERA_H
// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::quat rotation = glm::quat(glm::identity<glm::quat>());
	glm::vec3 n;
	//contorl falling========================
	bool isfalling = false;
	//========================================
	//contorl big jump========================
	float jump_init_big = 0.08;
	bool isJumpingM = false;
	//========================================
	bool put_down = true;
	float pitch = 0.0f;
	float jump_init_small = 0.05;
	bool isCrouching = false;
	bool isRightUp = false;
	float crouchDepth = 0.05f;
	float fall = 0.0f;
	//// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;
	bool isJumping = false;
	bool onbox = false;
	bool isrun = false;
	float jumpAcceleration = 0.0035f;      
	float currentJumpSpeed = 0.0f;  
	float gravity = 0.002f;         
	float groundPosition = -2.7f;   
	int hand_l_control = 36;
	int hand_r_control = 36;
	int rollleg = 180;
	bool hand_l_up = 0;
	bool hand_r_up = 0;
	int leg_l_control = 0;
	int leg_r_control = 0;
	bool leg_l_up = 0;
	bool leg_r_up = 0;
	bool isLeftUp = false;
	bool legPutDown = false;
	float bobbingSpeed = 0.25f; 
	float bobbingAmount = 0.001f; 
	float bobbing = 0.0f;
	int rolling = 0;
	bool isroll = 0;
	bool fall_start = 1;
	bool Cartwheell = false;
	bool Cartwheelr = false;
	// Stores the width and height of the window
	int width;
	int height;
	float know_height = -1.0f;
	int crouchStep = 0;
	bool isRot = false;
	// Adjust the speed of the camera and it's sensitivity when looking around
	float keyboardMoveSpeed = 0.03f;
	float mouseMoveSpeed = 0.001f;
	float cartwheelTime = 0.f;
	int counting = 0;
	glm::vec3 temp_right;
	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position)
	{
		Camera::width = width;
		Camera::height = height;
		Position = position;
		projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
	}
	void updateMatrix()
	{
		constexpr glm::vec3 original_front(0, 0, -1);
		constexpr glm::vec3 original_up(0, 1, 0);
		Orientation = glm::normalize(rotation * original_front);
		Up = glm::normalize(rotation * original_up);
		right = glm::normalize(glm::cross(Orientation, Up));
		view = glm::lookAt(Position, Position + Orientation, Up);
		cameraMatrix = projection * view;
	}
	glm::vec3 calculateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		glm::vec3 v = p2 - p1;
		glm::vec3 w = p3 - p1;
		return glm::cross(v, w);
	}
	void Matrix(Shader& shader, const char* uniform)
	{
		// Exports camera matrix
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
	}
	bool CheckCameraCollision(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, Sphere& cameraSphere) {
		for (size_t i = 1; i < indices.size()+1; i += 3) {
			Triangle triangle(
				vertices[indices[i-1]],     
				vertices[indices[i]], 
				vertices[indices[i + 1]]  
			);

			if (SphereTriangleIntersect(cameraSphere, triangle)) {
				n = calculateNormal(triangle.v0, triangle.v1, triangle.v2);
				float A = n.x;
				float B = n.y;
				float C = n.z;
				float D = -(A * triangle.v0.x + B * triangle.v0.y + C * triangle.v0.z);
				if (glm::dot(n, glm::vec3(0, 1, 0)) < 0) {
					n = -n; 
				}
				float dotProduct = glm::dot(glm::normalize(n), glm::vec3(0, 1, 0));

				float angleRadians = acos(dotProduct);
				float angle = glm::degrees(angleRadians);
				if (angle <= 60) {
					know_height = -(A * cameraSphere.center.x + C * cameraSphere.center.z + D) / B;
				}
				else {
					know_height = -1.0f; // Or any other default value you wish to use
				}
				return true; // Collision detected
			}
		}
		return false; // No collision detected
	}

	void Inputs(GLFWwindow* window, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices)
	{
		bool ismoved = false;
		bool istrans = false;
		bool isfalling = false;
		glm::vec3 move = glm::vec3(0.f,0.f,0.f);
		isrun = false;
		glm::vec3 newposition = Position;
		// Mouse part
		static double lastx = 0, lasty = 0;
		if (Cartwheell) {
			float anglePerFrame = glm::radians(6.f);
			glm::quat cartwheelRotation = glm::angleAxis(anglePerFrame, glm::vec3(0, 0, 1)); 

			rotation *= cartwheelRotation;
			Position -= temp_right * 0.01f;
			updateMatrix();
			if(counting<30)
				Position -= glm::vec3(0.f, 1.f, 0.f) * 0.004f;
			else
				Position += glm::vec3(0.f, 1.f, 0.f) * 0.004f;
			cartwheelTime += 6.f;
			counting += 1;
			if (cartwheelTime >359) {
				Cartwheell = false; 
				cartwheelTime = 0.0f; 
				counting = 0;
			}

			return;
		}
		if (Cartwheelr) {
			float anglePerFrame = glm::radians(6.f);
			glm::quat cartwheelRotation = glm::angleAxis(anglePerFrame, glm::vec3(0, 0, -1));

			rotation *= cartwheelRotation;
			Position += temp_right * 0.01f;
			updateMatrix();
			if (counting < 30)
				Position -= glm::vec3(0.f, 1.f, 0.f) * 0.004f;
			else
				Position += glm::vec3(0.f, 1.f, 0.f) * 0.004f;
			cartwheelTime += 6.f;
			counting += 1;
			if (cartwheelTime > 359) {
				Cartwheelr = false;
				cartwheelTime = 0.0f;
				counting = 0;
			}

			return;
		}
		if (isCrouching&&!isRot) {
				if (crouchStep < 5) {
					Position.y -= (0.1 / 10);
				}
				else {
					Position.y += (0.1 / 30);
				}
				updateMatrix();
				crouchStep++;
				if (crouchStep >= 30) {
					crouchStep = 0;  
					isCrouching = false;  
				}

			return;
		}
		if (isroll||isRot) {
			float anglePerFrame = glm::radians(-4.0f); 
			glm::quat ro(glm::angleAxis(anglePerFrame, glm::vec3(1, 0, 0)));
			rotation = rotation * ro;
			Position += glm::normalize(glm::vec3(Orientation[0], 0, Orientation[2])) * 0.0001f;

			updateMatrix();
			if (rolling > 359)
			{
				isroll = false;
				isRot = false;
				rolling = 0;
				rollleg = 180;
			}
			rollleg += 4;
			rolling += 4;

		}
		if (lastx == 0 && lasty == 0) {
			glfwGetCursorPos(window, &lastx, &lasty);
		}
		else {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			float dx = mouseMoveSpeed * static_cast<float>(xpos - lastx);
			float dy = mouseMoveSpeed * static_cast<float>(lasty - ypos);
			lastx = xpos;
			lasty = ypos;
			if (dx != 0 || dy != 0) {
				ismoved = true;
				glm::quat rx(glm::angleAxis(dx, glm::vec3(0, -1, 0)));
				const float maxPitch = glm::radians(30.0f);
				const float minPitch = -maxPitch;

				float newPitch = pitch + dy;

				if (newPitch > maxPitch) {
					dy = maxPitch - pitch;
					newPitch = maxPitch;
				}
				if (newPitch < minPitch) {
					dy = minPitch - pitch;
					newPitch = minPitch;
				}

				pitch = newPitch;
				glm::quat ry = glm::angleAxis(dy, glm::vec3(1, 0, 0));
				rotation = rx * rotation * ry;
			}
		}
		if (!isJumping && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			isJumping = true;
			currentJumpSpeed = jump_init_small;
		}
		/*if (!isJumpingM&& !isJumping && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			isJumpingM = true;
			currentJumpSpeed = jump_init_big;
		}*/
		if (isJumping && !fall_start) {
			newposition.y += currentJumpSpeed;
			currentJumpSpeed -= gravity;

			if (currentJumpSpeed < 0.0f) {
				fall_start = true;
				currentJumpSpeed = 0.0f;
				fall = 0.0f;
			}

			ismoved = true;
		}
		if ((!onbox && !isJumping) || fall_start) {
			/*std::cout << "start fall!";*/
			newposition.y += fall;
			fall -= gravity;
			if (newposition.y < groundPosition) {
				newposition = glm::vec3(0.0f, 50.f, 3.0f);
				fall = 0.0f;
			}
			ismoved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			move = glm::normalize(glm::vec3(Orientation[0], 0, Orientation[2])) * keyboardMoveSpeed;//modeify
			ismoved = true;
			istrans = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			move = -glm::normalize(glm::vec3(Orientation[0], 0, Orientation[2])) * keyboardMoveSpeed;//modeify
			ismoved = true;
			istrans = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			move = -glm::normalize(glm::vec3(right[0], 0, right[2])) * keyboardMoveSpeed;
			ismoved = true;
			istrans = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			move = glm::normalize(glm::vec3(right[0], 0, right[2])) * keyboardMoveSpeed;
			ismoved = true;
			istrans = true;
		}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			isrun = true;
			move *= 1.5;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			isroll = true;
		}
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS){
			temp_right = glm::vec3(right[0], 0.f, right[2]);
			Cartwheell = true;
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
			temp_right = glm::vec3(right[0], 0.f, right[2]);
			Cartwheelr = true;
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			newposition = glm::vec3(0.0f, 50.f, 3.0f);
			fall = 0.0f;
		}
		newposition += move;
		// Update view matrix if moved
		Sphere cameraSphere(newposition, 0.3f);
		bool isColliding = CheckCameraCollision(vertices, indices, cameraSphere);
		if (ismoved) {
				if (istrans) {
					if (isrun) {
						if (isRightUp) {
							if (hand_l_up && put_down)
							{
								hand_l_control -= 1;
								if (hand_l_control <= 0)
								{
									hand_l_control = 0;
									hand_l_up = 0;
								}
							}
							else
							{
								hand_l_control += 1;
								if (hand_l_control >= 36)
								{
									hand_l_control = 36;
									hand_l_up = 1;
								}
							}
						}
						if (hand_r_up && put_down)
						{
							hand_r_control -= 1;
							if (hand_r_control <= 0)
							{
								hand_r_control = 0;
								hand_r_up = 0;
								isRightUp = true;
							}
						}
						else
						{
							hand_r_control += 1;
							if (hand_r_control >= 36)
							{
								hand_r_control = 36;
								hand_r_up = 1;
							}
						}
						if (leg_l_up && legPutDown)
						{
							leg_l_control += 1;
							if (leg_l_control >= 36)
							{
								leg_l_control = 36;
								leg_l_up = 0;
								isLeftUp = true;
							}
						}
						else
						{
							leg_l_control -= 1;
							if (leg_l_control <= 0)
							{
									leg_l_control = 0;
									leg_l_up = 1;
							}
						}
						if (isLeftUp) {
							if (leg_r_up && legPutDown)
							{
								leg_r_control += 1;
								if (leg_r_control >= 36)
								{
									leg_r_control = 36;
									leg_r_up = 0;
								}
							}
							else
							{
								leg_r_control -= 1;
								if (leg_r_control <= 0)
								{
									leg_r_control = 0;
									leg_r_up = 1;
								}
							}
						}
						if (!put_down && (hand_l_control != 36 || hand_r_control != 36)) {
							hand_l_control += 1;
							hand_r_control += 1;
							if (hand_l_control >= 36)
								hand_l_control = 36;
							if (hand_r_control >= 36)
								hand_r_control = 36;
						}
						else if (!put_down && hand_l_control == 36 && hand_r_control == 36) {
							put_down = true;
							hand_l_up = 0;
							hand_r_up = 0;
						}
						if (!legPutDown && (leg_l_control != 0 || leg_r_control != 0)) {
							leg_l_control -= 1;
							leg_r_control -= 1;
							if (leg_l_control <= 0)
								leg_l_control = 0;
							if (leg_r_control <= 0)
								leg_r_control = 0;
						}
						else if (!legPutDown && leg_l_control == 0 && leg_r_control == 0) {
							legPutDown = true;
							leg_l_up = 0;
							leg_r_up = 0;
						}
						bobbing += bobbingSpeed;
						newposition.y += sin(bobbing) * bobbingAmount;
					}
					else {
						put_down = false;
						isRightUp = false;
						if (hand_l_control < 36)
							hand_l_control += 1;
						if (hand_r_control < 36)
							hand_r_control += 1;
						bobbing = 0.0f;
						legPutDown = false;
						isLeftUp = false;
						if (leg_l_control > 0)
							leg_l_control -= 1;
						if (leg_r_control > 0)
							leg_r_control -= 1;
					}
				}
				else {
					put_down = false;
					isRightUp = false;
					if (hand_l_control < 36)
						hand_l_control += 1;
					if (hand_r_control < 36)
						hand_r_control += 1;
					legPutDown = false;
					isLeftUp = false;
					if (leg_l_control > 0)
						leg_l_control -= 1;
					if (leg_r_control > 0)
						leg_r_control -= 1;
				}
		}
		if (isColliding) {
			if (know_height < newposition.y&& know_height!=-1.0)
			{
					Position = newposition;
					Position.y = know_height + 0.3f;
					onbox = true;
					isJumping =false;
					fall_start = false;
					std::cout << fall;
					if (fall < -0.08f)
					{
						isCrouching = true;
					}
					if (fall < -0.12f)
					{
						isRot = true;
					}
					fall = 0.0f;
			}
			else if(know_height == -1.0){
				Position.y = newposition.y;
				isJumping = false;
				fall_start = false;
				onbox = true;
			}
		}
		else
		{
				Position = newposition;
				onbox = false;
				know_height = -1.0;
				if (currentJumpSpeed < 0.0f) {
					fall_start = true;
					currentJumpSpeed = 0.0f;
					fall = 0.0f;
				}
		}
		updateMatrix();
	}
};
#endif
