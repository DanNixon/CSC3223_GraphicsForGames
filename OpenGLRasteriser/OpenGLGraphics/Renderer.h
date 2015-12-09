#pragma once
#include "../nclgl/OGLRenderer.h"

#include "RenderObject.h"

#include <vector>

using std::vector;

class Renderer : public OGLRenderer
{
public:
  static const int NUM_TEXTURES = 2;

  Renderer(Window &parent);
  ~Renderer(void);

  virtual void RenderScene();

  virtual void Render(const RenderObject &o);

  virtual void UpdateScene(float msec);

  void AddRenderObject(RenderObject &r)
  {
    m_renderObjects.push_back(&r);
  }

  void animStart();
  void animPause();
  void animStop();

  float getAnimPosition()
  {
    return m_animPosition;
  }

  GLuint LoadTexture(string filename);

protected:
  float m_time;
  float m_animPosition;
  float m_animDelta;
  bool m_runAnim;
  vector<RenderObject *> m_renderObjects;
};
