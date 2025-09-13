./link.sh
cd out/linux/
cp bin/l5-testing.64 bin/l5.64
if [[ "$1" == "d" ]]; then
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib
	cp bin/l5.64 bin/l5.64.tmp
	lldb-19 bin/l5.64.tmp
	rm bin/l5.64.tmp
else
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib
	cp bin/l5.64 bin/l5.64.tmp
	bin/l5.64.tmp
	#read -p "Press enter to continue..." key
	rm bin/l5.64.tmp
fi
cd ../../