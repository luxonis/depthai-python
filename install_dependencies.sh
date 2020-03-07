SOURCE_DIR=$(dirname "$0")

cd $SOURCE_DIR

if [ ! -d "shared/3rdparty/boost_1_71_0" ]
then
    echo "boost: installing ..."
    cd "shared/3rdparty/"
    wget https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.tar.bz2
    echo "boost: unpacking."
    tar xjf boost_1_71_0.tar.bz2
    rm boost_1_71_0.tar.bz2
    cd ../..
    echo "boost: installed."
else
    echo "boost: already installed."
fi

git submodule init
git submodule update --recursive
#git submodule sync
