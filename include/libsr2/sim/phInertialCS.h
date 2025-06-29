#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phPolygon;

    class phInertialCS {
        public:
            phInertialCS();
            virtual ~phInertialCS();

            virtual void resetTransform();
            virtual void zero();
            virtual void freeze();
            virtual void init(f32 mass, f32 Ixx, f32 Iyy, f32 Izz);
            virtual void initFromValues(f32 mass, f32 invMass, const vec3f& angInertia, const vec3f& invAngInertia);
            virtual void update();
            virtual void updateInner(f32 dt);
            virtual void updateImplicit();
            virtual void method_0x40();
            virtual void calcNetPush(const vec3f& p1, const vec3f& p2);
            virtual void moveICS();
            virtual void getLocalVelocity(vec3f& some_position, vec3f& velocity);
            virtual void getLocalAcceleration(vec3f* param_1, vec3f* param_2);
            virtual void method_0x68();
            virtual void getInvMassMatrix2(const vec3f& unk, mat3x4f& out);

            void zeroForces();
            void initBoxMass(f32 density, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z);
            void initBoxDensity(f32 mass, f32 width, f32 height, f32 length);
            void initCylinderMass(f32 mass, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z, char axis);
            void initCylinderDensity(f32 density, f32 param_2, f32 param_3, f32 param_4, char axis);
            void initHotdogMass(f32 param_1, f32 param_2, f32 param_3, char axis);
            void initHotdogDensity(f32 param_1, f32 param_2, f32 param_3, char axis);
            void initSphereMass(f32 param_1, f32 param_2);
            void initSphereDensity(f32 density, f32 param_2);
            void initFromGeometry(f32 mass, vec3f* vertices, phPolygon* polygons, i32 poly_count, mat3x4f* param_6);

            mat3x4f* addInertia(mat3x4f* param_1, f32 param_2, vec3f* param_4, vec3f* param_5);
            mat3x4f* addInertia(mat3x4f *param_1, f32 param_2, vec3f *param_4, mat3x4f *param_5);
            void applyContactForce(const vec3f& force, const vec3f& maybeContactPoint, const mat3x4f& param_3, vec3f* param_4);
            mat3x4f* findPrincipalAxes(mat3x4f* param_2, f32 param_3, f32 param_4, f32 param_5);
            void getCMFilteredVelocity(vec3f* velocity);
            void getLocalFilteredVelocity2(vec3f& out0, vec3f& out1);
            void getForce(f32 param_1, vec3f* force);
            void getInertiaMatrix(mat3x4f* inertia);
            void getInverseInertiaMatrix(mat3x4f* inertia);
            void getInverseMassMatrix(const vec3f& unk, mat3x4f& out);
            vec3f* principalAxis(f32 param_2, f32 param_3, f32 param_4, f32 param_5, vec3f* param_6);
            mat3x4f* rejuvinate();
            void setVelocity(vec3f* velocity);
            void tetrahedronAngInertia(vec3f* verts, f32* param_2, f32* param_3, f32* param_4, f32* param_5, f32* param_6, f32* param_7);
            f32 calcCollisionNoFriction(f32 p1, const vec3f& p2, const vec3f& p3);

            // known
            bool active;
            bool pushed_last_frame;
            phInertialCS* next;
            phInertialCS* last;
            f32 mass;
            f32 inv_mass;
            f32 max_velocity;
            i32 needs_oversampling;
            f32 maybe_last_time;
            vec3f ang_inertia;
            vec3f inv_ang_inertia;
            vec3f max_ang_velocity;
            vec3f world_velocity;
            vec3f angular_velocity;
            vec3f momentum;
            vec3f angular_momentum;
            vec3f force;
            vec3f torque;
            vec3f oversampleForce;
            vec3f oversampleTorque;
            vec3f impulse;
            vec3f angular_impulse;
            vec3f push;
            vec3f turn;
            vec3f applied_push;
            vec3f last_push;
            mat3x4f world_transform;

            // unknown
            undefined field_0x54;
            undefined field_0x55;
            undefined field_0x56;
            undefined field_0x57;
            undefined field_0x58;
            undefined field_0x59;
            undefined field_0x5a;
            undefined field_0x5b;
            undefined field_0x5c;
            undefined field_0x5d;
            undefined field_0x5e;
            undefined field_0x5f;
            undefined4 field_0x128;
            undefined field_0x12c;
            undefined field_0x12d;
            undefined field_0x12e;
            undefined field_0x12f;
            mat3x4f field_0x130;
            mat3x4f field_0x160;
            mat3x4f field_0x190;
            undefined4 field_0x1c4;
            undefined field_0x1cc;
            undefined field_0x1cd;
            undefined field_0x1ce;
            undefined field_0x1cf;
    };
};