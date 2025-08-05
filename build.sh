# set as all c++ files in src/
cfiles=$(find src/ -type f -name "*.cpp")
# add glut and opengl libs
incs="-lglut -lGL -lGLU"

clang++ $cfiles $incs -std=c++17 -g -w -ffast-math -O3 -o out/linux/bin/l5.64
if [ $? == 1 ]; then
	read "error: press enter" key
else
	echo "done"
fi