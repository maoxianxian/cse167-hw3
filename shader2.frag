#version 330 core

out vec4 color;
in vec3 normalvecin;
void main()
{
    		color = vec4((normalvecin.x+1)/2.0f, (normalvecin.y+1)/2.0f, (normalvecin.z+1)/2.0f, 1.0f);
}