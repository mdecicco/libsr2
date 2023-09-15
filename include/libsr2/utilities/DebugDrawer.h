#pragma once
#include <libsr2/types.h>

namespace sr2 {
    enum Axis {
        X_AXIS = 0,
        Y_AXIS,
        Z_AXIS
    };

    class IDebugDrawable {
        public:
            virtual void debugDraw() = 0;
    };

    class IDebugDrawer {
        public:
            IDebugDrawer();
            virtual ~IDebugDrawer();

            virtual void line(const vec3f& a, const vec3f& b, const vec4f& color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void sphere(f32 radius, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void sphere(f32 radius, const vec3f& center, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void triangle(const vec3f& a, const vec3f& b, const vec3f& c, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void aabb(const vec3f& upper, const vec3f& lower, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void transform(const mat3x4f& transform, f32 orthoLen);
            virtual void arc(
                const vec3f& center, const vec3f& normal, const vec3f& axis,
                f32 radiusA, f32 radiusB, f32 minAngle, f32 maxAngle,
                const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f },
                bool drawSect = false, f32 stepDegrees = 10.0f
            );
            virtual void spherePatch(
                const vec3f& center, const vec3f& up, const vec3f& axis, f32 radius,
                f32 minTh, f32 maxTh, f32 minPs, f32 maxPs,
                const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f },
                f32 stepDegrees = 10.0f,
                bool drawCenter = true
            );
            virtual void box(const vec3f& min, const vec3f& max, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void box(const vec3f& min, const vec3f& max, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void capsule(f32 radius, f32 halfHeight, Axis upAxis, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void cylinder(f32 radius, f32 halfHeight, Axis upAxis, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void cone(f32 radius, f32 height, Axis upAxis, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void plane(const vec3f& normal, f32 planeConst, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            virtual void originGrid(i32 width, i32 length);

    };

    class DebugDraw {
        public:
            static void setDrawer(IDebugDrawer* drawer);
            static void line(const vec3f& a, const vec3f& b, const vec4f& color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void sphere(f32 radius, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void sphere(f32 radius, const vec3f& center, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void triangle(const vec3f& a, const vec3f& b, const vec3f& c, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void aabb(const vec3f& upper, const vec3f& lower, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void transform(const mat3x4f& transform, f32 orthoLen);
            static void arc(
                const vec3f& center, const vec3f& normal, const vec3f& axis,
                f32 radiusA, f32 radiusB, f32 minAngle, f32 maxAngle,
                const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f },
                bool drawSect = false,
                f32 stepDegrees = 10.0f
            );
            static void spherePatch(
                const vec3f& center, const vec3f& up, const vec3f& axis, f32 radius,
                f32 minTh, f32 maxTh, f32 minPs, f32 maxPs,
                const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f },
                f32 stepDegrees = 10.0f,
                bool drawCenter = true
            );
            static void box(const vec3f& min, const vec3f& max, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void box(const vec3f& min, const vec3f& max, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void capsule(f32 radius, f32 halfHeight, Axis upAxis, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void cylinder(f32 radius, f32 halfHeight, Axis upAxis, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void cone(f32 radius, f32 height, Axis upAxis, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void plane(const vec3f& normal, f32 planeConst, const mat3x4f& transform, const vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f });
            static void originGrid(i32 width, i32 length);

            template <typename T>
            static T* getDrawer() {
                if (!DebugDraw::drawer) return nullptr;
                return (T*)DebugDraw::drawer;
            }

        private:
            static IDebugDrawer* drawer;
    };
};