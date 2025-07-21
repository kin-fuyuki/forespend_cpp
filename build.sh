# recommended if ur in windows (tip: add msys/<compiler>/bin folder to path):
# sh build.sh b w r
# for linux and mac:
#./build.sh b l m r

build() {
	SDL2="-lSDL2main -lSDL2 -lSDL2_Image -lSDL2_TTF -lSDL2_Mixer  -DSDL_MAIN_HANDLED"
	#bins full list
	#BINS=("w32","nt32","nt64","nt7.32","nt7.64","nt1x.32","nt1x.64","linux5.32","linux5.64","bios","uefi")
	SYSDEFINES="-DTARGET_RENDERER=0b00000100 -DTARGET_HARDWARE=0b00000001"
	platforms=($(cat platforms.txt))
	platformnames=($(cat platformnames.txt))
	foldernames=($(cat foldernames.txt))
	sources="src/main.cpp src/game/*.cpp src/engine/*.cpp src/game/world/*.cpp"
	#iterate though both lists at once
	for i in "${!platforms[@]}"; do
		PLATFORM="${platforms[i]}"
		PLATFORMNAME="${platformnames[i]}"
		FOLDERNAME="${foldernames[i]}"
		mkdir -p out/${FOLDERNAME}/bin
		clang++.exe ${sources} -g -O0 -ffast-math -DTARGET_SYSTEM=${PLATFORM} ${SYSDEFINES} ${SDL2} -w -o out/${FOLDERNAME}/bin/${PLATFORMNAME}
	done

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
	possibleplatforms=($(grep -oP '#define\s+SYSTEM_\w+' src/engine/platform/platforms.h | awk '{print $2}'))
	platformvalues=($(grep -oP '#define\s+SYSTEM_\w+\s+0b[01]+' src/engine/platform/platforms.h | awk '{print $3}'))

	for i in "${!possibleplatforms[@]}"; do
		echo -n "use platform ${possibleplatforms[i]}? [y/n]: "
		read -r choice
		if [[ "$choice" =~ ^[Yy]$ ]]; then
			platforms+=("${platformvalues[i]}")
		fi
	done
	echo "${platforms[@]}" > platforms.txt
}
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
			echo "  -h, --help     show this help message and exit"
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

