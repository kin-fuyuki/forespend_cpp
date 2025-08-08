rm -rf out/linux/res
cd res
find . -type d -exec mkdir -p -- "../out/linux/res{}" \;
find . -type f -exec ln {} "../out/linux/res{}" \;
mv ../out/linux/res. ../out/linux/res
