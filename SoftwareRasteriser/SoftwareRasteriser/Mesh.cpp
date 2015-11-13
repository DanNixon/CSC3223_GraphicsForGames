#include "Mesh.h"
#include <vector>

Mesh::Mesh(void)
{
  type = PRIMITIVE_POINTS;

  numVertices = 0;

  vertices = NULL;
  colours = NULL;
  textureCoords = NULL;
}

Mesh::~Mesh(void)
{
  delete[] vertices;
  delete[] colours;
  delete[] textureCoords;
}

Mesh *Mesh::LoadMeshFile(const string &filename)
{
  ifstream f(filename);

  if (!f)
    return NULL;

  Mesh *m = new Mesh();
  m->type = PRIMITIVE_TRIANGLES;
  f >> m->numVertices;

  int hasTex = 0;
  f >> hasTex;

  int hasColour = 0;
  f >> hasColour;

  m->vertices = new Vector4[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];
  m->colours = new Colour[m->numVertices];

  for (unsigned int i = 0; i < m->numVertices; ++i)
  {
    f >> m->vertices[i].x;
    f >> m->vertices[i].y;
    f >> m->vertices[i].z;
  }

  if (hasColour)
  {
    for (unsigned int i = 0; i < m->numVertices; ++i)
    {
      f >> m->colours[i].r;
      f >> m->colours[i].g;
      f >> m->colours[i].b;
      f >> m->colours[i].a;
    }
  }

  if (hasTex)
  {
    for (unsigned int i = 0; i < m->numVertices; ++i)
    {
      f >> m->textureCoords[i].x;
      f >> m->textureCoords[i].y;
    }
  }

  return m;
}

Mesh *Mesh::GeneratePoint(const Vector3 &from, const Colour &col)
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_POINTS;

  m->numVertices = 1;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  m->vertices[0] = Vector4(from.x, from.y, from.z, 1.0f);
  m->colours[0] = Colour(col.r, col.g, col.b, col.a);
  m->textureCoords[0] = Vector2(0.0f, 0.0f);

  return m;
}

Mesh *Mesh::GenerateLine(const Vector3 &from, const Vector3 &to)
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_LINES;

  m->numVertices = 2;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  m->vertices[0] = Vector4(from.x, from.y, from.z, 1.0f);
  m->colours[0] = Colour(255, 0, 0, 255);
  m->textureCoords[0] = Vector2(0.0f, 0.0f);

  m->vertices[1] = Vector4(to.x, to.y, to.z, 1.0f);
  m->colours[1] = Colour(0, 0, 255, 255);
  m->textureCoords[1] = Vector2(1.0f, 1.0f);

  return m;
}

Mesh *Mesh::GenerateNSided2D(const int n)
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_LINES;

  m->numVertices = n * 2;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  const float p = 2 * PI / n;
  for (int i = 0; i < m->numVertices; i += 2)
  {
    const float x1 = cos(p * i);
    const float y1 = sin(p * i);

    m->vertices[i] = Vector4(x1, y1, 0.0, 1.0f);
    m->colours[i] = Colour(255, 255, 255, 255);
    m->textureCoords[i] = Vector2(x1, y1);

    const float x2 = cos(p * (i + 1));
    const float y2 = sin(p * (i + 1));

    m->vertices[i + 1] = Vector4(x2, y2, 0.0, 1.0f);
    m->colours[i + 1] = Colour(255, 255, 255, 255);
    m->textureCoords[i + 1] = Vector2(x2, y2);
  }

  return m;
}

Mesh *Mesh::GenerateTriangle()
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_TRIANGLES;

  m->numVertices = 3;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  m->vertices[0] = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
  m->vertices[1] = Vector4(0.0f, 0.5f, 0.0f, 1.0f);
  m->vertices[2] = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);

  m->colours[0] = Colour(255, 0, 0, 127);
  m->colours[1] = Colour(0, 255, 0, 127);
  m->colours[2] = Colour(0, 0, 255, 127);

  m->textureCoords[0] = Vector2(0.0f, 0.0f);
  m->textureCoords[1] = Vector2(0.5f, 1.0f);
  m->textureCoords[2] = Vector2(1.0f, 0.0f);

  return m;
}

Mesh *Mesh::GenerateTriangleStrip()
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_TRIANGLE_STRIP;

  m->numVertices = 5;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  m->vertices[0] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
  m->vertices[1] = Vector4(0.25f, 0.5f, 0.0f, 1.0f);
  m->vertices[2] = Vector4(0.5f, 0.0f, 0.0f, 1.0f);
  m->vertices[3] = Vector4(0.75f, 0.5f, 0.0f, 1.0f);
  m->vertices[4] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

  m->colours[0] = Colour(255, 0, 0, 127);
  m->colours[1] = Colour(0, 255, 0, 127);
  m->colours[2] = Colour(0, 0, 255, 127);
  m->colours[3] = Colour(255, 0, 0, 255);
  m->colours[4] = Colour(0, 255, 0, 255);

  m->textureCoords[0] = Vector2(0.0f, 0.0f);
  m->textureCoords[1] = Vector2(0.5f, 1.0f);
  m->textureCoords[2] = Vector2(1.0f, 0.0f);
  m->textureCoords[3] = Vector2(1.0f, 0.5f);
  m->textureCoords[4] = Vector2(0.5f, 0.5f);

  return m;
}

