#version 120

// selection color of points
uniform mediump vec4 selectionColor;

// selection check
uniform int checkSelected;

// info of the point
attribute float info;

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

    if(andOperator(infoInt,checkSelected) > 0)
    {
        gl_FrontColor = selectionColor;
    }
    else
    {
        gl_FrontColor = gl_Color;
    }

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
