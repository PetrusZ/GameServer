/*
 * =====================================================================================
 *
 *       Filename:  World.h
 *
 *    Description:  Game World
 *
 *        Version:  1.0
 *        Created:  06/01/2015 06:26:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef WORLD_H_FKNOTEM7
#define WORLD_H_FKNOTEM7

#include <cstdint>
#include "base/Singleton.hpp"
#include "common/Common.h"

class World : public Singleton<World> {
    public:
        World() = default;
        virtual ~World() = default;

        void Update(uint64_t diff);

    private:
        /* data */
};

#define sWorld World::getSingleton()

#endif /* end of include guard: WORLD_H_FKNOTEM7 */
