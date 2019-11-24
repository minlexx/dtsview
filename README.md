# dtsview

## How To Build This Template

### On Unix:

cd <project_name_path>
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$KDEDIRS -DCMAKE_BUILD_TYPE=Debug ..
make
make install  or  su -c 'make install'  or  sudo make install

where $KDEDIRS points to your KDE installation prefix.

Note: you can use another build path. Then cd in your build dir and:
export KDE_SRC=path_to_your_src
cmake $KDE_SRC -DCMAKE_INSTALL_PREFIX=$KDEDIRS -DCMAKE_BUILD_TYPE=Debug