Mesh *Mesh::GenerateTriangleFan()
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_TRIANGLE_FAN;

  m->numVertices = 5;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  m->vertices[0] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
  m->vertices[1] = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
  m->vertices[2] = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
  m->vertices[3] = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
  m->vertices[4] = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);

  m->colours[0] = Colour(0, 0, 0, 127);
  m->colours[1] = Colour(255, 0, 0, 127);
  m->colours[2] = Colour(0, 255, 0, 127);
  m->colours[3] = Colour(0, 0, 255, 127);
  m->colours[4] = Colour(255, 255, 255, 127);

  m->textureCoords[0] = Vector2(0.0f, 0.0f);
  m->textureCoords[1] = Vector2(0.5f, 1.0f);
  m->textureCoords[2] = Vector2(1.0f, 0.0f);
  m->textureCoords[3] = Vector2(1.0f, 0.0f);
  m->textureCoords[4] = Vector2(1.0f, 0.0f);

  return m;
}

/**
 * Generates a 3D sphere.
 *
 * \param radius Radius (default 1.0)
 * \param resolution Number of "slices" in lon and lat
 * \param c Solid colour
 * \return New mesh
 */
Mesh *Mesh::GenerateSphere(const float radius, const int resolution, const Colour &c)
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_TRIANGLE_STRIP;

  m->numVertices = resolution * resolution * 2;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  const float deltaTheta = (PI / resolution);
  const float deltaPhi = ((PI * 2) / resolution);

  const float texEpsilon = 1.0f / resolution;

  int n = 0;
  for (int i = 0; i < resolution; i++)
  {
    const float theta1 = i * deltaTheta;
    const float theta2 = (i + 1) * deltaTheta;

    const float u1 = i * texEpsilon;
    const float u2 = (i + 1) * texEpsilon;

    for (int j = 0; j < resolution; j++)
    {
      const float phi = j * deltaPhi;

      const float v = j * texEpsilon;

      m->vertices[n] = Vector4(cos(theta1) * sin(phi) * radius, sin(theta1) * sin(phi) * radius,
                               cos(phi) * radius, 1.0f);
      m->colours[n] = c;
      m->textureCoords[n] = Vector2(u1, v);
      n++;

      m->vertices[n] = Vector4(cos(theta2) * sin(phi) * radius, sin(theta2) * sin(phi) * radius,
                               cos(phi) * radius, 1.0f);
      m->colours[n] = c;
      m->textureCoords[n] = Vector2(u2, v);
      n++;
    }
  }

  return m;
}

/**
 * Generates a 2D filled circle/disc.
 *
 * \param radius Radius (default 1.0)
 * \param resolution Number of "slices" in angle
 * \return New mesh
 */
Mesh *Mesh::GenerateDisc2D(const float radius, const int resolution)
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_TRIANGLE_FAN;

  m->numVertices = resolution + 2;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  const float deltaA = ((PI * 2) / resolution);

  // "Origin" vertex
  m->vertices[0] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
  m->colours[0] = Colour::White;
  m->textureCoords[0] = Vector2(0.5f, 0.5f);

  for (int i = 1; i < resolution + 2; ++i)
  {
    const float a = i * deltaA;

    m->vertices[i] = Vector4(cos(a) * radius, sin(a) * radius, 0.0f, 1.0f);
    m->colours[i] = Colour::White;
    Vector2 v = Vector2(abs(cos(a)), abs(sin(a)));
    m->textureCoords[i] = v;
  }

  return m;
}

/**
 * Generates a 2D ring.
 *
 * \param radiusOuter Outer radius
 * \param radiusInner Inner radius
 * \param resolution Number of "slices" in angle
 * \return New mesh
 */
Mesh *Mesh::GenerateRing2D(const float radiusOuter, const float radiusInner, const int resolution)
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_TRIANGLE_STRIP;

  m->numVertices = (resolution + 2) * 2;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];
  m->textureCoords = new Vector2[m->numVertices];

  const float deltaA = ((PI * 2) / resolution);

  int n = 0;
  for (int i = 0; i < resolution + 2; i++)
  {
    const float a = i * deltaA;

    m->vertices[n] = Vector4(cos(a) * radiusOuter, sin(a) * radiusOuter, 0.0f, 1.0f);
    m->colours[n] = Colour::White;
    m->textureCoords[n] = Vector2(abs(cos(a)), abs(sin(a)));
    n++;

    m->vertices[n] = Vector4(cos(a) * radiusInner, sin(a) * radiusInner, 0.0f, 1.0f);
    m->colours[n] = Colour::White;
    m->textureCoords[n] = Vector2(abs(cos(a)), abs(sin(a)));
    n++;
  }

  return m;
}
