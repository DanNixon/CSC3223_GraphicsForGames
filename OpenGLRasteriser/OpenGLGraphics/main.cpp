#include "Renderer.h"
#include "RenderObject.h"

#pragma comment(lib, "nclgl.lib")

#define NUM_SHADERS 6
Shader *g_shaders[NUM_SHADERS];

void delete_shaders()
{
  for (int i = 0; i < NUM_SHADERS; i++)
  {
    if (g_shaders[i] != NULL)
      delete g_shaders[i];

    g_shaders[i] = NULL;
  }
}

void load_shaders()
{
  g_shaders[0] = new Shader("basic_vertex.glsl", "basic_fragment.glsl");
  g_shaders[1] = new Shader("shrink_vertex.glsl", "basic_fragment.glsl");
  g_shaders[2] = new Shader("basic_vertex.glsl", "texfade_fragment.glsl");
  g_shaders[3] = new Shader("basic_vertex.glsl", "fade_fragment.glsl");
  g_shaders[4] = new Shader("nomvp_vertex.glsl", "basic_fragment.glsl", "split_geometry.glsl");
  g_shaders[5] = new Shader("nomvp_vertex.glsl", "basic_fragment.glsl", "", "test_tess_ctrl.glsl", "test_tess_eval.glsl");

  for (int i = 0; i < NUM_SHADERS; i++)
  {
    if (g_shaders[i] != NULL && g_shaders[i]->UsingDefaultShader())
      cout << "Shader " << i << " failed to load or compile." << endl;
  }
}

void main(void)
{
  Window w = Window(800, 600);
  Renderer r(w);

  // Load the cube mesh and textures
  Mesh *cubeMesh = Mesh::LoadMeshFile("cube.asciimesh");
  GLuint cubeNormalTexture = r.LoadTexture("bricks.png");
  GLuint cubeDestroyedTexture = r.LoadTexture("bricks_destroyed.png");

  // Load and compile the shaders
  load_shaders();

  RenderObject cube(cubeMesh, g_shaders[0], cubeNormalTexture);
  cube.SetTexture(1, cubeDestroyedTexture);

  cube.SetModelMatrix(Matrix4::Translation(Vector3(0, 0, -10)) * Matrix4::Scale(Vector3(1, 1, 1)));
  r.AddRenderObject(cube);

  r.SetProjectionMatrix(Matrix4::Perspective(1, 100, 1.33f, 45.0f));
  r.SetViewMatrix(Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, -10)));

  // Print the list of key brindings for shader demos
  cout << endl << "Key bindings:" << endl
    << "r - Reset scene" << endl
    << "p - Pause animation" << endl
    << "P - Pause rotation" << endl
    << "0 - Reload and compile shaders" << endl
    << "s - Shrink the cube until it disappears" << endl
    << "d - Fades form the normal texture to a destroyed texture" << endl
    << "f - Fade the cube to transaparent" << endl
    << "a - Split the cube into several smaller cubes" << endl
    << "c - Detail and crack the cube" << endl;

  bool rotate = true;
  bool disableDepthDuringAnim = false;
  
  while (w.UpdateWindow())
  {
    float msec = w.GetTimer()->GetTimedMS();

    // Pause
    if (Keyboard::KeyTriggered(KEY_P))
    {
      if (Keyboard::KeyDown(KEY_SHIFT))
        rotate = !rotate;
      else
        r.animPause();
    }

    // Reload shaders
    if (Keyboard::KeyTriggered(KEY_0))
    {
      delete_shaders();
      load_shaders();
      cube.SetShader(g_shaders[0]);
    }

    // Reset scene
    if (Keyboard::KeyTriggered(KEY_R))
    {
      cubeMesh->type = GL_TRIANGLES;
      glEnable(GL_DEPTH_TEST);
      r.animStop();
      cube.SetShader(g_shaders[0]);
    }

    // Shrink cube
    if (Keyboard::KeyTriggered(KEY_S))
    {
      cube.SetShader(g_shaders[1]);
      r.animStart();
    }

    // Fade to destroyed texture
    if (Keyboard::KeyTriggered(KEY_D))
    {
      cube.SetShader(g_shaders[2]);
      r.animStart();
    }

    // Fade cube to transparent
    if (Keyboard::KeyTriggered(KEY_F))
    {
      disableDepthDuringAnim = true;
      cube.SetShader(g_shaders[3]);
      r.animStart();
    }

    // Split the cube into several smaller cubes
    if (Keyboard::KeyTriggered(KEY_A))
    {
      cube.SetShader(g_shaders[4]);
      r.animStart();
    }

    // Detail and crack the cube
    if (Keyboard::KeyTriggered(KEY_C))
    {
      cubeMesh->type = GL_PATCHES;
      cube.SetShader(g_shaders[5]);
      r.animStart();
    }

    // Disable depth test part way through an animation, useful for fading to transparency
    if (disableDepthDuringAnim && glIsEnabled(GL_DEPTH_TEST) && r.getAnimPosition() > 0.25)
    {
      glDisable(GL_DEPTH_TEST);
      disableDepthDuringAnim = false;
    }

    // Rotate cube
    if (rotate)
      cube.SetModelMatrix(cube.GetModelMatrix() * Matrix4::Rotation(0.1f * msec, Vector3(0, 1, 1)));

    r.UpdateScene(msec);
    r.ClearBuffers();
    r.RenderScene();
    r.SwapBuffers();
  }
}
