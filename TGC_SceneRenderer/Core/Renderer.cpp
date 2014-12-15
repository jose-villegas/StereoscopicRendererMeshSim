#include "Renderer.h"

using namespace Core;

#define DRAW_TEST_TRIANGLE
#ifdef DRAW_TEST_TRIANGLE
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

GLuint vArrayID;
GLuint vBuffer;
Types::ShaderProgram *shProgram;
#endif // DRAW_TEST_TRIANGLE

Renderer::Renderer(void)
{
}

bool Core::Renderer::load()
{
    return (bool)ogl_LoadFunctions();
}

void Core::Renderer::setup()
{
    // Setup OpenGL Flags
    glEnable(GL_DEPTH_TEST);
    // Triangle Setup
#ifdef DRAW_TEST_TRIANGLE
    glGenVertexArrays(1, &vArrayID);
    glBindVertexArray(vArrayID);
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    shProgram = new Types::ShaderProgram();
    Types::Shader *frag = new Types::Shader(Types::Shader::Fragment);
    Types::Shader *vert = new Types::Shader(Types::Shader::Vertex);
    frag->loadFromFile("../TGC_SceneRenderer/Resources/Shaders/default_frag.glsl");
    vert->loadFromFile("../TGC_SceneRenderer/Resources/Shaders/default_vert.glsl");
    frag->compile();
    vert->compile();
    shProgram->attachShader(*frag);
    shProgram->attachShader(*vert);
    shProgram->link();
#endif
}

void Core::Renderer::loop()
{
    // Clear the color and depth buffers.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef DRAW_TEST_TRIANGLE
    shProgram->use();
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void *)0           // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
#endif
}
