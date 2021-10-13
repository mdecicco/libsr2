#pragma once
#include <libsr2/types.h>
#include <libsr2/utilities/parser.h>

namespace sr2 {
    class phInertialCS;
    class phBound;
    class ModelBounds;

    // todo: A lot more has been discovered about this class and
    // classes that derive from it
    class parFileIO {
        public:
            parFileIO();
            ~parFileIO();

            virtual Parser::node* prepare_parser(Parser* p);
            virtual const char* directory();
            virtual const char* file_type();

            bool load();
            bool save();

            // knowns
            char* filename;
            phInertialCS* solver_0;
            phInertialCS* solver;
            mat3x4f world_transform;
            phBound* bound;

            // unknowns
            undefined* field_0x8;
            f32 field_0xc;
            mat3x4f* field_0x10;
            undefined* field_0x14;
            f32 field_0x18;
            f32 field_0x1c;
            f32 field_0x20;
            undefined* field_0x28;
            undefined* field_0x2c;
            ModelBounds* field_0x30;
            f32 field_0x38;
            f32 field_0x3c;
            undefined* field_0x70;
            undefined* field_0x74;
            undefined* field_0x78;
            undefined field_0x7c;
            undefined field_0x7d;
            undefined field_0x7e;
            undefined field_0x7f;
            undefined field_0x80;
            undefined field_0x81;
            undefined field_0x82;
            undefined field_0x83;
            undefined field_0x84;
            undefined field_0x85;
            undefined field_0x86;
            undefined field_0x87;
            undefined field_0x88;
            undefined field_0x89;
            undefined field_0x8a;
            undefined field_0x8b;
            undefined field_0x8c;
            undefined field_0x8d;
            undefined field_0x8e;
            undefined field_0x8f;
            undefined* field_0x90;
            undefined* field_0x94;
            undefined* field_0x98;
            undefined field_0x9c;
            undefined field_0x9d;
            undefined field_0x9e;
            undefined field_0x9f;
            undefined field_0xa4;
            undefined field_0xa5;
            undefined field_0xa6;
            undefined field_0xa7;
            undefined field_0xa8;
            undefined field_0xa9;
            undefined field_0xaa;
            undefined field_0xab;
            undefined field_0xac;
            undefined field_0xad;
            undefined field_0xae;
            undefined field_0xaf;
            undefined* field_0xb0;
            undefined* field_0xb4;
            undefined* field_0xb8;
            undefined field_0xbc;
            undefined field_0xbd;
            undefined field_0xbe;
            undefined field_0xbf;
            undefined field_0xc0;
            undefined field_0xc1;
            undefined field_0xc2;
            undefined field_0xc3;
            undefined field_0xc4;
            undefined field_0xc5;
            undefined field_0xc6;
            undefined field_0xc7;
            undefined field_0xc8;
            undefined field_0xc9;
            undefined field_0xca;
            undefined field_0xcb;
            undefined field_0xcc;
            undefined field_0xcd;
            undefined field_0xce;
            undefined field_0xcf;
            undefined* field_0xd0;
            undefined* field_0xd4;
            undefined* field_0xd8;
            undefined* field_0xdc;
            undefined* field_0xe0;
            undefined* field_0xe4;
            undefined* field_0xe8;
    };
};

