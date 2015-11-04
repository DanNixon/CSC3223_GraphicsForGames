#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

int main()
{
  SoftwareRasteriser r(800, 600);
  const float aspect = 800.0 / 600.0;

  r.SetProjectionMatrix(Matrix4::Perspective(1.0, 100.0, aspect, 45.0));

  RenderObject * o1 = new RenderObject();
  o1->mesh = Mesh::LoadMeshFile("../cube.asciimesh");
  o1->modelMatrix = Matrix4::Translation(Vector3(1.5, 0.0, -5.0));
  
  RenderObject * o2 = new RenderObject();
  o2->mesh = Mesh::GenerateTriangle();
  //o2->texture = Texture::TextureFromTGA("../brick.tga");
  o2->modelMatrix = Matrix4::Translation(Vector3(0.0, 0.0, -5.0));
  
  RenderObject * o3 = new RenderObject();
  o3->mesh = Mesh::GenerateTriangle();
  o3->modelMatrix = Matrix4::Translation(Vector3(2.0, 0.0, -10.0));

  const float diff = 0.01f;
  Matrix4 viewMatrix;
  Vector3 camTranslate = Vector3(0, 0, 0);

  while (r.UpdateWindow())
  {
    if (Keyboard::KeyDown(KEY_A))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-diff, 0.0f, 0.0f));
    if (Keyboard::KeyDown(KEY_D))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(diff, 0.0f, 0.0f));
    if (Keyboard::KeyDown(KEY_W))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, -diff, 0.0f));
    if (Keyboard::KeyDown(KEY_S))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, diff, 0.0f));

    if (Keyboard::KeyDown(KEY_UP))
      camTranslate.z += diff;
    if (Keyboard::KeyDown(KEY_DOWN))
      camTranslate.z -= diff;

    if (Keyboard::KeyTriggered(KEY_F))
    {
      r.SwitchTextureFiltering();
      std::cout << r.GetTextureSampleState() << std::endl;
    }

    if (Keyboard::KeyTriggered(KEY_E))
    {
      r.SwitchBlendState();
      std::cout << r.GetBlendState() << std::endl;
    }

    r.SetViewMatrix(viewMatrix);
    //r.SetViewMatrix(Matrix4::Translation(camTranslate));

    r.ClearBuffers();

    //r.DrawObject(o1);
    r.DrawObject(o3);
    r.DrawObject(o2);

    r.SwapBuffers();
  }

  return 0;
}
