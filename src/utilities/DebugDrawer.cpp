#include <libsr2/utilities/DebugDrawer.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <math.h>

//
// Big thank you to Bullet Physics for providing the handy drawing code
// https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btIDebugDraw.h
//

#define HALF_PI 1.57079632679f
#define RADS_PER_DEG 0.0174533f
#define PI 3.141592653f
#define PI2 6.283185306f

template <class T>
void btPlaneSpace1(const T& n, T& p, T& q) {
	if (fabsf(n[2]) > 0.7071067811865475244008443621048490f) {
		// choose p in y-z plane
		sr2::f32 a = n[1] * n[1] + n[2] * n[2];
		sr2::f32 k = 1.0f / sqrtf(a);
		p[0] = 0;
		p[1] = -n[2] * k;
		p[2] = n[1] * k;
		// set q = n x p
		q[0] = a * k;
		q[1] = -n[0] * p[2];
		q[2] = n[0] * p[1];
	} else {
		// choose p in x-y plane
		sr2::f32 a = n[0] * n[0] + n[1] * n[1];
		sr2::f32 k = 1.0f / sqrtf(a);
		p[0] = -n[1] * k;
		p[1] = n[0] * k;
		p[2] = 0;
		// set q = n x p
		q[0] = -n[2] * p[1];
		q[1] = n[2] * p[0];
		q[2] = a * k;
	}
}

namespace sr2 {
    mat3x3f basis(const mat3x4f& m) {
        return { m.x, m.y, m.z };
    }

    IDebugDrawer::IDebugDrawer() {}
    IDebugDrawer::~IDebugDrawer() {}
    void IDebugDrawer::line(const vec3f& a, const vec3f& b, const vec4f& color) {

    }

    void IDebugDrawer::sphere(f32 radius, const mat3x4f& transform, const vec4f& color) {
		vec3f center = transform.w;
		vec3f up = transform.y;
		vec3f axis = transform.x;
		f32 minTh = -HALF_PI;
		f32 maxTh = HALF_PI;
		f32 minPs = -HALF_PI;
		f32 maxPs = HALF_PI;
		f32 stepDegrees = 30.f;
		spherePatch(center, up, axis, radius, minTh, maxTh, minPs, maxPs, color, stepDegrees, false);
		spherePatch(center, up, -axis, radius, minTh, maxTh, minPs, maxPs, color, stepDegrees, false);
    }

    void IDebugDrawer::sphere(f32 radius, const vec3f& center, const vec4f& color) {
		mat3x4f tr;
        math::identity(tr);
        tr.w = center;
		sphere(radius, tr, color);
    }

    void IDebugDrawer::triangle(const vec3f& a, const vec3f& b, const vec3f& c, const vec4f& color) {
		line(a, b, color);
		line(b, c, color);
		line(c, a, color);
    }

