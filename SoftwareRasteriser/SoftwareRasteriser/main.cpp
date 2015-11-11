#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

void generateRandomStarfield(vector<RenderObject *> &out, const int num = 100, const float xyFact = 1.0f, const float zFact = 1.0f);
void generateRandomAsteroids(vector<RenderObject *> &out, const int num = 100, const float xyFact = 1.0f, const float zFact = 1.0f);
void generateAsteroid2D(vector<RenderObject *> &out, const Vector3 &position, const float scale);

int main()
{
  const int screenX = 800;
  const int screenY = 600;
  SoftwareRasteriser r(screenX, screenY);

  const float aspect = (float) screenX / (float) screenY;
  r.SetProjectionMatrix(Matrix4::Perspective(1.0, 100.0, aspect, 45.0));

  vector<RenderObject *> drawables;
  
  // Generate star field
  generateRandomStarfield(drawables, 10000);

  // Generate asteroids
  generateRandomAsteroids(drawables, 100);

  // Generate more asteroids closer to scene
  generateRandomAsteroids(drawables, 50, 0.5);

  //TODO
  RenderObject * o = new RenderObject();
  o->mesh = Mesh::GenerateTriangleFan();
  //o->texture = Texture::TextureFromTGA("../brick.tga");
  o->modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -2.0f));
  drawables.push_back(o);
  //END

  const float movementDelta = 0.01f;
  Matrix4 viewMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -10.0f));
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
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(movementDelta, 0.0f, 0.0f));
    if (Keyboard::KeyDown(KEY_D))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-movementDelta, 0.0f, 0.0f));

    // Handle camera/POV Z movement
    if (Keyboard::KeyDown(KEY_W))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, movementDelta));
    if (Keyboard::KeyDown(KEY_S))
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, -movementDelta));

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

/**
 * Generates a star field composed of points at random positions.
 *
 * \param out Vector to add render objects to
 * \param num Number of points to generate
 * \param xyFact Span in X and Y axis
 * \param zFact Span in Z axis
 */
void generateRandomStarfield(vector<RenderObject *> &out, const int num, const float xyFact, const float zFact)
{
  for (int i = 0; i < num; ++i)
  {
    const float x = ((float) ((rand() % 100) - 50)) * xyFact;
    const float y = ((float) ((rand() % 100) - 50)) * xyFact;
    const float z = ((float) ((rand() % 100) - 50)) * zFact;

    //TODO: random colours
    Colour c = Colour(0, 255, 255, 255);

    RenderObject * o = new RenderObject();
    o->mesh = Mesh::GeneratePoint(Vector3(), c);
    o->modelMatrix = Matrix4::Translation(Vector3(x, y, z));
    out.push_back(o);
  }
}

/**
 *Generates a random field of asteroids of random sizes.
 *
 * \param out Vector to add render objects to
 * \param num Number of asteroids to generate
 * \param xyFact Span in X and Y axis
 * \param zFact Span in Z axis
 */
void generateRandomAsteroids(vector<RenderObject *> &out, const int num, const float xyFact, const float zFact)
{
  for (int i = 0; i < num; ++i)
  {
    const float x = ((float)((rand() % 100) - 50)) * xyFact;
    const float y = ((float)((rand() % 100) - 50)) * xyFact;
    const float z = ((float)((rand() % 100) - 50)) * zFact;
    const float scale = ((float)(rand() % 100)) / 100.0f;

    //TODO: random colours/textures

    generateAsteroid2D(out, Vector3(x, y, z), scale);
  }
}

/**
 * Generates a 2D asteroid.
 *
 * \param out Vector to add render objects to
 * \param position Position to translate to
 * \param scale Scale factor
 */
void generateAsteroid2D(vector<RenderObject *> &out, const Vector3 &position, const float scale)
{
  const Matrix4 modelMat = Matrix4::Translation(position) * Matrix4::Scale(Vector3(scale, scale, scale));

  // Random rotation between 160 - 200
  const float rot = (float)((rand() % 40) + 160);

  RenderObject * o1 = new RenderObject();
  o1->mesh = Mesh::GenerateNSided(5);
  o1->modelMatrix = modelMat;

  RenderObject * o2 = new RenderObject();
  o2->mesh = Mesh::GenerateNSided(7);
  o2->modelMatrix = modelMat * Matrix4::Scale(Vector3(0.85f, 0.85f, 0.85f)) * Matrix4::Rotation(rot, Vector3(0.0f, 0.0f, 1.0f));

  out.push_back(o1);
  out.push_back(o2);
}