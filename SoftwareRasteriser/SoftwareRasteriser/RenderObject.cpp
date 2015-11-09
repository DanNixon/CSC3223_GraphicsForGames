#include "RenderObject.h"

RenderObject::RenderObject(void)
{
  texture = NULL;
  mesh = NULL;
}

RenderObject::~RenderObject(void)
{
  if (texture != NULL)
    delete texture;

  if (mesh != NULL)
    delete mesh;
}
