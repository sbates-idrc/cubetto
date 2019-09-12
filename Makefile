.PHONY: dist

dist:
	rm -rf dist
	mkdir -p dist
	cd AccelStepper && zip -r ../dist/AccelStepper.zip . & cd ..
	cd CubettoLibrary && zip -r ../dist/CubettoLibrary.zip . && cd ..
	cp -R PrimoCubettoOOBExample dist
