#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

int main() {
	SoftwareRasteriser r(800,600);

	RenderObject * o1 = new RenderObject();
	o1->mesh = Mesh::GenerateLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.0f));

	RenderObject * o2 = new RenderObject();
	o2->mesh = Mesh::GeneratePoint(Vector3(0.25f, 0.5f, 0.0f));

	RenderObject * o3 = new RenderObject();
	o3->mesh = Mesh::GenerateTriangle();

	while(r.UpdateWindow()) {
		r.ClearBuffers();

		r.DrawObject(o1);
		r.DrawObject(o2);
		r.DrawObject(o3);

		r.SwapBuffers();
	}

	delete o1->mesh;
	delete o1;
	delete o2->mesh;
	delete o2;
	delete o3->mesh;
	delete o3;

	return 0;
}