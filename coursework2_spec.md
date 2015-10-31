CSC3223 Coursework 2
====================

Due: 9:00am 16th December 2015

This coursework builds on the skills you have learned in graphics programming,
to construct a series of shaders incorporated into a rendered scene.

The challenge is to use OpenGL, and its associated shader language GLSL, to
build a series of graphical shaders which affect the rendering of a simple
scene in real time. The scene consists of a rotating cube.

The code for the basic scene is provided; the task is to replace the shaders.

The cube should be affected by a number of different shaders.

As time passes, the shaders should gradually destroy the cube.

You will not lose marks for using code from the tutorials, although further
marks will be gained for introducing more complex elements.

Effort should be taken to implement any such features as part of a unified
graphics pipeline.

You will also not lose marks for using textures derived from outside sources.

Remember that all such external material must be referenced and attributed
properly.

The destruction of the cube can be achieved in a number of different ways.

The more complex methods are worth more marks.

Some examples are given below, but further ideas are encouraged.

Required features:

- A vertex shader that gradually moves each vertex toward the centre of the
  object, so that it appears to shrink and vanish.
- A fragment shader that gradually blends a "clean" texture with a "destroyed"
  texture.
- A fragment shader that gradually increases the transparency of the object so
  that it fades away.

Some examples of advanced features:

- Use a geometry shader to take in the vertices of the cube, and output new
  primitives from them (lines, or triangles forming new cubes) which make the
  cube appear to fall to pieces.
- Use a tessellation shader to add more detail to each face, and then splinter
  and smash the resulting model.
- Draw a laser beam which creates a crater in the model where it hits, with
  real-time lighting (fragment shader) and deformation (tessellation shader).
- Combine some shaders to turn the model inside out and disappear into a black
  hole.

Deliverable items (submitted via NESS):

1. Executable and source code (zip file).
2. PDF document including screenshots. Two pages maximum.

Deliverable items (demonstration):

3. Demonstrate the program running in the Rack during the practical session on
   16th December.

Marks Available (25):

- Implementation of the required features gains up to 12 marks. (These features
  implemented with well-structured code guarantee 12 marks).
- Advanced rendering techniques gain up to 12 marks.
- Document gains up to 1 mark.
- The marking scheme is indicative only, and may be modified in individual
  cases if that is to your advantage.
- A first class submission entails a scene displaying different shaders in a
  cohesive way, including at least three advanced shader solutions. The code
  must be clearly formatted.

Learning Outcomes:

- Define relationship between the elements of a rendered scene and graphical
  shader technology.
- Identify appropriate shader types and methods for rendering graphical scene
  elements.
- Describe graphical representations mathematically.
- Ability to program shaders in GLSL and OpenGL.