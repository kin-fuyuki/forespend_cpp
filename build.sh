# set as all c++ files in src/
cfiles=$(find src/ -type f -name "*.cpp")
# add glut and opengl libs
incs="-I/home/komi/src/cpp/inc/"
libs="-lraylib"

clang++ $cfiles $incs $libs -std=c++17 -g -w -ffast-math -O3 -o out/linux/bin/l5.64
if [ $? == 1 ]; then
	read -p "Press enter to continue..." key
else
	echo "done"
fi