# set as all c++ files in src/
cfiles=$(find src/ -type f -name "*.cpp")
# add glut and opengl libs
incs="-I/home/komi/src/cpp/inc/"
libs="-lraylib -lGL"
objs=""

objs=""
for f in $cfiles; do
    obj="build/objs/${f##*/}.o"
    mkdir -p "$(dirname "$obj")"

    if [[ ! -f "$obj" || "$f" -nt "$obj" ]]; then
        clang++ -std=c++17 -g -w -ffast-math -Ofast -c "$f" $incs -o "$obj"
        if [ $? -ne 0 ]; then
            read -p "press enter to continue..." key
            exit 1
        fi
    fi
    objs="$objs $obj"
done

clang++ $objs $libs -o out/linux/bin/l5.64
if [ $? -ne 0 ]; then
    read -p "press enter to continue..." key
else
    echo "done"
fi