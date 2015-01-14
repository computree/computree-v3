#version 120

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

    gl_Position = gl_ModelViewProjectionMatrix * csMatrix[csIndexInt] * gl_Vertex;
}
