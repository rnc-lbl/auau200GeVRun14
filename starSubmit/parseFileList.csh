#!/bin/csh
#  
#  Script to split up a fileList of picoDst.root files
#  and process them in individual root4star sessions
#  
#  - Used in makerMode = 1 (kWrite), to always have a 
#    corresponding ${treeName}.root file for each picoDst.root file
#
# ###############################################

starver SL15c

set fileList=$1
set basePath=$2
set prodId=$3
set mMode=$4
set treeName=$5
set jobId=$6
set rootMacro=$7
set badRunListFileName=$8
set productionBasePath=$9
set decayChannel=$10

echo FILELIST $fileList

foreach line ( `cat $fileList` )
    set fileBaseName=`basename $line`
    set day=`perl $STAR/StRoot/macros/embedding/getYearDayFromFile.pl -d ${fileBaseName}`
    set run=`perl $STAR/StRoot/macros/embedding/getYearDayFromFile.pl -r ${fileBaseName}`

    set outDirTree=${basePath}/production/${treeName}/${day}/${run}
    set outDirList=${basePath}/production/${prodId}/${day}/${run}
    set logDir=${basePath}/jobs/log/${prodId}/${day}/${run}

    mkdir -p $outDirTree $outDirList $logDir

    set outName=`echo $fileBaseName | awk -F ".picoDst.root" '{ print $1 }'`

    root -q -b -l StRoot/macros/loadSharedHFLibraries.C StRoot/macros/${rootMacro}++'("'${line}'","'${outName}'", '${mMode}', "'${badRunListFileName}'", "'${treeName}'", "'${productionBasePath}'", '${decayChannel}')' > ${jobId}_${day}_${run}.log

    mv *.${treeName}.root $outDirTree
    mv *.root  $outDirList

    tar -zcvf ${jobId}.log.tgz ${jobId}_${day}_${run}.log
    mv *.log.tgz   $logDir
    rm  ${jobId}_${day}_${run}.log
end
    
