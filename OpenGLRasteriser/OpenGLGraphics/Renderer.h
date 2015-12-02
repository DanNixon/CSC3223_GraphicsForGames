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

  GLuint LoadTexture(string filename);
  GLuint LoadTexture(string filename, int idx);

protected:
  float m_time;
  vector<RenderObject *> m_renderObjects;
  GLuint m_textures[NUM_TEXTURES];
};
