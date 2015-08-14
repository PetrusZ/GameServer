#!/bin/bash
 ps -ef | grep "Gateway" | awk '{print "kill -9 " $2}' | sh
