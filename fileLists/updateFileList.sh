#!/bin/bash

baseFolder=/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/physics/P15ic

outFolder=/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/fileLists/picoLists
#outFolderGIT=/global/homes/j/jthaeder/picoDstTransfer/auau200GeVRun14/fileLists/picoLists
outFolderGIT=`pwd`/picoLists

tmpFolder=/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/fileLists/tmp
mkdir -p ${tmpFolder}/runs

tmpAll=${tmpFolder}/picoList_all.list
if [ -f $tmpAll ] ; then
    rm $tmpAll
fi
touch $tmpAll

tmpRunList=${tmpFolder}/picoList_runList.list
if [ -f $tmpRunList ] ; then
    rm $tmpRunList
fi
touch $tmpRunList

yesterday=`date --date="1 days ago" '+%F'`
yesterdayFile=${outFolder}/daily/picoList_${yesterday}.list

tmpYesterdayFile=${tmpFolder}/picoList_${yesterday}.list
if [ -f $tmpYesterdayFile ] ; then
    rm $tmpYesterdayFile
fi 
touch  $tmpYesterdayFile

for day in `ls ${baseFolder} | sort` ; do 

    if [[ "${day}" = "preview2" ||  "${day}" = "fileLists" ]] ; then
	continue
    fi

    for run in `ls ${baseFolder}/${day} | sort` ; do 
	nFiles=`ls ${baseFolder}/${day}/${run} | wc -l`
	if [ $nFiles -eq 0 ] ; then 
	    continue
	fi

	tmpRun=${tmpFolder}/runs/picoList_${day}_${run}.list
	runEntry=${outFolder}/runs/picoList_${day}_${run}.list
	echo ${runEntry} >> ${tmpRunList} 

	find ${baseFolder}/${day}/${run} -name "*.root" | sort > ${tmpRun}
	cat ${tmpRun} >> ${tmpAll}
    done
done

cp -r ${tmpAll} ${outFolder}
cp -r ${tmpRunList} ${outFolder}
cp -r ${tmpFolder}/runs ${outFolder}

cp -r ${tmpAll} ${outFolderGIT}
cp -r ${tmpRunList} ${outFolderGIT}
cp -r ${tmpFolder}/runs ${outFolderGIT}

# ----------------------------------------------------

if [ ! -f ${yesterdayFile} ] ; then

    if [ ! -d ${outFolder}/daily ] ; then
	mkdir -p ${outFolder}/daily
    fi
    
    if [ ! -d ${outFolderGIT}/daily ] ; then
	mkdir -p ${outFolderGIT}/daily
    fi

    cat ${outFolder}/daily/picoList_*.list > ${tmpFolder}/picoUpToYesterday.list

    while read -r line ; do 
	grep ${line} ${tmpFolder}/picoUpToYesterday.list > /dev/null
	ret=$?
	
	if [ ${ret} -eq 0 ] ; then
            continue
	fi
	echo $line  >> ${tmpYesterdayFile}
	
    done < <(cat ${tmpAll})

    cp -r ${tmpYesterdayFile} ${outFolder}/daily
    cp -r ${tmpYesterdayFile} ${outFolderGIT}/daily
    
fi

# ----------------------------------------------------

rm -rf ${tmpFolder}

chmod 644 ${outFolder}/*.* 
chmod 755 ${outFolder}/runs
chmod 644 ${outFolder}/runs/*.* 

chmod 644 ${outFolderGIT}/*.* 
chmod 755 ${outFolderGIT}/runs
chmod 644 ${outFolderGIT}/runs/*.* 

now=`date '+%F %H-%M'`

pushd /global/homes/j/jthaeder/picoDstTransfer/auau200GeVRun14/fileLists > /dev/null
/usr/bin/git pull origin master
/usr/bin/git add picoLists
/usr/bin/git commit picoLists -m "update file list - ${now}"
/usr/bin/git push
popd > /dev/null