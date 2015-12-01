#!/bin/bash
root=$(dirname "$0")

if [ $# -eq 0 ] ; then
    echo "Usage: $0 all -- stop all server processes"
    echo "       $0 server_name -- stop the specific server"
else
    if [ $1 = all ] ; then
        cat ${root}/Gateway.pid | xargs kill -9
        cat ${root}/Database.pid | xargs kill -9
    elif [ $1 = Gateway ]; then
        cat ${root}/Gateway.pid | xargs kill -9
    elif [ $1 = Database ]; then
        cat ${root}/Database.pid | xargs kill -9
    else
        echo "Usage: $0 all -- stop all server processes"
        echo "       $0 server_name -- stop the specific server"
    fi
fi
