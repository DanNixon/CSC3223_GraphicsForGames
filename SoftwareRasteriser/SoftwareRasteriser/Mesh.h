/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to represent the geometric data that makes up the meshes
we render on screen.

-_-_-_-_-_-_-_,------,
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""

*/ /////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vector4.h"
#include "Colour.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Common.h"

#include <string>
#include <fstream>

using std::ifstream;
using std::string;

enum PrimitiveType
{
  PRIMITIVE_POINTS,
  PRIMITIVE_LINES,
  PRIMITIVE_TRIANGLES,
  PRIMITIVE_TRIANGLE_STRIP,
  PRIMITIVE_TRIANGLE_FAN
};

class Mesh
{
  friend class SoftwareRasteriser;

public:
  Mesh(void);
  ~Mesh(void);

  static Mesh *LoadMeshFile(const string & filename);
  static Mesh *GeneratePoint(const Vector3 &pos, const Colour &col = Colour(255, 255, 255, 255));
  static Mesh *GenerateLine(const Vector3 &from, const Vector3 &to);
  static Mesh *GenerateNSided(const int n);
  static Mesh *GenerateTriangle();
  static Mesh *GenerateTriangleStrip();
  static Mesh *GenerateTriangleFan();
  static Mesh *GenerateSphere();

  PrimitiveType GetType()
  {
    return type;
  }

protected:
  PrimitiveType type;

  uint numVertices;

  Vector4 *vertices;
  Colour *colours;
  Vector2 *textureCoords;
};
