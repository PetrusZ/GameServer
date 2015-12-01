#!/bin/bash
root=$(dirname "$0")

if [ $# -eq 0 ] ; then
    echo "Usage: $0 all -- clear all server log and core dump files"
    echo "                                                       "
    echo "       $0 server_name -- clear the specific server log"
    echo "       $0 core_dump  -- clear all core dump files"
    echo "                                                       "
    echo "       $0 pid all -- clear all pid file"
    echo "       $0 pid server_name -- clear the specific pid file"
else
    if [ $1 = all ] ; then
        rm -rf ./Log
        rm -rf ./core*
    elif [ $1 = Gateway ]; then
        rm -rf ./Log/Gateway
    elif [ $1 = Database ]; then
        rm -rf ./Log/Database
    elif [ $1 = core_dump ]; then
        rm -rf ./core*
    elif [ $1 = pid ]; then
        if [ $2 = all ] ; then
            rm -f Gateway.pid
            rm -f Database.pid
        elif [ $2 = Gateway ]; then
            rm -f Gateway.pid
        elif [ $2 = Database ]; then
            rm -f Database.pid
        fi
    else
        echo "Usage: $0 all -- clear all server log and core dump files"
        echo "                                                       "
        echo "       $0 server_name -- clear the specific server log"
        echo "       $0 core_dump  -- clear all core dump files"
        echo "                                                       "
        echo "       $0 pid all -- clear all pid file"
        echo "       $0 pid server_name -- clear the specific pid file"
    fi
fi
