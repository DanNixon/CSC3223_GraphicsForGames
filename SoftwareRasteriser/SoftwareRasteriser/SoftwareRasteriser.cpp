#include "SoftwareRasteriser.h"
#include <cmath>
#include <math.h>
/*
While less 'neat' than just doing a 'new', like in the tutorials, it's usually
possible to render a bit quicker to use direct pointers to the drawing area
that the OS gives you. For a bit of a speedup, you can uncomment the define below
to switch to using this method.

For those of you new to the preprocessor, here's a quick explanation:

Preprocessor definitions like #define allow parts of a file to be selectively enabled
or disabled at compile time. This is useful for hiding parts of the codebase on a
per-platform basis: if you have support for linux and windows in your codebase, obviously
the linux platform won't have the windows platform headers available, so compilation will
fail. So instead you can hide away all the platform specific stuff:

#if PLATFORM_WINDOWS
 DoSomeWindowsStuff();
#elif PLATFORM_LINUX
 DoSomeLinuxStuff();
 #else
 #error Unsupported Platform Specified!
 #endif

 As in our usage, it also allows you to selectively compile in some different functionality
 without any 'run time' cost - if it's not enabled by the preprocessor, it won't make it to
 the compiler, so no assembly will be generated.

Also, I've implemented the Resize method for you, in a manner that'll behave itself
no matter which method you use. I kinda forgot to do that, so there was a chance you'd
get exceptions if you resized to a bigger screen area. Sorry about that.
*/
//#define USE_OS_BUFFERS

float SoftwareRasteriser::ScreenAreaOfTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2)
{
  float area = ((v0.x * v1.y) + (v1.x * v2.y) + (v2.x * v0.y)) -
               ((v1.x * v0.y) + (v2.x * v1.y) + (v0.x * v2.y));
  return area * 0.05f;
}

SoftwareRasteriser::SoftwareRasteriser(uint width, uint height)
    : Window(width, height)
{
  currentDrawBuffer = 0;

#ifndef USE_OS_BUFFERS
  // Hi! In the tutorials, it's mentioned that we need to form our front + back buffer like so:
  for (int i = 0; i < 2; ++i)
  {
    buffers[i] = new Colour[screenWidth * screenHeight];
  }
#else
  // This works, but we can actually save a memcopy by rendering directly into the memory the
  // windowing system gives us, which I've added to the Window class as the 'bufferData' pointers
  for (int i = 0; i < 2; ++i)
  {
    buffers[i] = (Colour *)bufferData[i];
  }
#endif

  depthBuffer = new unsigned short[screenWidth * screenHeight];

  float zScale = (pow(2.0f, 16) - 1) * 0.5f;

  Vector3 halfScreen = Vector3((screenWidth - 1) * 0.5f, (screenHeight - 1) * 0.5f, zScale);

  portMatrix = Matrix4::Translation(halfScreen) * Matrix4::Scale(halfScreen);
}

SoftwareRasteriser::~SoftwareRasteriser(void)
{
#ifndef USE_OS_BUFFERS
  for (int i = 0; i < 2; ++i)
  {
    delete[] buffers[i];
  }
#endif
  delete[] depthBuffer;
}

void SoftwareRasteriser::Resize()
{
  Window::Resize(); // make sure our base class gets to do anything it needs to

#ifndef USE_OS_BUFFERS
  for (int i = 0; i < 2; ++i)
  {
    delete[] buffers[i];
    buffers[i] = new Colour[screenWidth * screenHeight];
  }
#else
  for (int i = 0; i < 2; ++i)
  {
    buffers[i] = (Colour *)bufferData[i];
  }
#endif

  delete[] depthBuffer;
  depthBuffer = new unsigned short[screenWidth * screenHeight];

  float zScale = (pow(2.0f, 16) - 1) * 0.5f;

  Vector3 halfScreen = Vector3((screenWidth - 1) * 0.5f, (screenHeight - 1) * 0.5f, zScale);

  portMatrix = Matrix4::Translation(halfScreen) * Matrix4::Scale(halfScreen);
}

Colour *SoftwareRasteriser::GetCurrentBuffer()
{
  return buffers[currentDrawBuffer];
}

void SoftwareRasteriser::ClearBuffers()
{
  Colour *buffer = GetCurrentBuffer();

  unsigned int clearVal = 0xFF000000;
  unsigned int depthVal = ~0;

  for (uint y = 0; y < screenHeight; ++y)
  {
    for (uint x = 0; x < screenWidth; ++x)
    {
      buffer[(y * screenWidth) + x].c = clearVal;
      depthBuffer[(y * screenWidth) + x] = depthVal;
    }
  }
}

void SoftwareRasteriser::SwapBuffers()
{
  PresentBuffer(buffers[currentDrawBuffer]);
  currentDrawBuffer = !currentDrawBuffer;
}

