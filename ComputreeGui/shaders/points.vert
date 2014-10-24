#version 130

// selection color of points
uniform mediump vec4 selectionColor;

// selection check
uniform int checkSelected;

// invisibility check
uniform int checkInvisible;

// info of the point
attribute float info;

void main()
{
    uint infoUint = uint(info);

    if((infoUint & uint(checkInvisible)) > 0u)
    {
        gl_FrontColor = vec4(0,0,0,0);
    }
    else if((infoUint & uint(checkSelected)) > 0u)
    {
        gl_FrontColor = selectionColor;
    }
    else
    {
        gl_FrontColor = gl_Color;
    }

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
