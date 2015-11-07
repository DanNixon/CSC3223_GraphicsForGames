#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

int main()
{
  SoftwareRasteriser r(800, 600);
  const float aspect = 800.0 / 600.0;

  r.SetProjectionMatrix(Matrix4::Perspective(1.0, 100.0, aspect, 45.0));

  Mesh * testTri = Mesh::GenerateTriangle();
    
  RenderObject * o1 = new RenderObject();
  RenderObject * o2 = new RenderObject();
  RenderObject * o3 = new RenderObject();
  RenderObject * o4 = new RenderObject();
  
  o1->mesh = testTri;
  o2->mesh = testTri;
  o3->mesh = testTri;
  o4->mesh = testTri;
  
  o1->modelMatrix = Matrix4::Rotation(180.0f, Vector3(0, 1, 0));
  o2->modelMatrix = Matrix4::Translation(Vector3(-0.6, 0.6, -2));
  o3->modelMatrix = Matrix4::Translation(Vector3(0.6, -0.6, -2));
  o4->modelMatrix = Matrix4::Translation(Vector3(0.6, 0.6, -2));

  o2->texture = Texture::TextureFromTGA("../brick.tga");

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

    //r.SetViewMatrix(viewMatrix);
    r.SetViewMatrix(Matrix4::Translation(camTranslate));

    r.ClearBuffers();

    r.DrawObject(o1);
    r.DrawObject(o2);
    r.DrawObject(o3);
    r.DrawObject(o4);

    r.SwapBuffers();
  }

  return 0;
}
