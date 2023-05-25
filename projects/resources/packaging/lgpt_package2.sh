BASE_PATH=$PWD
BUILD="$(grep -oP 'BUILD_COUNT [^"]*"\K[^"]*' sources/Application/Model/Project.h)"
VERSION=1.3o_$BUILD
PACKAGE=LGPT-$VERSION.zip
BIN_PATH="$BASE_PATH/projects/packaging/bin/"
PCK_PATH="$BASE_PATH/projects/packaging/"

rm ./projects/buildPSP/lgpt.elf
mkdir $BIN_PATH
cd ..
# mv bin/lgpt.exe bin/lgpt-win32.exe

function collect_resources { #1config #2binary
    SUB_PACK=LGPT-$1-$VERSION.zip
    CONTENT="bin/lgpt$2 bin/config-$1.xml bin/mapping-$1.xml"
    ls -la $CONTENT
    echo Adding $CONTENT to $SUB_PACK:
    # zip -9 $SUB_PACK $CONTENT
    CONTENT="lgpt_ALPHA/* samplelib/ README.txt"
    echo Adding $CONTENT to $SUB_PACK:
    ls -la $CONTENT
    # zip -r -9 $SUB_PACK $CONTENT

    #zip
}

CONTENT="$(find -name lgpt.*exe -o -name *.elf -o -name *.dge) "
echo Moving $CONTENT to $BIN_PATH:
# mv $CONTENT $BIN_PATH
CONTENT="$(find -name EBOOT.PBP) "
# mv $CONTENT $BIN_PATH/..
cd $PCK_PATH
collect_resources chip .chip-exe
# zip -9 $PACKAGE -j $CONTENT
# CONTENT="$(find -name config.xml -o -name mapping*.xml -o -name README.txt)"
# zip -9 $PACKAGE $CONTENT
# mv bin/lgpt-win32.exe bin/lgpt.exe

#for zip in *.zip create zip

