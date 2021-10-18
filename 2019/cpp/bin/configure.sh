#! /bin/bash

PROJECT_DIR=$PWD
VECTOR_URL="https://github.com/chrinkus/sxc-vector.git"
CATCH_URL="https://github.com/catchorg/Catch2.git"

mkdir -p build lib include

cd lib
echo $PWD

# C vector lib
if [ ! -d "sxc-vector" ]; then
	git clone "$VECTOR_URL"
	cd sxc-vector
	cmake -Bbuild -DCMAKE_INSTALL_PREFIX="${PROJECT_DIR}"
	cmake --build build/ --target install
	cd ..
fi

# C++ testing suite
if [ ! -d "Catch2" ]; then
	git clone "$CATCH_URL"
	cd Catch2
	cmake -Bbuild -DCMAKE_INSTALL_PREFIX="${PROJECT_DIR}" \
		-H. -DBUILD_TESTING=OFF 
	cmake --build build/ --target install
fi
