#!/bin/bash
root=$(dirname "$0")

if [ $# -eq 0 ] ; then
    echo "Usage: $0 all -- start all server processes"
    echo "       $0 server_name -- start the specific server"
else
    if [ $1 = all ] ; then
        ./Gateway
        ./Database
    elif [ $1 = Gateway ]; then
        ./Gateway
    elif [ $1 = Database ]; then
        ./Database
    else 
        echo "Usage: $0 all -- start all server processes"
        echo "       $0 server_name -- start the specific server"
    fi
fi
