#!/bin/bash

#git status
if [[ $1 == "" ]] || [[ $2 == "" ]] || [[ $3 == "" ]]; then
	echo "Bad version"
	exit 1
fi
v="$1.$2.$3"
echo "About to release $v" 
read -p "Are you sure you are ready: " N
[ "$N" != "YES" ] && exit

sed -ri "s/SET\(CPACK_PACKAGE_VERSION_MAJOR \"[0-9]+\"\)/SET(CPACK_PACKAGE_VERSION_MAJOR \"$1\")/" CMakeLists.txt
sed -ri "s/SET\(CPACK_PACKAGE_VERSION_MINOR \"[0-9]+\"\)/SET(CPACK_PACKAGE_VERSION_MINOR \"$2\")/" CMakeLists.txt
sed -ri "s/SET\(CPACK_PACKAGE_VERSION_PATCH \"[0-9]+\"\)/SET(CPACK_PACKAGE_VERSION_PATCH \"$3\")/" CMakeLists.txt
sed -ri "s/MAJOR_VERSION=[0-9]+ MINOR_VERSION=[0-9]+ PATCH_VERSION=[0-9]+/MAJOR_VERSION=$1 MINOR_VERSION=$2 PATCH_VERSION=$3/" wkhtmltopdf.pro

./scripts/static-build.sh linux || (echo Build failed; exit 1)
if ! ./scripts/test.sh; then
	echo "Test failed"
	exit 1
fi
./scripts/static-build.sh win || (echo Build failed; exit 1)

git commit -m "Making ready for version $v" CMakeLists.txt wkhtmltopdf.pro
git tag "$v"

rm -rf release-$v
mkdir release-$v
git checkout-index --prefix=./release-$v/wkhtmltopdf-$v/ -a
tar -cjvf release-$v/wkhtmltopdf-$v.tar.bz2 -C release-$v wkhtmltopdf-$v
cp wkhtmltopdf.exe release-$v/wkhtmltopdf-$v.exe
tar -cjvf release-$v/wkhtmltopdf-$v-static.tar.bz2 wkhtmltopdf

