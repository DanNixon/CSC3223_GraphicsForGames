#include "Renderer.h"
#include "RenderObject.h"

#pragma comment(lib, "nclgl.lib")

void main(void)
{
  Window w = Window(800, 600);
  Renderer r(w);

  Mesh *m = Mesh::LoadMeshFile("cube.asciimesh");
  Shader *s = new Shader("BasicVert.glsl", "BasicFrag.glsl");

  if (s->UsingDefaultShader())
  {
    cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
    cout << "Press any key to continue." << endl;
    std::cin.get();
  }

  GLuint smiley = r.LoadTexture("smiley.png", 0);
  GLuint noise = r.LoadTexture("noise.png", 1);

  RenderObject o(m, s, smiley);

  o.SetModelMatrix(Matrix4::Translation(Vector3(0, 0, -10)) * Matrix4::Scale(Vector3(1, 1, 1)));
  r.AddRenderObject(o);

  r.SetProjectionMatrix(Matrix4::Perspective(1, 100, 1.33f, 45.0f));

  r.SetViewMatrix(Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, -10)));

  Vector4 vec[5];
  vec[0] = Vector4(1.0, 0.0, 0.0, 1.0);
  vec[1] = Vector4(0.0, 1.0, 0.0, 1.0);
  vec[2] = Vector4(0.0, 0.0, 1.0, 1.0);
  vec[3] = Vector4(0.0, 1.0, 1.0, 1.0);
  vec[4] = Vector4(1.0, 1.0, 0.0, 1.0);

  int col = 0;

  while (w.UpdateWindow())
  {
    float msec = w.GetTimer()->GetTimedMS();

    if (Keyboard::KeyTriggered(KEY_C))
    {
      col++;
      if (col > 5)
        col = 0;
    }

    GLuint program = o.GetShader()->GetShaderProgram();
    glUseProgram(program);
    glUniform4fv(glGetUniformLocation(program, "colours"), 5, (float*)&vec);
    glUniform1i(glGetUniformLocation(program, "selectedColour"), col);

    o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Rotation(0.1f * msec, Vector3(0, 1, 1)));

    r.UpdateScene(msec);
    r.ClearBuffers();
    r.RenderScene();
    r.SwapBuffers();
  }
}
