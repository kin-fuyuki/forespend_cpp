CFILES := $(shell find src/ -type f -name "*.cpp")
INCS := -I/usr/local/include/
LIBS := -lraylib
FLAGS := -std=c++17 -g -w -ffast-math -Ofast
WINDOWSFLAGS := --target=x86_64-w64-mingw32 -mconsole
WLINKS := -lopengl32 -lgdi32 -lwinmm
NDK := /home/komi/Android/Sdk/ndk/29.0.13846066
API21 := $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang++
API28 := $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android28-clang++
API33 := $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android33-clang++
ALINKS := -llog -landroid -lEGL -lGLESv2 -lc -lm -ldl -lc++_static

LOBJS := $(patsubst src/%.cpp,build/objs/linux/%.cpp.o,$(CFILES))
WOBJS := $(patsubst src/%.cpp,build/objs/windows/%.cpp.o,$(CFILES))
A5OBJS := $(patsubst src/%.cpp,build/objs/android/5/%.cpp.o,$(CFILES))
A9OBJS := $(patsubst src/%.cpp,build/objs/android/9/%.cpp.o,$(CFILES))
A13OBJS := $(patsubst src/%.cpp,build/objs/android/13/%.cpp.o,$(CFILES))


	
.PHONY: all linux windows android clean
all: linux windows android
linux: out/linux/bin/l5.64
windows: out/windows/bin/w1x.64.exe
android: out/android/bin/a5.so out/android/bin/a9.so out/android/bin/a13.so



build/objs/linux/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	clang++ $(FLAGS) -c $< $(INCS) -o $@

build/objs/windows/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	clang++ $(FLAGS) -c $< $(INCS) -o $@ $(WINDOWSFLAGS)

build/objs/android/5/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	$(API21) -fPIC $(FLAGS) -c $< $(INCS) -o $@

build/objs/android/9/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	$(API28) -fPIC $(FLAGS) -c $< $(INCS) -o $@

build/objs/android/13/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	$(API33) -fPIC $(FLAGS) -c $< $(INCS) -o $@

out/linux/bin/l5.64: $(LOBJS)
	@mkdir -p $(dir $@)
	@clang++ $^ $(LIBS) -o $@ -lGL

out/windows/bin/w1x.64.exe: $(WOBJS)
	@mkdir -p $(dir $@)
	@clang++ $^ $(LIBS) -o $@ -L /usr/local/win64/lib $(WINDOWSFLAGS) $(WLINKS) -static -static-libgcc -static-libstdc++

out/android/bin/a5.so: $(A5OBJS)
	@mkdir -p $(dir $@)
	@$(API21) $^ -Dmobile -o $@ -L /usr/local/android8/lib $(ALINKS) $(LIBS) -shared -fPIC

out/android/bin/a9.so: $(A9OBJS)
	@mkdir -p $(dir $@)
	@$(API28) $^ -Dmobile -o $@ -L /usr/local/android8/lib $(ALINKS) $(LIBS) -shared -fPIC

out/android/bin/a13.so: $(A13OBJS)
	@mkdir -p $(dir $@)
	@$(API33) $^ -Dmobile -o $@ -L /usr/local/android8/lib $(ALINKS) $(LIBS) -shared -fPIC

clean:
	rm -rf build/