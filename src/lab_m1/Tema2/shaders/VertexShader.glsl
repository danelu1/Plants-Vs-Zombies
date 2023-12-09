#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float lifeLocation;
uniform int componentLocation;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_color;
out vec3 frag_normal;
out vec2 frag_texture;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position = v_position;
    frag_color = v_color;
    frag_normal = v_normal;
    frag_texture = v_texture;

    if (componentLocation == 1) {
        vec3 bodyColor = vec3(0, 0.5, 0);
        vec3 grayColor = vec3(0);
        float aux = lifeLocation / 100;
        frag_color = mix(bodyColor, grayColor, aux);
    }
    if (componentLocation == 2) {
        vec3 turretColor = vec3(0, 0.7, 0);
        vec3 grayColor = vec3(0);
        float aux = lifeLocation / 100;
        frag_color = mix(turretColor, grayColor, aux);
    }
    if (componentLocation == 3) {
        vec3 otherComponentsColor = vec3(0.7, 0, 0);
        vec3 grayColor = vec3(0);
        float aux = lifeLocation / 100;
        frag_color = mix(otherComponentsColor, grayColor, aux);
    }

    float auxlifeLocation;
    vec3 auxPosition = v_position;

    if (lifeLocation == 40) {
        auxlifeLocation = 0.05;
        auxPosition.x = v_position.x * exp(-auxlifeLocation);
        auxPosition.z = v_position.z * exp(-auxlifeLocation);
    }
    if (lifeLocation == 30) {
        auxlifeLocation = 0.1;
        auxPosition.x = v_position.x * exp(-auxlifeLocation);
        auxPosition.z = v_position.z * exp(-auxlifeLocation);
    }
    if (lifeLocation == 20) {
        auxlifeLocation = 0.2;
        auxPosition.x = v_position.x * exp(-auxlifeLocation);
        auxPosition.z = v_position.z * exp(-auxlifeLocation);
    }
    if (lifeLocation == 10) {
        auxlifeLocation = 0.25;
        auxPosition.x = v_position.x * exp(-auxlifeLocation);
        auxPosition.z = v_position.z * exp(-auxlifeLocation);
    }
    if (lifeLocation == 0) {
        auxlifeLocation = 0.3;
        auxPosition = v_position * exp(-auxlifeLocation);
    }

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(auxPosition, 1.0);
}
