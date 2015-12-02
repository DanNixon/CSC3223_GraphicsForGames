#include "Renderer.h"

Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
{
  glEnable(GL_DEPTH_TEST);

  m_time = 0.0f;

  for (int i = 0; i < NUM_TEXTURES; i++)
    m_textures[i] = 0;
}

Renderer::~Renderer(void)
{
  m_renderObjects.clear();

  for (int i = 0; i < NUM_TEXTURES; i++)
  {
    if (m_textures[i] != 0)
      glDeleteTextures(1, &m_textures[i]);
  }
}

void Renderer::RenderScene()
{
  for (vector<RenderObject *>::iterator i = m_renderObjects.begin(); i != m_renderObjects.end(); ++i)
  {
    Render(*(*i));
  }
}

void Renderer::Render(const RenderObject &o)
{
  modelMatrix = o.GetWorldTransform();

  if (o.GetShader() && o.GetMesh())
  {
    GLuint program = o.GetShader()->GetShaderProgram();

    glUseProgram(program);
    UpdateShaderMatrices(program);

    glUniform1f(glGetUniformLocation(program, "time"), m_time);
    glUniform1i(glGetUniformLocation(program, "objectTexture"), 0);
    glUniform1i(glGetUniformLocation(program, "textures[0]"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, o.GetTexture());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);

    o.Draw();
  }

  for (vector<RenderObject *>::const_iterator i = o.GetChildren().begin();
       i != o.GetChildren().end(); ++i)
  {
    Render(*(*i));
  }
}

void Renderer::UpdateScene(float msec)
{
  m_time += msec;

  for (vector<RenderObject *>::iterator i = m_renderObjects.begin(); i != m_renderObjects.end(); ++i)
  {
    (*i)->Update(msec);
  }
}

GLuint Renderer::LoadTexture(string filename)
{
  return SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}

GLuint Renderer::LoadTexture(string filename, int idx)
{
  if (idx > NUM_TEXTURES)
    return 0;

  m_textures[idx] = LoadTexture(filename);
  return m_textures[idx];
}