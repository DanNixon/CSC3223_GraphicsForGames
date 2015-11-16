/******************************************************************************
Class:SoftwareRasteriser
Implements:Window
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to encapsulate the various rasterisation techniques looked
at in the course material.

This is the class you'll be modifying the most!

-_-_-_-_-_-_-_,------,
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""

*/ /////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Matrix4.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderObject.h"
#include "Common.h"
#include "Window.h"

#include <vector>

using std::vector;

enum BlendMode
{
  BLEND_REPLACE,
  BLEND_ALPHA,
  BLEND_ADDITIVE
};

enum TextureSampleMode
{
  SAMPLE_NEAREST,
  SAMPLE_BILINEAR,
  SAMPLE_MIPMAP_NEAREST
};

struct BoundingBox
{
  Vector2 topLeft;
  Vector2 bottomRight;
};

class RenderObject;
class Texture;

class SoftwareRasteriser : public Window
{
public:
  static float ScreenAreaOfTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2);

  SoftwareRasteriser(uint width, uint height);
  ~SoftwareRasteriser(void);

  void DrawObject(RenderObject *o);

  void ClearBuffers();
  void SwapBuffers();

  void SetViewMatrix(const Matrix4 &m)
  {
    m_viewMatrix = m;
    m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
  }

  void SetProjectionMatrix(const Matrix4 &m)
  {
    m_projectionMatrix = m;
    m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
  }

  inline bool DepthFunc(int x, int y, float depthValue)
  {
    if (y < 0 || x < 0 || y >= screenHeight || x >= screenWidth)
      return false;

    int index = (y * screenWidth) + x;
    unsigned int castVal = (unsigned int)depthValue;
    if (castVal > m_depthBuffer[index])
      return false;

    m_depthBuffer[index] = castVal;
    return true;
  }

  void SetTextureSamplingMode(TextureSampleMode mode)
  {
    m_texSampleState = mode;
  }

  TextureSampleMode GetTextureSamplingMode()
  {
    return m_texSampleState;
  }

  void SetBlendMode(BlendMode mode)
  {
    m_blendState = mode;
  }

  BlendMode GetBlendMode()
  {
    return m_blendState;
  }

  bool CohenSutherlandLine(Vector4 &inA, Vector4 &inB, Colour &colA, Colour &colB, Vector3 &texA,
                           Vector3 &texB);

  void SutherlandHodgmanTri(Vector4 &v0, Vector4 &v1, Vector4 &v2, const Colour &c0 = Colour(),
                            const Colour &c1 = Colour(), const Colour &c2 = Colour(),
                            const Vector2 &t0 = Vector2(), const Vector2 &t1 = Vector2(),
                            const Vector2 &t2 = Vector2());

  float ClipEdge(const Vector4 &inA, const Vector4 &inB, int axis);
  int HomogeneousOutcode(const Vector4 &in);

protected:
  Colour *GetCurrentBuffer();

  void CalculateWeights(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2, const Vector4 &p,
                        float &alpha, float &beta, float &gamma);

  void RasterisePointsMesh(RenderObject *o);
  void RasteriseLinesMesh(RenderObject *o);
  void RasteriseTriMesh(RenderObject *o);
  void RasteriseTriMeshStrip(RenderObject *o);
  void RasteriseTriMeshFan(RenderObject *o);

  BoundingBox CalculateBoxForTri(const Vector4 &a, const Vector4 &b, const Vector4 &c);

  virtual void Resize();

  void RasteriseLine(const Vector4 &v0, const Vector4 &v1,
                     const Colour &colA = Colour(255, 255, 255, 255),
                     const Colour &colB = Colour(255, 255, 255, 255),
                     const Vector3 &texA = Vector3(0, 0, 0),
                     const Vector3 &texB = Vector3(1, 1, 1));

  void RasteriseTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2,
                    const Colour &c0 = Colour(), const Colour &c1 = Colour(),
                    const Colour &c2 = Colour(), const Vector3 &t0 = Vector3(),
                    const Vector3 &t1 = Vector3(), const Vector3 &t2 = Vector3());

  void RasteriseTriSpans(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2,
                         const Colour &c0 = Colour(), const Colour &c1 = Colour(),
                         const Colour &c2 = Colour(), const Vector3 &t0 = Vector3(),
                         const Vector3 &t1 = Vector3(), const Vector3 &t2 = Vector3());

  void RasteriseTriEdgeSpans(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2,
                             const Vector4 &v3);

  inline void ShadePixel(uint x, uint y, const Colour &c)
  {
    if (y >= screenHeight)
      return;
    if (x >= screenWidth)
      return;

    const int index = (y * screenWidth) + x;
    m_buffers[m_currentDrawBuffer][index] = c;
  }

  inline void BlendPixel(uint x, uint y, const Colour &c)
  {
    if (y >= screenHeight)
      return;
    if (x >= screenWidth)
      return;

    const int index = (y * screenWidth) + x;
    Colour &dest = m_buffers[m_currentDrawBuffer][index];

    switch (m_blendState)
    {
    case BLEND_ALPHA:
    {
      const unsigned char sFactor = c.a;
      const unsigned char dFactor = (255 - c.a);
      dest.r = ((c.r * sFactor) + (dest.r * dFactor)) / 255;
      dest.g = ((c.g * sFactor) + (dest.g * dFactor)) / 255;
      dest.b = ((c.b * sFactor) + (dest.b * dFactor)) / 255;
      dest.a = ((c.a * sFactor) + (dest.a * dFactor)) / 255;
      break;
    }
    case BLEND_ADDITIVE:
      dest = dest + c;
      break;
    default:
      dest = c;
    }
  }

  int m_currentDrawBuffer;
  Texture *m_currentTexture;

  Colour *m_buffers[2];
  unsigned short *m_depthBuffer;

  Matrix4 m_viewMatrix;
  Matrix4 m_projectionMatrix;
  Matrix4 m_textureMatrix;

  Matrix4 m_viewProjMatrix;
  Matrix4 m_portMatrix;

  TextureSampleMode m_texSampleState;
  BlendMode m_blendState;
};
