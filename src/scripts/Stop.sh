#!/bin/bash
 ps -ef | grep "GameServer" | awk '{print "kill -9 " $2}' | sh
