#version 120

// if you change the version under this line you must change the version check in G3DPainter.cpp
// vertex shader version 02.04.15

// selection color of points
uniform mediump vec4 selectionColor;

// selection check
uniform int checkSelected;

// maximum of 64 coordinate system
uniform vec3 csOffset[64];

// the matrix of the camera (without offset)
uniform mat4 camMatrix;

// info of the point
attribute float info;

// index of the coordinate system matrix
attribute float csIndex;

int andOperator(int a, int b)
{
    int c = 0;
    for (int x = 0; x < 32; ++x)
    {
        c += c;
        if (a < 0) {
            if (b < 0) {
                c += 1;
            }
        }
        a += a;
        b += b;
    }

    return c;
}

void main()
{
    int infoInt = int(info);
	int csIndexInt = int(csIndex);

    if(andOperator(infoInt,checkSelected) > 0)
    {
        gl_FrontColor = selectionColor;
    }
    else
    {
        gl_FrontColor = gl_Color;
    }

    vec3 cs = csOffset[csIndexInt];
		
	vec4 newVertex = gl_Vertex;
	newVertex[0] -= cs[0];
	newVertex[1] -= cs[1];
	newVertex[2] -= cs[2];
	
    gl_Position = gl_ModelViewProjectionMatrix * camMatrix * newVertex;
}
