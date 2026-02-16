#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
//layout (location = 1) in vec3 aColor;

out vec3 vertexColor; // specify a color output to the fragment shader
uniform float wAspect;
uniform vec3 color;

void main()
{
   // Correct aspect ratio for both landscape and portrait orientations
   vec2 pos = wAspect > 1.0 
       ? vec2(aPos.x, aPos.y * wAspect)        // Landscape: stretch Y
       : vec2(aPos.x / wAspect, aPos.y);       // Portrait: stretch X

   gl_Position = vec4(pos, aPos.z, 1.0);
   vertexColor = color;
}