#!/bin/bash
cd "${0%/*}" || exit  # Run from current directory (source directory) or exit

case "$(uname)" in
Darwin)
    NCORES=$(sysctl -n hw.ncpu)
    ;;
*)
    NCORES=$(getconf _NPROCESSORS_ONLN 2>/dev/null)
    ;;
esac
[ -n "$NCORES" ] || NCORES=4

rm -rf deploy
rm -rf build
mkdir build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release $1
(cd build && make -j)
# (cd build && make -j $NCORES)

echo
echo "Copying files into 'deploy'"
mkdir deploy

# Serial
echo
echo "... executables (serial)"
for name in \
    graphchecker \
    evaluator \
    signed_graph_clustering \
    signed_graph_clustering_evolutionary 
do
    cp ./build/"$name" deploy/
done

echo
echo "Created files in deploy/"
echo =========================
(cd deploy 2>/dev/null && ls -dF *)
echo =========================
echo
echo "Can remove old build directory"
echo

# ------------------------------------------------------------------------
