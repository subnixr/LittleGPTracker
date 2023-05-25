cd ~/Documents/GitRepos/LittleGPTracker/
BUILD="$(grep -oP 'BUILD_COUNT [^"]*"\K[^"]*' sources/Application/Model/Project.h)"
VERSION=1.3o_$BUILD
PACKAGE=LGPT-$VERSION.zip

rm ./projects/buildPSP/lgpt.elf
mv bin/lgpt.exe bin/lgpt-win32.exe
CONTENTS="$(find -name lgpt.*exe -o -name *.elf -o -name *.dge) "
echo Moving $CONTENTS to bin:
mv $CONTENTS bin/
CONTENTS="bin/*"
echo Adding bin to $PACKAGE:
zip -9 $PACKAGE $CONTENTS
CONTENTS="$(find -name EBOOT.PBP) "
zip -9 $PACKAGE -j $CONTENTS
CONTENTS="$(find -name config.xml -o -name mapping*.xml -o -name README.txt)"
zip -9 $PACKAGE $CONTENTS
CONTENTS="lgpt_ALPHA/*"
zip -r -9 $PACKAGE $CONTENTS
mv bin/lgpt-win32.exe bin/lgpt.exe
