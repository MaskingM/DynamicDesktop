#version 330 core
in vec2 coord;
uniform sampler2D frame;
out vec4 FragColor;
void main(){
	vec4 color = texture(frame, coord);
	FragColor = vec4(color.b,color.g,color.r,color.a);
}