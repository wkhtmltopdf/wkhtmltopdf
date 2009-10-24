#!/bin/bash

git status
if [[ $1 == "" ]] || [[ $2 == "" ]] || [[ $3 == "" ]]; then
	echo "Bad version"
	exit 1
fi
v="$1.$2.$3_$4"

echo "About to release $v" 
read -p "Are you sure you are ready: " N
[ "$N" != "YES" ] && exit

sed -ri "s/MAJOR_VERSION=[0-9]+ MINOR_VERSION=[0-9]+ PATCH_VERSION=[0-9]+ BUILD=\"[^#*]\"/MAJOR_VERSION=$1 MINOR_VERSION=$2 PATCH_VERSION=$3 BUILD=\"$4\"/" wkhtmltopdf.pro || exit 1

rm -rf wkhtmltopdf-i386 wkhtmltopdf-amd64 wkhtmltopdf.exe
./scripts/static-build.sh linux-i386 || (echo Build failed; exit 1)
mv wkhtmltopdf-i386 wkhtmltopdf
if ! ./scripts/test.sh; then
	echo "Test failed"
	exit 1
fi

./wkhtmltopdf-i386 --readme > README
./scripts/static-build.sh linux-amd64 || (echo Build failed; exit 1)
./scripts/static-build.sh windows || (echo Build failed; exit 1)

git commit -m "Making ready for version $v" wkhtmltopdf.pro README
git tag "$v"

rm -rf "release-$v"
mkdir "release-$v"
git checkout-index --prefix="./release-$v/wkhtmltopdf-$v/" -a
tar -cjvf "release-$v/wkhtmltopdf-$v.tar.bz2" -C "release-$v" "wkhtmltopdf-$v"
cp wkhtmltopdf.exe "release-$v/wkhtmltopdf-$v.exe"
tar -cjvf "release-$v/wkhtmltopdf-$v-static-i386.tar.bz2" wkhtmltopdf-i386
tar -cjvf "release-$v/wkhtmltopdf-$v-static-amd64.tar.bz2" wkhtmltopdf-amd64
