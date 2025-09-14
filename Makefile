
CFILES := $(shell find src/ -type f -name "*.cpp")
INCS := -I/usr/local/include/
LIBS := -lraylib
FLAGS := -std=c++2c -g -w -ffast-math -Ofast
WINDOWSFLAGS := --target=x86_64-w64-mingw32 -mconsole ${F}
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
LSOS := $(filter-out out/linux/lib/main.cpp.so,$(foreach f,$(CFILES),out/linux/lib/$(notdir $(f)).so))

.PHONY: all linux windows android clean
all: linux windows android
linux: out/linux/bin/l5.64
linux-testing: out/linux/bin/l5-testing.64
windows: out/windows/bin/w1x.64.exe
android: out/android/bin/a5.so out/android/bin/a9.so out/android/bin/a13.so
ALL_OBJS := $(LOBJS) $(WOBJS) $(A5OBJS) $(A9OBJS) $(A13OBJS) $(LSOS)
NUM_OBJS := $(words $(ALL_OBJS))
CY := \033[33m
CP := \033[35m
CR := \033[0m
CG := \033[32m
define print_progress
 @ALL_TODO="$(MAKECMDGOALS)"; \
 if [ "$$ALL_TODO" = "all" ] || [ -z "$$ALL_TODO" ]; then \
 OBJS="$(ALL_OBJS)"; \
 elif [ "$$ALL_TODO" = "linux" ]; then \
 OBJS="$(LOBJS)"; \
 elif [ "$$ALL_TODO" = "windows" ]; then \
 OBJS="$(WOBJS)"; \
 elif [ "$$ALL_TODO" = "android" ]; then \
 OBJS="$(A5OBJS) $(A9OBJS) $(A13OBJS)"; \
 else \
 OBJS="$(ALL_OBJS)"; \
 fi; \
 COUNT=$$(echo "$$OBJS" | tr ' ' '\n' | grep -n -F "$1" | cut -d: -f1); \
 NUM_OBJS=$$(echo "$$OBJS" | tr ' ' '\n' | wc -l); \
 PERCENT=$$(( $${COUNT:-0}*100 / $$NUM_OBJS )); \
 echo "${CY}[${CP}$$PERCENT%${CY}]${CR} compiling${CG} $1${CR}"
endef

LDLIBS := $(patsubst $(LIBDIR)/lib%.so,-l%,$(wildcard $(LIBDIR)/lib*.so))
out/linux/lib/%.cpp.so:
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	@clang++ $(LIBS) -lGL -shared -fPIC $(FLAGS) $(INCS) -o $@ $<
build/objs/linux/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	@clang++ $(FLAGS) -c $< $(INCS) -o $@
build/objs/windows/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	@clang++ $(FLAGS) -c $< $(INCS) -o $@ $(WINDOWSFLAGS)
build/objs/android/5/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	@$(API21) -fPIC $(FLAGS) -c $< $(INCS) -o $@
build/objs/android/9/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	@$(API28) -fPIC $(FLAGS) -c $< $(INCS) -o $@
build/objs/android/13/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call print_progress,$@)
	@$(API33) -fPIC $(FLAGS) -c $< $(INCS) -o $@
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
out/linux/bin/l5-testing.64: $(LSOS) liblinks
	@mkdir -p $(dir $@)
	@clang++ src/main.cpp \
	-L $(LIBDIR) $(LDLIBS) \
	-o $@ $(LIBS) -lGL -Wl,-rpath,$ORIGIN/../lib
	@for so in $(LSOS); do \
		patchelf --replace-needed out/linux/lib/$$(basename $$so) lib/$$(basename $$so) $@; \
	done
clean:
	@echo "cleaning build, out/*/bin and out/*/lib"
	@rm -rf build/
	@rm -f out/*/lib/*