    void IDebugDrawer::aabb(const vec3f& upper, const vec3f& lower, const vec4f& color) {
		vec3f halfExtents = (upper - lower) * 0.5f;
		vec3f center = (upper + lower) * 0.5f;
		int i, j;

		vec3f edgecoord(1.f, 1.f, 1.f), pa, pb;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 3; j++)
			{
				pa = vec3f(edgecoord[0] * halfExtents[0], edgecoord[1] * halfExtents[1],
							   edgecoord[2] * halfExtents[2]);
				pa += center;

				int othercoord = j % 3;
				edgecoord[othercoord] *= -1.f;
				pb = vec3f(edgecoord[0] * halfExtents[0], edgecoord[1] * halfExtents[1],
							   edgecoord[2] * halfExtents[2]);
				pb += center;

				line(pa, pb, color);
			}
			edgecoord = vec3f(-1.f, -1.f, -1.f);
			if (i < 3)
				edgecoord[i] *= -1.f;
		}
    }

    void IDebugDrawer::transform(const mat3x4f& transform, f32 orthoLen) {
		vec3f start = transform.w;
		line(start, start + basis(transform) * vec3f(orthoLen, 0, 0), vec4f(f32(1.), f32(0.3), f32(0.3), 1.0f));
		line(start, start + basis(transform) * vec3f(0, orthoLen, 0), vec4f(f32(0.3), f32(1.), f32(0.3), 1.0f));
		line(start, start + basis(transform) * vec3f(0, 0, orthoLen), vec4f(f32(0.3), f32(0.3), f32(1.), 1.0f));
    }

    void IDebugDrawer::arc(
        const vec3f& center, const vec3f& normal, const vec3f& axis,
        f32 radiusA, f32 radiusB, f32 minAngle, f32 maxAngle,
        const vec4f& color, bool drawSect, f32 stepDegrees
    ) {
		const vec3f& vx = axis;
		vec3f vy = normal.cross(axis);
		f32 step = stepDegrees * RADS_PER_DEG;
		int nSteps = (int)fabsf((maxAngle - minAngle) / step);
		if (!nSteps) nSteps = 1;
		vec3f prev = center + radiusA * vx * cosf(minAngle) + radiusB * vy * sinf(minAngle);
		if (drawSect)
		{
			line(center, prev, color);
		}
		for (int i = 1; i <= nSteps; i++)
		{
			f32 angle = minAngle + (maxAngle - minAngle) * f32(i) / f32(nSteps);
			vec3f next = center + radiusA * vx * cosf(angle) + radiusB * vy * sinf(angle);
			line(prev, next, color);
			prev = next;
		}
		if (drawSect)
		{
			line(center, prev, color);
		}
    }

    void IDebugDrawer::spherePatch(
        const vec3f& center, const vec3f& up, const vec3f& axis,
        f32 radius, f32 minTh, f32 maxTh, f32 minPs, f32 maxPs,
        const vec4f& color, f32 stepDegrees, bool drawCenter
    ) {
		vec3f vA[74];
		vec3f vB[74];
		vec3f *pvA = vA, *pvB = vB, *pT;
		vec3f npole = center + up * radius;
		vec3f spole = center - up * radius;
		vec3f arcStart;
		f32 step = stepDegrees * RADS_PER_DEG;
		const vec3f& kv = up;
		const vec3f& iv = axis;
		vec3f jv = kv.cross(iv);
		bool drawN = false;
		bool drawS = false;
		if (minTh <= -HALF_PI)
		{
			minTh = -HALF_PI + step;
			drawN = true;
		}
		if (maxTh >= HALF_PI)
		{
			maxTh = HALF_PI - step;
			drawS = true;
		}
		if (minTh > maxTh)
		{
			minTh = -HALF_PI + step;
			maxTh = HALF_PI - step;
			drawN = drawS = true;
		}
		int n_hor = (int)((maxTh - minTh) / step) + 1;
		if (n_hor < 2) n_hor = 2;
		f32 step_h = (maxTh - minTh) / f32(n_hor - 1);
		bool isClosed = false;
		if (minPs > maxPs)
		{
			minPs = -PI + step;
			maxPs = PI;
			isClosed = true;
		}
		else if ((maxPs - minPs) >= PI * f32(2.f))
		{
			isClosed = true;
		}
		else
		{
			isClosed = false;
		}
		int n_vert = (int)((maxPs - minPs) / step) + 1;
		if (n_vert < 2) n_vert = 2;
		f32 step_v = (maxPs - minPs) / f32(n_vert - 1);
		for (int i = 0; i < n_hor; i++)
		{
			f32 th = minTh + f32(i) * step_h;
			f32 sth = radius * sinf(th);
			f32 cth = radius * cosf(th);
			for (int j = 0; j < n_vert; j++)
			{
				f32 psi = minPs + f32(j) * step_v;
				f32 sps = sinf(psi);
				f32 cps = cosf(psi);
				pvB[j] = center + cth * cps * iv + cth * sps * jv + sth * kv;
				if (i)
				{
					line(pvA[j], pvB[j], color);
				}
				else if (drawS)
				{
					line(spole, pvB[j], color);
				}
				if (j)
				{
					line(pvB[j - 1], pvB[j], color);
				}
				else
				{
					arcStart = pvB[j];
				}
				if ((i == (n_hor - 1)) && drawN)
				{
					line(npole, pvB[j], color);
				}

				if (drawCenter)
				{
					if (isClosed)
					{
						if (j == (n_vert - 1))
						{
							line(arcStart, pvB[j], color);
						}
					}
					else
					{
						if (((!i) || (i == (n_hor - 1))) && ((!j) || (j == (n_vert - 1))))
						{
							line(center, pvB[j], color);
						}
					}
				}
			}
			pT = pvA;
			pvA = pvB;
			pvB = pT;
		}
    }

    void IDebugDrawer::box(const vec3f& min, const vec3f& max, const vec4f& color) {
		line(vec3f(min.x, min.y, min.z), vec3f(max.x, min.y, min.z), color);
		line(vec3f(max.x, min.y, min.z), vec3f(max.x, max.y, min.z), color);
		line(vec3f(max.x, max.y, min.z), vec3f(min.x, max.y, min.z), color);
		line(vec3f(min.x, max.y, min.z), vec3f(min.x, min.y, min.z), color);
		line(vec3f(min.x, min.y, min.z), vec3f(min.x, min.y, max.z), color);
		line(vec3f(max.x, min.y, min.z), vec3f(max.x, min.y, max.z), color);
		line(vec3f(max.x, max.y, min.z), vec3f(max.x, max.y, max.z), color);
		line(vec3f(min.x, max.y, min.z), vec3f(min.x, max.y, max.z), color);
		line(vec3f(min.x, min.y, max.z), vec3f(max.x, min.y, max.z), color);
		line(vec3f(max.x, min.y, max.z), vec3f(max.x, max.y, max.z), color);
		line(vec3f(max.x, max.y, max.z), vec3f(min.x, max.y, max.z), color);
		line(vec3f(min.x, max.y, max.z), vec3f(min.x, min.y, max.z), color);
    }

    void IDebugDrawer::box(const vec3f& min, const vec3f& max, const mat3x4f& transform, const vec4f& color) {
		line(transform * vec3f(min.x, min.y, min.z), transform * vec3f(max.x, min.y, min.z), color);
		line(transform * vec3f(max.x, min.y, min.z), transform * vec3f(max.x, max.y, min.z), color);
		line(transform * vec3f(max.x, max.y, min.z), transform * vec3f(min.x, max.y, min.z), color);
		line(transform * vec3f(min.x, max.y, min.z), transform * vec3f(min.x, min.y, min.z), color);
		line(transform * vec3f(min.x, min.y, min.z), transform * vec3f(min.x, min.y, max.z), color);
		line(transform * vec3f(max.x, min.y, min.z), transform * vec3f(max.x, min.y, max.z), color);
		line(transform * vec3f(max.x, max.y, min.z), transform * vec3f(max.x, max.y, max.z), color);
		line(transform * vec3f(min.x, max.y, min.z), transform * vec3f(min.x, max.y, max.z), color);
		line(transform * vec3f(min.x, min.y, max.z), transform * vec3f(max.x, min.y, max.z), color);
		line(transform * vec3f(max.x, min.y, max.z), transform * vec3f(max.x, max.y, max.z), color);
		line(transform * vec3f(max.x, max.y, max.z), transform * vec3f(min.x, max.y, max.z), color);
		line(transform * vec3f(min.x, max.y, max.z), transform * vec3f(min.x, min.y, max.z), color);
    }

    void IDebugDrawer::capsule(f32 radius, f32 halfHeight, Axis upAxis, const mat3x4f& transform, const vec4f& color) {
		int stepDegrees = 30;

		vec3f capStart(0.f, 0.f, 0.f);
		capStart[upAxis] = 0.0f;

		vec3f capEnd(0.f, 0.f, 0.f);
		capEnd[upAxis] = halfHeight * 2.0f;

		// Draw the ends
		{
			mat3x4f childTransform = transform;
			childTransform.w = transform * capStart;
			{
				vec3f center = childTransform.w;
				vec3f up = basis(childTransform)[(upAxis + 1) % 3];
				vec3f axis = -basis(childTransform)[upAxis];
				f32 minTh = -HALF_PI;
				f32 maxTh = HALF_PI;
				f32 minPs = -HALF_PI;
				f32 maxPs = HALF_PI;

				spherePatch(center, up, axis, radius, minTh, maxTh, minPs, maxPs, color, f32(stepDegrees), false);
			}
		}

		{
			mat3x4f childTransform = transform;
			childTransform.w = transform * capEnd;
			{
				vec3f center = childTransform.w;
				vec3f up = basis(childTransform)[(upAxis + 1) % 3];
				vec3f axis = basis(childTransform)[upAxis];
				f32 minTh = -HALF_PI;
				f32 maxTh = HALF_PI;
				f32 minPs = -HALF_PI;
				f32 maxPs = HALF_PI;
				spherePatch(center, up, axis, radius, minTh, maxTh, minPs, maxPs, color, f32(stepDegrees), false);
			}
		}

		// Draw some additional lines
		vec3f start = transform.w;

		for (int i = 0; i < 360; i += stepDegrees)
		{
			capEnd[(upAxis + 1) % 3] = capStart[(upAxis + 1) % 3] = sinf(f32(i) * RADS_PER_DEG) * radius;
			capEnd[(upAxis + 2) % 3] = capStart[(upAxis + 2) % 3] = cosf(f32(i) * RADS_PER_DEG) * radius;
			line(transform * capStart, transform * capEnd, color);
		}
    }

    void IDebugDrawer::cylinder(f32 radius, f32 halfHeight, Axis upAxis, const mat3x4f& transform, const vec4f& color) {
		vec3f start = transform.w;
		vec3f offsetHeight(0, 0, 0);
		offsetHeight[upAxis] = halfHeight * 2.0f;
		int stepDegrees = 30;
		vec3f capStart(0.f, 0.f, 0.f);
		capStart[upAxis] = 0.0f;
		vec3f capEnd(0.f, 0.f, 0.f);
		capEnd[upAxis] = halfHeight * 2.0f;

		for (int i = 0; i < 360; i += stepDegrees)
		{
			capEnd[(upAxis + 1) % 3] = capStart[(upAxis + 1) % 3] = sinf(f32(i) * RADS_PER_DEG) * radius;
			capEnd[(upAxis + 2) % 3] = capStart[(upAxis + 2) % 3] = cosf(f32(i) * RADS_PER_DEG) * radius;
			line(start + basis(transform) * capStart, start + basis(transform) * capEnd, color);
		}
		// Drawing top and bottom caps of the cylinder
		vec3f yaxis(0, 0, 0);
		yaxis[upAxis] = f32(1.0);
		vec3f xaxis(0, 0, 0);
		xaxis[(upAxis + 1) % 3] = f32(1.0);
		arc(start - basis(transform) * (offsetHeight), basis(transform) * yaxis, basis(transform) * xaxis, radius, radius, 0, PI2, color, false, f32(10.0));
		arc(start + basis(transform) * (offsetHeight), basis(transform) * yaxis, basis(transform) * xaxis, radius, radius, 0, PI2, color, false, f32(10.0));
    }

    void IDebugDrawer::cone(f32 radius, f32 height, Axis upAxis, const mat3x4f& transform, const vec4f& color) {
		int stepDegrees = 30;
		vec3f start = transform.w;

		vec3f offsetHeight(0, 0, 0);
		f32 halfHeight = height * f32(0.5);
		offsetHeight[upAxis] = halfHeight;
		vec3f offsetRadius(0, 0, 0);
		offsetRadius[(upAxis + 1) % 3] = radius;
		vec3f offset2Radius(0, 0, 0);
		offset2Radius[(upAxis + 2) % 3] = radius;

		vec3f capEnd(0.f, 0.f, 0.f);
		capEnd[upAxis] = -halfHeight;

		for (int i = 0; i < 360; i += stepDegrees)
		{
			capEnd[(upAxis + 1) % 3] = sinf(f32(i) * RADS_PER_DEG) * radius;
			capEnd[(upAxis + 2) % 3] = cosf(f32(i) * RADS_PER_DEG) * radius;
			line(start + basis(transform) * (offsetHeight), start + basis(transform) * capEnd, color);
		}

		line(start + basis(transform) * (offsetHeight), start + basis(transform) * (-offsetHeight + offsetRadius), color);
		line(start + basis(transform) * (offsetHeight), start + basis(transform) * (-offsetHeight - offsetRadius), color);
		line(start + basis(transform) * (offsetHeight), start + basis(transform) * (-offsetHeight + offset2Radius), color);
		line(start + basis(transform) * (offsetHeight), start + basis(transform) * (-offsetHeight - offset2Radius), color);

		// Drawing the base of the cone
		vec3f yaxis(0, 0, 0);
		yaxis[upAxis] = f32(1.0);
		vec3f xaxis(0, 0, 0);
		xaxis[(upAxis + 1) % 3] = f32(1.0);
		arc(start - basis(transform) * (offsetHeight), basis(transform) * yaxis, basis(transform) * xaxis, radius, radius, 0, PI2, color, false, 10.0);
    }

    void IDebugDrawer::plane(const vec3f& normal, f32 planeConst, const mat3x4f& transform, const vec4f& color) {
		vec3f planeOrigin = normal * planeConst;
		vec3f vec0, vec1;
		btPlaneSpace1(normal, vec0, vec1);
		f32 vecLen = 100.f;
		vec3f pt0 = planeOrigin + vec0 * vecLen;
		vec3f pt1 = planeOrigin - vec0 * vecLen;
		vec3f pt2 = planeOrigin + vec1 * vecLen;
		vec3f pt3 = planeOrigin - vec1 * vecLen;
		line(transform * pt0, transform * pt1, color);
		line(transform * pt2, transform * pt3, color);
    }

    void IDebugDrawer::originGrid(i32 width, i32 length) {
        i32 hwidth = width / 2;
        i32 hlength = length / 2;

        for (f32 x = -f32(hwidth);x <= f32(hwidth);x += 1.0f) {
			if (x == 0.0f) continue;
            line(
                { x, 0, -f32(hlength) },
                { x, 0,  f32(hlength) },
                vec4f(0.5f, 0.5f, 0.5f, 1.0f)
            );
        }

        for (f32 z = -f32(hlength);z <= f32(hlength);z += 1.0f) {
			if (z == 0.0f) continue;
            line(
                { -f32(hwidth), 0, z },
                {  f32(hwidth), 0, z },
                vec4f(0.5f, 0.5f, 0.5f, 1.0f)
            );
        }

		line(
			{ -f32(hwidth), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			vec4f(0.5f, 0.0f, 0.0f, 1.0f)
		);
		line(
			{ 0.0f, 0.0f, 0.0f },
			{ f32(hwidth), 0.0f, 0.0f },
			vec4f(1.0f, 0.0f, 0.0f, 1.0f)
		);
		line(
			{ 0.0f, 0.0f, -f32(hlength) },
			{ 0.0f, 0.0f, 0.0f },
			vec4f(0.0f, 0.0f, 0.5f, 1.0f)
		);
		line(
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, f32(hlength) },
			vec4f(0.0f, 0.0f, 1.0f, 1.0f)
		);
    }




    IDebugDrawer* DebugDraw::drawer = nullptr;
    void DebugDraw::setDrawer(IDebugDrawer* drawer) {
        DebugDraw::drawer = drawer;
    }

    void DebugDraw::line(const vec3f& a, const vec3f& b, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->line(a, b, color);
    }

    void DebugDraw::sphere(f32 radius, const mat3x4f& transform, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->sphere(radius, transform, color);
    }

    void DebugDraw::sphere(f32 radius, const vec3f& center, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->sphere(radius, center, color);
    }

    void DebugDraw::triangle(const vec3f& a, const vec3f& b, const vec3f& c, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->triangle(a, b, c, color);
    }

    void DebugDraw::aabb(const vec3f& upper, const vec3f& lower, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->aabb(upper, lower, color);
    }

    void DebugDraw::transform(const mat3x4f& transform, f32 orthoLen) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->transform(transform, orthoLen);
    }

    void DebugDraw::arc(
        const vec3f& center, const vec3f& normal, const vec3f& axis,
        f32 radiusA, f32 radiusB, f32 minAngle, f32 maxAngle,
        const vec4f& color, bool drawSect, f32 stepDegrees
    ) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->arc(center, normal, axis, radiusA, radiusB, minAngle, maxAngle, color, drawSect, stepDegrees);
    }

    void DebugDraw::spherePatch(
        const vec3f& center, const vec3f& up, const vec3f& axis, f32 radius,
        f32 minTh, f32 maxTh, f32 minPs, f32 maxPs,
        const vec4f& color, f32 stepDegrees, bool drawCenter
    ) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->spherePatch(center, up, axis, radius, minTh, maxTh, minPs, maxPs, color, stepDegrees, drawCenter);
    }

    void DebugDraw::box(const vec3f& min, const vec3f& max, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->box(min, max, color);
    }

    void DebugDraw::box(const vec3f& min, const vec3f& max, const mat3x4f& transform, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->box(min, max, transform, color);
    }

    void DebugDraw::capsule(f32 radius, f32 halfHeight, Axis upAxis, const mat3x4f& transform, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->capsule(radius, halfHeight, upAxis, transform, color);
    }

    void DebugDraw::cylinder(f32 radius, f32 halfHeight, Axis upAxis, const mat3x4f& transform, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->cylinder(radius, halfHeight, upAxis, transform, color);
    }

    void DebugDraw::cone(f32 radius, f32 height, Axis upAxis, const mat3x4f& transform, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->cone(radius, height, upAxis, transform, color);
    }

    void DebugDraw::plane(const vec3f& normal, f32 planeConst, const mat3x4f& transform, const vec4f& color) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->plane(normal, planeConst, transform, color);
    }

    void DebugDraw::originGrid(i32 width, i32 length) {
        if (!DebugDraw::drawer) return;
        DebugDraw::drawer->originGrid(width, length);
    }
};