function menu {
    clear
    echo "1) git commit n push"
	echo "2) git add"
	echo "3) quit"
}
while true
do
	menu
	read option
	case $option in
		1)
			read -p "git message " msg
			git commit -a -m "$msg"
			git push
			;;
		2)
			read -p "git add " files
			git add $files
			;;
		3)
			break
			;;
	esac
done