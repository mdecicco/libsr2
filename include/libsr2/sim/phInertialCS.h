#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phPolygon;

    class phInertialCS {
        public:
            phInertialCS();
            ~phInertialCS();

            void Init(f32 mass, f32 Ixx, f32 Iyy, f32 Izz);
            void InitBoxMass(f32 mass, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z);
            void InitCylinderMass(f32 mass, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z, char axis);
            void InitHotdogMass(f32 param_1, f32 param_2, f32 param_3, char axis);
            void InitSphereMass(f32 param_1, f32 param_2);
            void InitFromGeometry(f32 mass, vec3f* vertices, phPolygon* polygons, i32 poly_count, mat3x4f* param_6);
            void InitHotdogDensity(f32 param_1, f32 param_2, f32 param_3, char axis);

            void InitMatrix();
            void Freeze();
            void ZeroForces();
            mat3x4f* AddInertia(mat3x4f* param_1, f32 param_2, vec3f* param_4, vec3f* param_5);
            mat3x4f* AddInertia(mat3x4f *param_1, f32 param_2, vec3f *param_4, mat3x4f *param_5);
            void ApplyContactForce(vec3f* param_1, vec3f* param_2, mat3x4f* param_3, vec3f* param_4);
            mat3x4f* FindPrincipalAxes(mat3x4f* param_2, f32 param_3, f32 param_4, f32 param_5);
            void GetCMFilteredVelocity(vec3f* velocity);
            void GetForce(f32 param_1, vec3f* force);
            void GetInertiaMatrix(mat3x4f* inertia);
            void GetInverseInertiaMatrix(mat3x4f* inertia);
            void GetLocalAcceleration(vec3f* param_1, vec3f* param_2);
            void GetLocalVelocity(vec3f* some_position, vec3f* velocity);
            vec3f* PrincipalAxis(f32 param_2, f32 param_3, f32 param_4, f32 param_5, vec3f* param_6);
            mat3x4f* Rejuvinate();
            void SetVelocity(vec3f* velocity);
            void TetrahedronAngInertia(vec3f* verts, f32* param_2, f32* param_3, f32* param_4, f32* param_5, f32* param_6, f32* param_7);
            
            // one of these does not belong to phInertialCS
            //void Update(f32 current_time);
            //void Update(f32 delta_time);

            // knowns
            f32 mass;
            f32 inv_mass;
            vec3f inertia_tensor;
            vec3f inv_inertia_tensor;
            f32 max_velocity;
            vec3f veolocity_related;
            mat3x4f world_transform;
            vec3f world_velocity;
            vec3f velocity_related_0; // force
            vec3f velocity_related_1; // acceleration?
            f32 maybe_last_time;
            vec3f pos_delta;

            // unknowns
            f32 field_0x0;
            undefined4 field_0x4;
            undefined4 field_0x8;
            f32 field_0x30;
            f32 field_0x34;
            f32 field_0x38;
            vec3f field_0x48;
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
            vec3f field_0x9c;
            vec3f field_0xb4;
            vec3f field_0xc0;
            vec3f field_0xcc;
            undefined4 field_0xd8;
            vec3f field_0xe8;
            vec3f field_0x100;
            vec3f field_0x10c;
            f32 field_0x118;
            f32 field_0x11c;
            f32 field_0x120;
            undefined4 field_0x124;
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