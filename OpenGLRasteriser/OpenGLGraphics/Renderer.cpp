#include "Renderer.h"

Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_time(0.0f)
    , m_animPosition(0.0f)
    , m_animDelta(0.00025)
    , m_runAnim(false)
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
    ApplyShaderLight(program);

    Matrix3 rotation = Matrix3(viewMatrix);
    Vector3 invCamPos = viewMatrix.GetPositionVector();
    Vector3 camPos = rotation * -invCamPos;
    glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, (float*)&camPos);

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
  if (m_runAnim)
  {
    if (m_animPosition < 1.0)
    {
      float animDelta = msec * m_animDelta;
      m_animPosition += animDelta;
    }
    else
    {
      animStop();
    }
  }

  m_time += msec;

  for (vector<RenderObject *>::iterator i = m_renderObjects.begin(); i != m_renderObjects.end(); ++i)
  {
    (*i)->Update(msec);
  }
}

void Renderer::animStart()
{
  m_animPosition = 0.0f;
  m_runAnim = true;
}

void Renderer::animPause()
{
  if (m_animPosition < m_animDelta || m_animPosition > 1.0 - m_animDelta)
    return;

  m_runAnim = !m_runAnim;
}

void Renderer::animStop()
{
  m_runAnim = false;
}

GLuint Renderer::LoadTexture(string filename)
{
  return SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}

void Renderer::SetLighting(int i, const Vector3 &position, float radius, const Vector3 &colour)
{
  m_light[i].position = position;
  m_light[i].radius = radius;
  m_light[i].colour = colour;
}

void Renderer::ApplyShaderLight(GLuint program)
{
  glUniform3fv(glGetUniformLocation(program, "lightPos[0]"), 1, (float*)&(m_light[0].position));
  glUniform1f(glGetUniformLocation(program, "lightRadius[0]"), m_light[0].radius);
  glUniform3fv(glGetUniformLocation(program, "lightColour[0]"), 1, (float*)&(m_light[0].colour));

  glUniform3fv(glGetUniformLocation(program, "lightPos[1]"), 1, (float*)&(m_light[1].position));
  glUniform1f(glGetUniformLocation(program, "lightRadius[1]"), m_light[1].radius);
  glUniform3fv(glGetUniformLocation(program, "lightColour[1]"), 1, (float*)&(m_light[1].colour));
}