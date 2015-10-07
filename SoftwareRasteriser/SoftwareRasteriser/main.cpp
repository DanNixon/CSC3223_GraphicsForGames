#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

int main() {
	SoftwareRasteriser r(800,600);

	RenderObject * object = new RenderObject();
	object->mesh = Mesh::GenerateLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.0f));

	RenderObject * o2 = new RenderObject();
	o2->mesh = Mesh::GeneratePoint(Vector3(0.25f, 0.5f, 0.0f));

	while(r.UpdateWindow()) {
		r.ClearBuffers();
		r.DrawObject(object);
		r.DrawObject(o2);
		r.SwapBuffers();
	}

	delete object->mesh;
	delete object;
	return 0;
}