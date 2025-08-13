cd out/linux/
if [[ "$1" == "d" ]]; then
	lldb bin/l5.64
else
	cp bin/l5.64 bin/l5.64.tmp
	bin/l5.64.tmp
	rm bin/l5.64.tmp
fi
cd ../../