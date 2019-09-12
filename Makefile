.PHONY: dist

dist:
	rm -rf build
	rm -f Cubetto.zip
	mkdir -p build
	cd AccelStepper && zip -r ../build/AccelStepper.zip . && cd ..
	cd CubettoLibrary && zip -r ../build/CubettoLibrary.zip . && cd ..
	cp -R PrimoCubettoOOBExample build
	cd build && zip -r ../Cubetto.zip . && cd ..
