#!/bin/bash
BUILD="$(grep -oP 'BUILD_COUNT [^"]*"\K[^"]*' ../sources/Application/Model/Project.h)"
VERSION=1.3o_$BUILD
PACKAGE=LGPT-$VERSION.zip

collect_resources() { #1PLATFORM #2lgpt.*-exe
  PACKAGE=LGPT-$1-$VERSION.zip
  echo Packaging $PACKAGE
  CONTENTS="./resources/$1/*"
  CONTENTS+=" $(find -iname $2 -o -name README.txt)"
  zip -9 $PACKAGE -j $CONTENTS # Add intermediate step creating bin folder
  cd ./resources/packaging 
  CONTENTS="$(find -name lgpt_ALPHA -o -name samplelib)"
  zip -9 ../../$PACKAGE $CONTENTS && cd -
}

rm -f buildPSP/lgpt.elf
collect_resources PSP EBOOT.PBP
collect_resources DEB lgpt.deb-exe
collect_resources STEAM lgpt.steam-exe
collect_resources RS97 lgpt.dge
collect_resources BITTBOY lgpt.elf
collect_resources W32 lgpt.exe