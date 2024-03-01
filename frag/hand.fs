#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 Color;

// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

vec4 calculateLighting(vec3 lightDirection)
{
    // ambient lighting
    float ambientStrength = 0.20f;
    vec3 ambient = ambientStrength * lightColor.rgb * Color;

    // diffuse lighting
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor.rgb * Color;

    // specular lighting
    float specularStrength = 0.50f;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor.rgb;

    // combine results
    vec3 result = ambient + diffuse + specular;
    return vec4(result, 1.0); // alpha is 1.0 for full opacity
}

void main()
{
    // Normalized light direction in world space
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f)); // Assuming the light is coming from the top-right front
    FragColor = calculateLighting(lightDir);
}