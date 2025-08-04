# recommended if ur in windows (tip: add msys/<compiler>/bin folder to path):
# sh build.sh b w r
# for linux and mac:
#./build.sh b l m r


(
clangflags=" -g -w -ffast-math -O3 "

build() {
	"${l5_64[@]}" #linux 5.x+ 64 bit


}



r(){
	
	platformfolder=$1
	if [[ "$platformfolder" == "" ]]; then
		clear
		echo "which platform you wanna run?
	1) windows
	2) linux
	3) mac
	"
		read -r choice
		if [[ "$choice" == "1" ]]; then
			platformfolder="windows"
		elif [[ "$choice" == "2" ]]; then
			platformfolder="linux"
		elif [[ "$choice" == "3" ]]; then
			platformfolder="mac"
		fi
	fi
	cd out/${platformfolder}/
	
	./start.sh
	cd ../..
}
sel() {
	platforms=()
	for i in "${!possibleplatforms[@]}"; do
		echo -n "use platform ${possibleplatforms[i]}? [y/n]: "
		read -r choice
		if [[ "$choice" =~ ^[Yy]$ ]]; then
			platforms+=("${platformvalues[i]}")
		fi
	done
	echo "${platforms[@]}" > platforms.txt
}





 ENGINENDPATH="/home/komi/src/cpp/libs/enginend_cpp"
 SDL2="-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer  -DSDL_MAIN_HANDLED -DSDL2"
 FB0="-DFB0"
 WIN32="32.exe -target i686-pc-windows-msvc -m32"
 WIN64="64.exe -target x86_64-pc-windows-msvc -m64"
 WXP="-D_WIN32_WINNT=0x0501 -o out/windows/bin/wxp."
 W7P="-D_WIN32_WINNT=0x0601 -o out/windows/bin/w7."
 W10P="-D_WIN32_WINNT=0x0A00 -o out/windows/bin/w1x."
 LINUX32="32 -target i686-unknown-linux-gnu -m32"
 LINUX64="64 -target x86_64-unknown-linux-gnu -m64"
 L2="-D_GLIBCXX_USE_CXX11_ABI=0 -D__USE_MISC -D__USE_XOPEN2K8 -o out/linux/bin/l2."
 L5="-o out/linux/bin/l5."
 clang="clang++"
 cfiles=$(find src -name "*.cpp")
 cfiles="$cfiles ${ENGINENDPATH}/cfiles/term.cpp"
 incs="-I${ENGINENDPATH}/inc"
 l5_64=($clang $incs $clangflags $cfiles $SDL2 $L5$LINUX64)
ran=false
built=false
runplatform=""
for i in "$@"; do
	case "$i" in
		"w")
			runplatform="windows"
			;;
		"l")
			runplatform="linux"		
			;;
		"m")
			runplatform="mac"
			;;
		"--list")
			grep -oP '#define\s+SYSTEM_\w+\s+0b[01]+' src/engine/platform/platforms.h | awk '{print $2 " = " $3}'
			;;
		"b")
			if $built; then
				echo "already built"
				exit
			fi
			build
			built=true
			;;
		"r")
			if $ran; then
				echo "already ran"
				exit
			fi
			r ${runplatform}
			ran=true
			;;
		"br")
			if $built; then
				echo "already built"
				exit
			else
				build
				built=true
			fi
			if $ran; then
				echo "already ran"
				exit
			else
				run
				ran=true
			fi
			;;
		"--help")
			echo "usage: build.sh [options]"
			echo "options:"
			echo "  -h, --help	 show this help message and exit"
			echo "  b			   build the game"
			echo "  r   		   run the game"
			echo "  br			   build and run the game"
			;;
		"platforms")
			sel
			;;
		*)
			echo "unknown option: $i"
			exit
			;;
	esac
	
done



)