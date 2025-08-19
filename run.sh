cd out/linux/
if [[ "$1" == "d" ]]; then
	cp bin/l5.64 bin/l5.64.tmp
	lldb-19 bin/l5.64.tmp
	rm bin/l5.64.tmp
else
	cp bin/l5.64 bin/l5.64.tmp
	bin/l5.64.tmp
	#read -p "Press enter to continue..." key
	rm bin/l5.64.tmp
fi
cd ../../