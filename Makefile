.PHONY: build

build:
	mkdir -p build
	cd AccelStepper && zip -r ../build/AccelStepper.zip . && cd ..
	cd CubettoLibrary && zip -r ../build/CubettoLibrary.zip . && cd ..