void SoftwareRasteriser::DrawObject(RenderObject *o)
{
  switch (o->GetMesh()->GetType())
  {
  case PRIMITIVE_POINTS:
    RasterisePointsMesh(o);
    break;
  case PRIMITIVE_LINES:
    RasteriseLinesMesh(o);
    break;
  case PRIMITIVE_TRIANGLES:
    RasteriseTriMesh(o);
    break;
  }
}

void SoftwareRasteriser::RasteriseLine(const Vector4 &v0, const Vector4 &v1, const Colour &colA,
                                       const Colour &colB, const Vector2 &texA, const Vector2 &texB)
{
  Vector4 v0p = portMatrix * v0;
  Vector4 v1p = portMatrix * v1;
  Vector4 dir = v1p - v0p;

  int xDir = (dir.x < 0.0f) ? -1 : 1;
  int yDir = (dir.y < 0.0f) ? -1 : 1;

  int x = (int)v0p.x;
  int y = (int)v0p.y;

  int *target = NULL;
  int *scan = NULL;
  int targetVal = 0;
  int scanVal = 0;

  float slope = 0.0f;
  int range = 0;

  if (abs(dir.y) > abs(dir.x))
  {
    slope = dir.x / dir.y;
    range = (int)abs(dir.y);

    target = &x;
    scan = &y;
    targetVal = xDir;
    scanVal = yDir;
  }
  else
  {
    slope = dir.y / dir.x;
    range = (int)abs(dir.x);

    target = &y;
    scan = &x;
    targetVal = yDir;
    scanVal = xDir;
  }

  float absSlope = abs(slope);
  float error = 0.0f;

  const float reciprocalRange = 1.0f / range;

  for (int i = 0; i < range; i++)
  {
    float t = i * reciprocalRange;
    Colour c = colB * t + colA * (1.0f - t);

    ShadePixel(x, y, c);

    error += absSlope;

    if (error > 0.5f)
    {
      error -= 1.0f;
      (*target) += targetVal;
    }
    (*scan) += scanVal;
  }
}

void SoftwareRasteriser::RasteriseTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2,
                                      const Colour &c0, const Colour &c1, const Colour &c2,
                                      const Vector3 &t0, const Vector3 &t1, const Vector3 &t2)
{
  Vector4 v0p = portMatrix * v0;
  Vector4 v1p = portMatrix * v1;
  Vector4 v2p = portMatrix * v2;

  const BoundingBox b = CalculateBoxForTri(v0p, v1p, v2p);
  const float triArea = ScreenAreaOfTri(v0p, v1p, v2p);
  const float areaRecip = 1.0f / triArea;

  float subTriArea[3];
  Vector4 screenPos(0, 0, 0, 1);

  for (float y = b.topLeft.y; y < b.bottomRight.y; ++y)
  {
    for (float x = b.topLeft.x; x < b.bottomRight.x; ++x)
    {
      screenPos.x = x;
      screenPos.y = y;

      subTriArea[0] = abs(ScreenAreaOfTri(v0p, screenPos, v1p));
      subTriArea[1] = abs(ScreenAreaOfTri(v1p, screenPos, v2p));
      subTriArea[2] = abs(ScreenAreaOfTri(v2p, screenPos, v0p));

      float triSum = subTriArea[0] + subTriArea[1] + subTriArea[2];

      // Check if pixel is outside of triangle
      if (triSum > (triArea + 0.01f))
        continue;

      // Check if tringle is very small
      if (triSum < 1.0f)
        continue;

      const float alpha = subTriArea[1] * areaRecip;
      const float beta = subTriArea[2] * areaRecip;
      const float gamma = subTriArea[0] * areaRecip;

      Colour c = ((c0 * alpha) + (c1 * beta) + (c2 * gamma));

      // Pixel is in triangle, so shade it
      ShadePixel((int)x, (int)y, c);
    }
  }
}

void SoftwareRasteriser::RasteriseTriSpans(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2,
                                           const Colour &c0, const Colour &c1, const Colour &c2,
                                           const Vector3 &t0, const Vector3 &t1, const Vector3 &t2)
{
  Vector4 v0p = portMatrix * v0;
  Vector4 v1p = portMatrix * v1;
  Vector4 v2p = portMatrix * v2;

  float edge0y = abs(v0p.y - v1p.y);
  float edge1y = abs(v1p.y - v2p.y);
  float edge2y = abs(v2p.y - v0p.y);

  // Edhe 0 is longest
  if (edge0y >= edge1y && edge0y >= edge2y)
  {
    RasteriseTriEdgeSpans(v0p, v1p, v2p, v0p);
    RasteriseTriEdgeSpans(v0p, v1p, v1p, v2p);
  }
  // Edge 1 is longest
  else if (edge1y >= edge0y && edge1y >= edge2y)
  {
    RasteriseTriEdgeSpans(v1p, v2p, v2p, v0p);
    RasteriseTriEdgeSpans(v1p, v2p, v0p, v1p);
  }
  // Edge 2 is longest
  else
  {
    RasteriseTriEdgeSpans(v2p, v0p, v0p, v1p);
    RasteriseTriEdgeSpans(v2p, v0p, v1p, v2p);
  }
}

