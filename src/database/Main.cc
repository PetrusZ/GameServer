/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  main function of game server
 *
 *        Version:  1.0
 *        Created:  2015年03月25日 21时47分48秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */
#include "Master.h"

int
main(int argc, char *argv[])
{
    sMaster.Run(argc, argv);

    // delete Master::getSingletonPtr();

    return 0;
}
