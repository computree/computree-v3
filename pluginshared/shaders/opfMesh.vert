#version 120

//transform matrix
uniform mat4 tMatrix;

// bounding box : maxX - minX
uniform float deltaX;

// dUp - dDown
uniform float deltaD;

// bounding box : minX
uniform float minX;

// dDown
uniform float dDown;

void main()
{
	float dx = gl_Vertex.x - minX;
	float factorW = dDown + (deltaD * (dx / deltaX));

	vec4 outVertex = gl_Vertex;
	outVertex.y = outVertex.y * factorW;
	outVertex.z = outVertex.z * factorW;

	gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * tMatrix * outVertex;
}
