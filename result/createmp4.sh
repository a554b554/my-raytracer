#########################################################################
# File Name: createmp4.sh
# Author: DT
# mail: wangliwu@zju.edu.cn
# Created Time: Wed May 20 16:33:20 2015
#########################################################################
./ffmpeg -framerate 5 -i ./translation/trans%d.png -c:v libx264 -vf "fps=5,format=yuv420p" translation.mp4
