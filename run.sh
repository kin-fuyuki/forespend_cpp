cd out/linux/
if [[ "$1" == "d" ]]; then
	lldb bin/l5.64
else
	bin/l5.64
fi
cd ../../