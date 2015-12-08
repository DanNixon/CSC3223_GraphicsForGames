#include "Renderer.h"

Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_time(0.0f)
    , m_animPosition(0.0f)
    , m_runAnim(false)
    , m_loopAnim(false)
{
  glEnable(GL_DEPTH_TEST);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

Renderer::~Renderer(void)
{
  m_renderObjects.clear();
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

    glUniform1f(glGetUniformLocation(program, "animPosition"), m_animPosition);
    glUniform1i(glGetUniformLocation(program, "objectTextures[0]"), 0);
    glUniform1i(glGetUniformLocation(program, "objectTextures[1]"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, o.GetTexture(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, o.GetTexture(1));

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

  if (m_runAnim)
  {
    if (m_animPosition >= 1.0)
    {
      if (m_loopAnim)
      {
        animStop();
        animStart(m_loopAnim);
      }
    }
    else
      m_animPosition += 0.0001;
  }

  for (vector<RenderObject *>::iterator i = m_renderObjects.begin(); i != m_renderObjects.end(); ++i)
  {
    (*i)->Update(msec);
  }
}

void Renderer::animStart(bool loop)
{
  m_runAnim = true;
  m_loopAnim = loop;
}

void Renderer::animPause()
{
  m_runAnim = false;
}

void Renderer::animStop()
{
  m_animPosition = 0.0f;
  m_runAnim = false;
}

GLuint Renderer::LoadTexture(string filename)
{
  return SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}