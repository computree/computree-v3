#version 130

// selection color of points
uniform mediump vec4 selectionColor;

// selection check
uniform int checkSelected;

// maximum of 64 coordinate system (4x4 matrix)
uniform mat4 csMatrix[64];

// info of the point
attribute float info;

// index of the coordinate system matrix
attribute float csIndex;

void main()
{
    int infoInt = int(info);
	int csIndexInt = int(csIndex);

    if((infoInt & checkSelected) > 0)
    {
        gl_FrontColor = selectionColor;
    }
    else
    {
		gl_FrontColor = gl_Color;
    }

    gl_Position = gl_ModelViewProjectionMatrix * csMatrix[csIndexInt] * gl_Vertex;
}