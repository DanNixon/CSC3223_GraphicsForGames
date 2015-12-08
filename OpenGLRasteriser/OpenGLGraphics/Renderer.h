#pragma once
#include "../nclgl/OGLRenderer.h"

#include "RenderObject.h"

#include <vector>

using std::vector;

class Renderer : public OGLRenderer
{
public:
  static const int NUM_TEXTURES = 5;

  Renderer(Window &parent);
  ~Renderer(void);

  virtual void RenderScene();

  virtual void Render(const RenderObject &o);

  virtual void UpdateScene(float msec);

  void AddRenderObject(RenderObject &r)
  {
    m_renderObjects.push_back(&r);
  }

  void animStart(bool loop = false);
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
  bool m_runAnim;
  bool m_loopAnim;
  vector<RenderObject *> m_renderObjects;
};
