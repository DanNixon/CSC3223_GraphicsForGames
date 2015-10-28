#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

int main()
{
  SoftwareRasteriser r(800, 600);
  const float aspect = 800.0 / 600.0;

  r.SetProjectionMatrix(Matrix4::Perspective(1.0, 100.0, aspect, 45.0));

  RenderObject * o1 = new RenderObject();
  o1->mesh = Mesh::LoadMeshFile("C:\\Users\\dan-n\\git_repos\\NCL_CSC3223\\SoftwareRasteriser\\cube.asciimesh");
  o1->modelMatrix = Matrix4::Translation(Vector3(1.5, 0.0, -5.0));
  
  RenderObject * o2 = new RenderObject();
  o2->mesh = Mesh::GenerateTriangle();
  o2->modelMatrix = Matrix4::Translation(Vector3(2.0, 0.0, -25.0));
  
  RenderObject * o3 = new RenderObject();
  o3->mesh = Mesh::GenerateTriangle();
  o3->modelMatrix = Matrix4::Translation(Vector3(2.0, 0.0, -50.0));

  Matrix4 viewMatrix;
  float yaw = 0.0f;

  while (r.UpdateWindow())
  {
    //yaw += Mouse::GetRelativePosition().x;
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

    r.SwapBuffers();
  }

  return 0;
}
