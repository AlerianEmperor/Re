#ifndef _BBOX_H_
#define _BBOX_H_

#include "Ray.h"


static float maxf(const float& a, const float& b)
{
	return a > b ? a : b;
}

static float minf(const float& a, const float& b)
{
	return a < b ? a : b;
}


struct BBox
{
	vec3 bbox[2];
		
	BBox() { bbox[0] = { 1e10f, 1e10f, 1e10f }, bbox[1] = { -1e10f, -1e10f, -1e10f }; }
	BBox(vec3 bmin, vec3 bmax) { bbox[0] = bmin; bbox[1] = bmax; }

	/*bool __fastcall hit_axis_tl_mint(const Ray& r, float& tl, float& mint) const
	{
		float txmin = bbox[1 - r.sign[0]].x * r.invd.x + r.roinvd.x;
		float tymin = bbox[1 - r.sign[1]].y * r.invd.y + r.roinvd.y;
		float tzmin = bbox[1 - r.sign[2]].z * r.invd.z + r.roinvd.z;

		tl = maxf(txmin, maxf(tymin, tzmin));

		if (tl >= mint)
			return false;

		float txmax = bbox[r.sign[0]].x * r.invd.x + r.roinvd.x;
		float tymax = bbox[r.sign[1]].y * r.invd.y + r.roinvd.y;
		float tzmax = bbox[r.sign[2]].z * r.invd.z + r.roinvd.z;

		float th = minf(txmax, minf(tymax, tzmax));

		if (th < 0.0f)
			return false;

		return tl < th * 1.00000024f;
	}
	*/

	bool __fastcall hit_axis_tl(const Ray& r, float& tl) const
	{
		//float a = r.invd.x, b = r.invd.y, c = r.invd.z,
		//	d = r.roinvd.x, e = r.roinvd.y, f = r.roinvd.z;
		float txmax = bbox[r.sign[0]].x * r.invd.x + r.roinvd.x;
		float tymax = bbox[r.sign[1]].y * r.invd.y + r.roinvd.y;
		float tzmax = bbox[r.sign[2]].z * r.invd.z + r.roinvd.z;

		float th = minf(txmax, minf(tymax, tzmax));

		if (th < 0.0f)
			return false;

		
		//float th = minf(txmax, minf(tymax, tzmax));

		float txmin = bbox[1 - r.sign[0]].x * r.invd.x + r.roinvd.x;
		float tymin = bbox[1 - r.sign[1]].y * r.invd.y + r.roinvd.y;
		float tzmin = bbox[1 - r.sign[2]].z * r.invd.z + r.roinvd.z;

		tl = maxf(txmin, maxf(tymin, tzmin));

		return tl < 0.0f || tl <= th * 1.00000024f;
	}

	/*bool __fastcall hit_shadow(const Ray& r) const
	{	
		float txmin = bbox[1 - r.sign[0]].x * r.invd.x + r.roinvd.x;
		float txmax = bbox[r.sign[0]].x * r.invd.x + r.roinvd.x;

		float tymin = bbox[1 - r.sign[1]].y * r.invd.y + r.roinvd.y;
		float tymax = bbox[r.sign[1]].y * r.invd.y + r.roinvd.y;

		float tzmin = bbox[1 - r.sign[2]].z * r.invd.z + r.roinvd.z;
		float tzmax = bbox[r.sign[2]].z * r.invd.z + r.roinvd.z;


		float tl = maxf(txmin, maxf(tymin, tzmin));

		float th = minf(txmax, minf(tymax, tzmax));

		return th > 0.0f && tl < th * 1.00000024f;
	}*/


	bool __fastcall hit_shadow_th(const Ray& r, float& th) const
	{
		/*if (bbox[0].x <= r.o.x && r.o.x <= bbox[1].x &&
			bbox[0].y <= r.o.y && r.o.y <= bbox[1].y &&
			bbox[0].z <= r.o.z && r.o.z <= bbox[1].z)
		{
			float txmax = bbox[r.sign[0]].x * r.invd.x + r.roinvd.x;
			float tymax = bbox[r.sign[1]].y * r.invd.y + r.roinvd.y;
			float tzmax = bbox[r.sign[2]].z * r.invd.z + r.roinvd.z;

			th = minf(txmax, minf(tymax, tzmax));
			return true;
		}*/
		float txmin = bbox[1 - r.sign[0]].x * r.invd.x + r.roinvd.x;
		float txmax = bbox[r.sign[0]].x * r.invd.x + r.roinvd.x;

		float tymin = bbox[1 - r.sign[1]].y * r.invd.y + r.roinvd.y;
		float tymax = bbox[r.sign[1]].y * r.invd.y + r.roinvd.y;

		float tzmin = bbox[1 - r.sign[2]].z * r.invd.z + r.roinvd.z;
		float tzmax = bbox[r.sign[2]].z * r.invd.z + r.roinvd.z;


		float tl = maxf(txmin, maxf(tymin, tzmin));

		th = minf(txmax, minf(tymax, tzmax));

		return tl <= th * 1.00000024f;
	}


	vec3 c() const { return (bbox[0] + bbox[1]) * 0.5f; }


	void expand(const BBox& box)
	{
		bbox[0] = vec3(min(bbox[0].x, box.bbox[0].x), min(bbox[0].y, box.bbox[0].y), min(bbox[0].z, box.bbox[0].z));
		bbox[1] = vec3(max(bbox[1].x, box.bbox[1].x), max(bbox[1].y, box.bbox[1].y), max(bbox[1].z, box.bbox[1].z));
	}

	BBox expand_box(const BBox& box)
	{
		vec3 v1(min(bbox[0].x, box.bbox[0].x), min(bbox[0].y, box.bbox[0].y), min(bbox[0].z, box.bbox[0].z));
		vec3 v2(max(bbox[1].x, box.bbox[1].x), max(bbox[1].y, box.bbox[1].y), max(bbox[1].z, box.bbox[1].z));

		return BBox(v1, v2);
	}

	void expand(const vec3& p)
	{
		//bbox[0] = minvec(bbox[0], p);
		//bbox[1] = maxvec(bbox[1], p);

		bbox[0] = vec3(min(bbox[0].x, p.x), min(bbox[0].y, p.y), min(bbox[0].z, p.z));
		bbox[1] = vec3(max(bbox[1].x, p.x), max(bbox[1].y, p.y), max(bbox[1].z, p.z));
	}

	uint32_t maxDim()
	{
		vec3 extend(bbox[1] - bbox[0]);
		if (extend.x > extend.y && extend.x > extend.z) return 0;
		else if (extend.y > extend.z) return 1;
		return 2;
		
	}
	uint32_t minDim()
	{
		vec3 extend(bbox[1] - bbox[0]);
		if (extend.x < extend.y && extend.x < extend.z) return 0;
		else if (extend.y < extend.z) return 1;
		return 2;
	}

	float area()
	{
		vec3 extend(bbox[1] - bbox[0]);
		return (extend.x * extend.y + extend.y * extend.z + extend.z * extend.x);
	}

	
};

#endif // ! _BBOX_H_

