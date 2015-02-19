#version 120

// matrix that modify coordinates of points
uniform mat4 pMatrix;

void main()
{
	gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * pMatrix * gl_Vertex;
}