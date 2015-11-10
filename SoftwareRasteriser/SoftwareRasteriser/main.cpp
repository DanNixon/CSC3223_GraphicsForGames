#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

void generateRandomStarfield(vector<RenderObject *> &out, const int num = 100, const float xyFact=10.0f , const float zFact = 20.0f);

int main()
{
  const int screenX = 800;
  const int screenY = 600;
  SoftwareRasteriser r(screenX, screenY);

  const float aspect = (float) screenX / (float) screenY;
  r.SetProjectionMatrix(Matrix4::Perspective(1.0, 100.0, aspect, 45.0));

  vector<RenderObject *> drawables;
  
  //TODO
  generateRandomStarfield(drawables, 10000, 1, 1);

  RenderObject * o = new RenderObject();
  o->mesh = Mesh::GenerateTriangle();
  o->modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -2.0f));
  drawables.push_back(o);

  RenderObject * o2 = new RenderObject();
  o2->mesh = Mesh::GenerateNSided(49);
  o2->modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -2.0f));
  drawables.push_back(o2);
  //END

  const float diff = 0.01f;
  Matrix4 viewMatrix;
  Matrix4 camRotation;

  while (r.UpdateWindow())
  {
    // Toggle texture sample mode
    if (Keyboard::KeyTriggered(KEY_F))
    {
      r.SwitchTextureFiltering();
      std::cout << "Texture sample mode: " << r.GetTextureSampleState() << std::endl;
    }

    // Toggle blend mode
    if (Keyboard::KeyTriggered(KEY_E))
    {
      r.SwitchBlendState();
      std::cout << "Blend mode: " << r.GetBlendState() << std::endl;
    }

    // Handle strafe movement
    if (Keyboard::KeyDown(KEY_A))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(diff, 0.0f, 0.0f));
    if (Keyboard::KeyDown(KEY_D))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-diff, 0.0f, 0.0f));

    // Handle camera/POV Z movement
    if (Keyboard::KeyDown(KEY_W))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, diff));
    if (Keyboard::KeyDown(KEY_S))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, -diff));

    // Handle camera/POV rotation
    const Vector2 mouseRelPos = Mouse::GetRelativePosition();
    camRotation = camRotation
                * Matrix4::Rotation(mouseRelPos.x, Vector3(0.0f, 1.0f, 0.0f))
                * Matrix4::Rotation(mouseRelPos.y, Vector3(1.0f, 0.0f, 0.0f));

    r.SetViewMatrix(viewMatrix * camRotation);

    r.ClearBuffers();

    // Draw scene objects
    for (vector<RenderObject *>::iterator it = drawables.begin(); it != drawables.end(); ++it)
      r.DrawObject(*it);

    r.SwapBuffers();
  }

  // Remove all drawables, memory is freed in the RenderObject destructor
  drawables.clear();
  
  return 0;
}

void generateRandomStarfield(vector<RenderObject *> &out, const int num, const float xyFact, const float zFact)
{
  for (int i = 0; i < num; ++i)
  {
    const float x = ((float) ((rand() % 100) - 50)) / xyFact;
    const float y = ((float) ((rand() % 100) - 50)) / xyFact;
    const float z = ((float) ((rand() % 100) - 50)) / zFact;

    RenderObject * o = new RenderObject;
    o->mesh = Mesh::GeneratePoint(Vector3());
    o->modelMatrix = Matrix4::Translation(Vector3(x, y, z));
    out.push_back(o);
  }
}