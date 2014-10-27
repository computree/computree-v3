#version 130

// selection color of points
uniform mediump vec4 selectionColor;

// selection check
uniform int checkSelected;

// info of the point
attribute float info;

void main()
{
    uint infoUint = uint(info);

    if((infoUint & uint(checkSelected)) > 0u)
    {
        gl_FrontColor = selectionColor;
    }
    else
    {
        gl_FrontColor = gl_Color;
    }

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
