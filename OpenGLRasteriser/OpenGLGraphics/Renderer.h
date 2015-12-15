#pragma once
#include "../nclgl/OGLRenderer.h"

#include "RenderObject.h"

#include <vector>

using std::vector;

struct Light
{
  Vector3 position;
  float radius;
  Vector3 colour;
};

class Renderer : public OGLRenderer
{
public:
  static const int NUM_TEXTURES = 5;
  static const int NUM_LIGHTS = 2;

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

  void SetLighting(int i, const Vector3 &position, float radius, const Vector3 &colour);

protected:
  void ApplyShaderLight(GLuint program);

  float m_time;
  float m_animPosition;
  float m_animDelta;
  bool m_runAnim;
  vector<RenderObject *> m_renderObjects;
  Light m_light[NUM_LIGHTS];
};
