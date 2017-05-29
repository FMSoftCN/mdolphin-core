#!/bin/bash

PICTURE_DIR=pictures
DAT_DIR=dat_files
C_DIR=c_files

#RC4KEY=`echo -n $RANDOM | md5sum | awk '{print $1}'`
#echo $RC4KEY


function genCppFile() 
{
#
# dat -> c
#

    rm -rf $C_DIR/$1
    #subdir
    subdir=$C_DIR/$1
    echo  $subdir

    mkdir -p $subdir > /dev/null 2>&1
    

    # key.c
    CRC4KEY=`echo -n $TMPRC4KEY | sed "s/ /,/g"`
    echo $CRC4KEY
    echo "MD_LOCAL unsigned char $1_crypto_key[16] = {" > $subdir/$1_key.c
    echo "    "$CRC4KEY >> $subdir/$1_key.c
    echo "};" >> $subdir/$1_key.c
    echo "" >> $subdir/$1_key.c

    cd $DAT_DIR/$1; find . -name \*.dat | sed "s/^\.\/*//g" | sort > ../../res.lst; 
    
    cd ../..
    cat res.lst

    #echo "*********************************************" $PWD  $DAT_DIR/$1  $C_DIR/$1 
    #convert Lower case to Captal
    macro=`tr  '[a-z]' '[A-Z]' <<<"$1"`
    macro=_MD_ENABLE_$macro
    echo $macro

    ./inner-res-trans -m $macro -i $DAT_DIR/$1 -o $C_DIR/$1 -p __md_$1 -n _$1_inner_res -l res.lst


    find $C_DIR/$1 -name \*.dat.c -exec sed 's/unsigned/MD_LOCAL unsigned/' -i {} \;
    find $C_DIR/$1 -name \*.dat.c -exec sed '/common.h/d' -i {} \;
    find $C_DIR/$1 -name _$1_inner_res.c -exec sed 's/(void\*)/(Uint8\*)/' -i {} \;
    find $C_DIR/$1 -name _$1_inner_res.c -exec sed 's/\"dat\"/(void*)&/' -i {} \;
    echo $(pwd)
    rm -f res.lst
}

#
# picture -> dat
#
mkdir $C_DIR > /dev/null 2>&1
mkdir $DAT_DIR > /dev/null 2>&1
cd $PICTURE_DIR

#LIST=`ls *.png`
#
#for src in $LIST; do
#    dst=`echo $src | sed 's/png$/dat/'`
#    dst=../$DAT_DIR/$dst
#
#    # TODO: encrypt files
#    echo "$src --> $dst"
#    echo `pwd`
#    ../cryptool -e $src $dst $RC4KEY
#done

#rescurive directroy
for subdir in $(ls -F | grep "\/$" | sed 's/\/$//g')
do
    echo "*********************************************" $PWD  $subdir 
    mkdir -p ../$DAT_DIR/$subdir > /dev/null 2>&1
    LIST=`ls $subdir/*.png`
    
    RC4KEY=`awk 'BEGIN {srand();for (i=1;i<=16;i++) printf("%02x",int(256*rand()))}'`
    echo $RC4KEY

    TMPRC4KEY=`echo -n $RC4KEY | awk '{for(i=1;i<=32;i+=2) {print "0x"substr($1,i,2)}}'`

    echo $TMPRC4KEY

    #Don't crypt picture about loading splash
    isNeedCrypto="yes";
    if [[ $subdir == "loadsplash" ]]
    then
        isNeedCrypto="no";
    fi


    for src in $LIST; do
        dst=`echo $src | sed 's/png$/dat/'`
        dst=../$DAT_DIR/$dst

        # TODO: encrypt files
        echo "$src --> $dst"
        echo `pwd`
        if [[ x$isNeedCrypto == x"yes" ]]
        then
            ../cryptool -e $src $dst $RC4KEY
        else
            cat $src >  $dst 
        fi
    done
    cd ../
    genCppFile $subdir
    cd $PICTURE_DIR
    sleep  1   # make sure srand to gen different random
    
done



cd ..



