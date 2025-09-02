
# set as all c++ files in src/
cfiles=$(find src/ -type f -name "*.cpp")
# add glut and opengl libs
incs="-I/usr/local/include/"
libs="-lraylib"
flags="-std=c++17 -g -w -ffast-math -Ofast"
windowsflags="--target=x86_64-w64-mingw32 -mconsole"
wlinks="-lopengl32 -lgdi32 -lwinmm"
NDK=/home/komi/Android/Sdk/ndk/29.0.13846066
api21=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang++
api28=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android28-clang++
api33=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android33-clang++
alinks="-llog -landroid -lEGL -lGLESv2 -lc -lm -ldl -lc++_static"
lobjs=""
wobjs=""
a5objs=""
a9objs=""
a13objs=""
for f in $cfiles; do
    obj="${f##*/}.o"
    mkdir -p "$(dirname "$obj")"

    if [[ ! -f "$obj" || "$f" -nt "$obj" ]]; then
		
		echo $flags -c "$f" $incs -o build/objs/linux/"$obj"
        clang++ $flags -c "$f" $incs -o build/objs/linux/"$obj"
		echo $flags "$f" $incs -o build/objs/windows/"$obj" $windowsflags
		clang++ $flags -c "$f" $incs -o build/objs/windows/"$obj" $windowsflags
		$api21 -fPIC $flags -c "$f" $incs -o build/objs/android/5/"$obj"
		$api28 -fPIC $flags -c "$f" $incs -o build/objs/android/9/"$obj"
		$api33 -fPIC $flags -c "$f" $incs -o build/objs/android/13/"$obj"
        if [ $? -ne 0 ]; then
            read -p "press enter to continue..." key
            exit 1
        fi
    fi
	a5objs="$a5objs build/objs/android/5/$obj"
	a9objs="$a9objs build/objs/android/9/$obj"
	a13objs="$a13objs build/objs/android/13/$obj"
    lobjs="$lobjs build/objs/linux/$obj"
	wobjs="$wobjs build/objs/windows/$obj"
done


clang++ $lobjs $libs -o out/linux/bin/l5.64 -lGL
if [ $? -ne 0 ]; then
    read -p "press enter to continue..." key
fi
clang++ $wobjs $libs -o out/windows/bin/w1x.64.exe -L /usr/local/win64/lib $windowsflags $wlinks -static -static-libgcc -static-libstdc++
if [ $? -ne 0 ]; then
    read -p "press enter to continue..." key
fi
echo $a5objs  -o out/android/bin/a5.so -L /usr/local/android8/lib $alinks $libs 
$api21 $a5objs -Dmobile -o out/android/bin/a5.so -L /usr/local/android8/lib $alinks $libs -shared -fPIC 
if [ $? -ne 0 ]; then
    read -p "press enter to continue..." key
fi
echo $a9objs -o out/android/bin/a9.so -L /usr/local/android8/lib $alinks $libs
$api28 $a9objs -Dmobile -o out/android/bin/a9.so -L /usr/local/android8/lib $alinks $libs -shared -fPIC 
if [ $? -ne 0 ]; then
    read -p "press enter to continue..." key
fi
echo $a13objs -o out/android/bin/a13.so -L /usr/local/android8/lib $alinks $libs
$api33 $a13objs -Dmobile -o out/android/bin/a13.so -L /usr/local/android8/lib $alinks $libs -shared -fPIC 
if [ $? -ne 0 ]; then
    read -p "press enter to continue..." key
else
    echo "done"
fi
