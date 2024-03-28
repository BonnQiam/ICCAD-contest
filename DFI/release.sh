#########################################################################
# File Name: release.sh
# Author: Yibo Lin
# mail: yibolin@utexas.edu
# Created Time: Fri 08 Jan 2016 01:10:08 PM CST
#########################################################################
#!/bin/bash

./UFO \
	-in "./bench/s.gds" \
    -out "./s-out.gds" \
	-route "./bench/sM1.txt" \
	-route "./bench/sM2.txt" \
	-route "./bench/sM3.txt" \
	-score "./scores/score_s.txt" \
	-spacing 32 \
	-area 4800 \
	-log s.log \
    -minimize_gradient true 
