#include "Renderer.h"
#include "RenderObject.h"

#pragma comment(lib, "nclgl.lib")

void main(void)
{
  Window w = Window(800, 600);
  Renderer r(w);

  // Load the cube mesh and textures
  Mesh *cubeMesh = Mesh::LoadMeshFile("cube.asciimesh");
  GLuint cubeNormalTexture = r.LoadTexture("smiley.png");
  GLuint cubeDestroyedTexture = r.LoadTexture("noise.png");

  // Load and compile the shaders
  Shader *basicShader = new Shader("basic_vertex.glsl", "basic_fragment.glsl");
  Shader *shrinkShader = new Shader("shrink_vertex.glsl", "basic_fragment.glsl");
  Shader *texFadeShader = new Shader("basic_vertex.glsl", "texfade_fragment.glsl");
  Shader *fadeShader = new Shader("basic_vertex.glsl", "fade_fragment.glsl");

  RenderObject cube(cubeMesh, basicShader, cubeNormalTexture);
  cube.SetTexture(1, cubeDestroyedTexture);

  cube.SetModelMatrix(Matrix4::Translation(Vector3(0, 0, -10)) * Matrix4::Scale(Vector3(1, 1, 1)));
  r.AddRenderObject(cube);

  r.SetProjectionMatrix(Matrix4::Perspective(1, 100, 1.33f, 45.0f));
  r.SetViewMatrix(Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, -10)));

  // Print the list of key brindings for shader demos
  cout << endl << "Key bindings:" << endl
    << "r - Reset scene" << endl
    << "s - Shrink the cube until it disappears" << endl
    << "d - Fades form the normal texture to a destroyed texture" << endl
    << "f - Fade the cube to transaparent" << endl;

  bool disableDepthDuringAnim = false;
  while (w.UpdateWindow())
  {
    float msec = w.GetTimer()->GetTimedMS();

    // Reset scene
    if (Keyboard::KeyTriggered(KEY_R))
    {
      glEnable(GL_DEPTH_TEST);
      r.animStop();
      cube.SetShader(basicShader);
    }

    // Shrink cube
    if (Keyboard::KeyTriggered(KEY_S))
    {
      cube.SetShader(shrinkShader);
      r.animStart();
    }

    // Fade to destroyed texture
    if (Keyboard::KeyTriggered(KEY_D))
    {
      cube.SetShader(texFadeShader);
      r.animStart();
    }

    // Fade cube to transparent
    if (Keyboard::KeyTriggered(KEY_F))
    {
      disableDepthDuringAnim = true;
      cube.SetShader(fadeShader);
      r.animStart();
    }

    // Disable depth test part way through an animation, useful for fading to transparency
    if (disableDepthDuringAnim && glIsEnabled(GL_DEPTH_TEST) && r.getAnimPosition() > 0.25)
    {
      glDisable(GL_DEPTH_TEST);
      disableDepthDuringAnim = false;
    }

    // Rotate cube
    cube.SetModelMatrix(cube.GetModelMatrix() * Matrix4::Rotation(0.1f * msec, Vector3(0, 1, 1)));

    r.UpdateScene(msec);
    r.ClearBuffers();
    r.RenderScene();
    r.SwapBuffers();
  }
}
