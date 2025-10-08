rm -rf out/linux/res
cd res
find . -type d -exec mkdir -p -- "../out/linux/res{}" \;
find . -type f -exec ln {} "../out/linux/res{}" \;
mv ../out/linux/res. ../out/linux/res
cd ..
# for out/windows
rm -rf out/windows/res
cd res
find . -type d -exec mkdir -p -- "../out/windows/res{}" \;
find . -type f -exec ln {} "../out/windows/res{}" \;
mv ../out/windows/res. ../out/windows/res
cd ..