void SoftwareRasteriser::RasteriseTriEdgeSpans(const Vector4 &v0, const Vector4 &v1,
                                               const Vector4 &v2, const Vector4 &v3)
{
  Vector4 longEdge0 = v0;
  Vector4 longEdge1 = v1;
  Vector4 shortEdge0 = v2;
  Vector4 shortEdge1 = v3;

  if (longEdge1.y < longEdge0.y)
  {
    longEdge0 = v1;
    longEdge1 = v0;
  }
  if (shortEdge1.y < shortEdge0.y)
  {
    shortEdge0 = v3;
    shortEdge1 = v2;
  }

  Vector4 longDiff = longEdge1 - longEdge0;
  Vector4 shortDiff = shortEdge1 - shortEdge0;

  float longStep = longDiff.x / longDiff.y;
  float shortStep = shortDiff.x / shortDiff.y;

  float startOff = (shortEdge0.y - longEdge0.y) / longDiff.y;
  Vector4 start = longEdge0 + (longDiff * startOff);

  float endOff = (start.y - shortEdge0.y) / shortDiff.y;
  Vector4 end = shortEdge0 + (shortDiff * endOff);

  for (float y = shortEdge0.y; y < shortEdge1.y; ++y)
  {
    float minX = min(start.x, end.x);
    float maxX = max(start.x, end.x);

    for (float x = minX; x < maxX; ++x)
    {
      ShadePixel((int)x, (int)y, Colour::White);
    }

    start.x += longStep;
    end.x += shortStep;
  }
}

void SoftwareRasteriser::RasterisePointsMesh(RenderObject *o)
{
  Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
  for (uint i = 0; i < o->GetMesh()->numVertices; i++)
  {
    Vector4 vertexPos = mvp * o->GetMesh()->vertices[i];
    vertexPos.SelfDivisionByW();
    Vector4 screenPos = portMatrix * vertexPos;
    ShadePixel((uint)screenPos.x, (uint)screenPos.y, Colour::White);
  }
}

void SoftwareRasteriser::RasteriseLinesMesh(RenderObject *o)
{
  Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
  for (uint i = 0; i < o->GetMesh()->numVertices; i += 2)
  {
    Vector4 v0 = mvp * o->GetMesh()->vertices[i];
    Vector4 v1 = mvp * o->GetMesh()->vertices[i + 1];

    Colour c0 = o->GetMesh()->colours[0];
    Colour c1 = o->GetMesh()->colours[1];

    v0.SelfDivisionByW();
    v1.SelfDivisionByW();

    RasteriseLine(v0, v1, c0, c1);
  }
}

void SoftwareRasteriser::RasteriseTriMesh(RenderObject *o)
{
  Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

  for (uint i = 0; i < o->GetMesh()->numVertices; i += 3)
  {
    Vector4 v0 = mvp * o->GetMesh()->vertices[i];
    Vector4 v1 = mvp * o->GetMesh()->vertices[i + 1];
    Vector4 v2 = mvp * o->GetMesh()->vertices[i + 2];

    Colour c0 = o->GetMesh()->colours[0];
    Colour c1 = o->GetMesh()->colours[1];
    Colour c2 = o->GetMesh()->colours[2];

    v0.SelfDivisionByW();
    v1.SelfDivisionByW();
    v2.SelfDivisionByW();

    RasteriseTri(v0, v1, v2, c0, c1, c2);
    // RasteriseTriSpans(v0, v1, v2);
  }
}

BoundingBox SoftwareRasteriser::CalculateBoxForTri(const Vector4 &a, const Vector4 &b,
                                                   const Vector4 &c)
{
  BoundingBox box;

  box.topLeft.x = a.x;
  box.topLeft.x = min(box.topLeft.x, b.x);
  box.topLeft.x = min(box.topLeft.x, c.x);
  box.topLeft.x = max(box.topLeft.x, 0.0f);

  box.topLeft.y = a.y;
  box.topLeft.y = min(box.topLeft.y, b.y);
  box.topLeft.y = min(box.topLeft.y, c.y);
  box.topLeft.y = max(box.topLeft.y, 0.0f);

  box.bottomRight.x = a.x;
  box.bottomRight.x = max(box.bottomRight.x, b.x);
  box.bottomRight.x = max(box.bottomRight.x, c.x);
  box.bottomRight.x = min(box.bottomRight.x, screenWidth);

  box.bottomRight.y = a.y;
  box.bottomRight.y = max(box.bottomRight.y, b.y);
  box.bottomRight.y = max(box.bottomRight.y, c.y);
  box.bottomRight.y = min(box.bottomRight.y, screenHeight);

  return box;
}
