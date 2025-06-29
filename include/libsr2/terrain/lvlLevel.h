#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class unkInstance0;
    class CullRoomData;
    class orTerrainGrid;

    class lvlLevel {
        public:
            static lvlLevel* get();
            
            virtual void moveToRoom(unkInstance0* instance, i16 roomIdx);
            orTerrainGrid* getTerrain() const;

        protected:
            lvlLevel();
            ~lvlLevel();
            undefined2 m_unk;
            CullRoomData** m_cull_rooms;
            u32 m_room_count;
            unkInstance0* m_first_instance;
            u32 m_instance_count;
        
        private:
            static lvlLevel* instance;
    };
};