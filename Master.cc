/*
 * =====================================================================================
 *
 *       Filename:  Master.cc
 *
 *    Description:  Master cpp file
 *
 *        Version:  1.0
 *        Created:  03/26/2015 01:58:20 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#include "Master.h"
#include <iostream>

Master::Master() {

}

Master::~Master() {

}

bool Master::Run(int argc, char** argv) {
    std::cout << "Hello Game Server From Master" << std::endl;
    return true;
}
