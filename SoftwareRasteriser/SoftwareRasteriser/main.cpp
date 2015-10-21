#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

int main()
{
  SoftwareRasteriser r(800, 600);

  RenderObject *o1 = new RenderObject();
  o1->mesh = Mesh::GenerateLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.0f));

  RenderObject *o2 = new RenderObject();
  o2->mesh = Mesh::GeneratePoint(Vector3(0.25f, 0.5f, 0.0f));

  RenderObject *o3 = new RenderObject();
  o3->mesh = Mesh::GenerateTriangle();

  Matrix4 translate = Matrix4::Translation(Vector3(0.5f, 0.0f, 0.0f));
  Matrix4 rotate = Matrix4::Rotation(45.0f, Vector3(0.0f, 0.0f, 1.0f));
  Matrix4 scale = Matrix4::Scale(Vector3(1.0f, 0.5f, 1.0f));
  o3->modelMatrix = translate * rotate * scale;

  Mesh *tri = Mesh::GenerateTriangle();

  RenderObject *t0 = new RenderObject();
  RenderObject *t1 = new RenderObject();
  RenderObject *t2 = new RenderObject();
  RenderObject *t3 = new RenderObject();

  t0->mesh = tri;
  t1->mesh = tri;
  t2->mesh = tri;
  t3->mesh = tri;

  t0->modelMatrix = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0.0f));
  t1->modelMatrix = Matrix4::Translation(Vector3(-0.5f, 0.5f, 0.0f));
  t2->modelMatrix = Matrix4::Translation(Vector3(0.5f, -0.5f, 0.0f));
  t3->modelMatrix = Matrix4::Translation(Vector3(0.5f, 0.5f, 0.0f));

  Matrix4 viewMatrix;
  float yaw = 0.0f;

  while (r.UpdateWindow())
  {
    yaw += Mouse::GetRelativePosition().x;
    viewMatrix = viewMatrix * Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f));

    if (Keyboard::KeyDown(KEY_A))
    {
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.001f, 0.0f, 0.0f));
    }
    if (Keyboard::KeyDown(KEY_D))
    {
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.001f, 0.0f, 0.0f));
    }
    if (Keyboard::KeyDown(KEY_W))
    {
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, -0.001f, 0.0f));
    }
    if (Keyboard::KeyDown(KEY_S))
    {
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.001f, 0.0f));
    }

    r.SetViewMatrix(viewMatrix);

    r.ClearBuffers();

    r.DrawObject(o1);
    r.DrawObject(o2);
    r.DrawObject(o3);

    r.DrawObject(t0);
    r.DrawObject(t1);
    r.DrawObject(t2);
    r.DrawObject(t3);

    r.SwapBuffers();
  }

  delete o1->mesh;
  delete o1;
  delete o2->mesh;
  delete o2;
  delete o3->mesh;
  delete o3;

  return 0;
}
