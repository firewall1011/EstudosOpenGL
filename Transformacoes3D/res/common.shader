#shader vertex
attribute vec3 position;
uniform mat4 transformation_matrix;
void main()
{
    gl_Position = transformation_matrix * vec4(position, 1.0);
}
#shader fragment
uniform vec4 color;
void main()
{
    gl_FragColor = color;
}
