#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 _coord;
out vec2 coord;
void main(){
   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
   coord.x = _coord.x;
   coord.y = 1.0 - _coord.y;
}