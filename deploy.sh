# config (only edit these vars)

baseversion=0.03
subversion=a
target=/home/server/html/games.krystalsoft/down/endgames/forespend/classic/

# you can collapse this code block
	# folder setting

	version=$baseversion.$subversion
	version+=.g

	finalfolder=$target/$baseversion/
	mkdir $finalfolder

	win1x64=win.1x.64.$version.zip
	win1x32=win.1x.32.$version.zip
	linux564=l5.64.$version.zip

	# deployment

	rm -rf /tmp/deploy
	mkdir /tmp/deploy
	mkdir /tmp/deploy/bin
	cp out/linux/save.cfg /tmp/deploy/
	cp out/linux/res -r /tmp/deploy/

	#functions
	l564() {
		cp out/linux/bin/l5.64 /tmp/deploy/bin/game
		cp out/linux/start.sh /tmp/deploy/
		7z a $finalfolder$linux564 /tmp/deploy/*
		rm /tmp/deploy/bin/game
		rm /tmp/deploy/start.sh
	}

	w1x64() {
		cp out/windows/bin/w1x.64.exe /tmp/deploy/bin/game.exe
		cp out/windows/start.bat /tmp/deploy/
		7z a $finalfolder$win1x64 /tmp/deploy/*
		rm /tmp/deploy/bin/game.exe
		rm /tmp/deploy/start.bat
	}

# calls

l564
w1x64