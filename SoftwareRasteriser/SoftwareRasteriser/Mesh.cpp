#include "Mesh.h"

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

Mesh *Mesh::GeneratePoint(const Vector3 &from)
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_POINTS;

  m->numVertices = 1;
  m->vertices = new Vector4[m->numVertices];

  m->vertices[0] = Vector4(from.x, from.y, from.z, 1.0f);

  return m;
}

Mesh *Mesh::GenerateLine(const Vector3 &from, const Vector3 &to)
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_LINES;

  m->numVertices = 2;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];

  m->vertices[0] = Vector4(from.x, from.y, from.z, 1.0f);
  m->vertices[1] = Vector4(to.x, to.y, to.z, 1.0f);

  m->colours[0] = Colour(255, 0, 0, 255);
  m->colours[1] = Colour(0, 0, 255, 255);

  return m;
}

Mesh *Mesh::GenerateTriangle()
{
  Mesh *m = new Mesh();
  m->type = PRIMITIVE_TRIANGLES;

  m->numVertices = 3;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];

  m->vertices[0] = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
  m->vertices[1] = Vector4(0.0f, 0.5f, 0.0f, 1.0f);
  m->vertices[2] = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);

  m->colours[0] = Colour(255, 0, 0, 255);
  m->colours[1] = Colour(0, 255, 0, 255);
  m->colours[2] = Colour(0, 0, 255, 255);

  return m;
}
