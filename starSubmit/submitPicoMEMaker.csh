#!/bin/csh

#   Submit fileLists for classes derived from 
#    StPicoHFMaker;
#
#  - script will create a folder ${baseFolder}/jobs
#    all submission related files will end up there
#
#  - in ${baseFolder} the script expects (links or the actual folders)
#      .sl64_gcc447
#      StRoot                     ( from the git repo )
#      run14AuAu200GeVPrescales   ( from the git repo )
#      starSubmit                 ( from the git repo )
#
#      picoLists                  ( from the fileList git repo )
#
#   - the rootMacro is expected in StRoot/macros
#
#   - the bad run list is expected in ${baseFolder}
#     or in ${baseFolder}/picoLists
#
# ###############################################

# -- baseFolder of job
set baseFolder=/global/homes/m/mlomnitz/RNC_17/HFAna_04_18_2015/auau200GeVRun14

# --input file 
#    makerMode 0,1 : list must contain picoDst.root files
#    makerMode 2   : list must contain picoHFtree.root files
set input=${baseFolder}/test1.list

# -- set maker mode
#    0 - kAnalyze, 
#    1 - kWrite
#    2 - kRead
#set makerMode=0

# -- set root macro
set rootMacro=runPicoMixedEvent.C

# -- set filename for bad run list
set badRunListFileName="picoList_bad_MB.list"

# ###############################################
# -- DON'T CHANGE BELOW THAT LINE
# ###############################################

# -- production Id (kAnalyse / kRead)
set productionId=`date +%F_%H-%M`

# -- production Id (kWrite)
set treeName=hfTree

# -- job submission directory
mkdir -p ${baseFolder}/jobs

# -- result directory
mkdir -p ${baseFolder}/production

pushd ${baseFolder}/jobs > /dev/null

# -- prepare folder
mkdir -p report err log list csh

# -- check for prerequisits and create links
set folders=".sl64_gcc447 StRoot run14AuAu200GeVPrescales starSubmit"

foreach folder ( $folders ) 
    if ( ! -d ${baseFolder}/${folder} ) then
	echo "${folder} does not exist in ${baseFolder}"
	exit
    else
	ln -sf  ${baseFolder}/${folder}
    endif
end

if  ( ! -e ${baseFolder}/StRoot/macros/${rootMacro} ) then
    echo "${rootMacro} does not exist in ${baseFolder}/StRoot/macros"
    exit
endif

## check if macro compiles
if ( -e compileTest.log ) then
    rm compileTest.log
endif

root -l -b -q starSubmit/compileTest.C |& cat > compileTest.log 
cat compileTest.log |& grep "Compilation failed!"
if ( $status == 0 ) then
    echo "Compilation of ${rootMacro} failed"
    cat compileTest.log
    exit
else
    rm compileTest.log
endif

if ( ! -e ${input} ) then
    echo "Filelist ${input} does not exist"
    exit
endif

if ( ! -e ${baseFolder}/starSubmit/submitPicoMEMaker.xml ) then
    echo "XML submitPicoMEMaker.xml does not exist"
    exit
else
    ln -sf ${baseFolder}/starSubmit/submitPicoMEMaker.xml 
endif

if  ( -e ${baseFolder}/${badRunListFileName} ) then
    cp  ${baseFolder}/${badRunListFileName} picoList_badRuns.list
else if ( -e ${baseFolder}/picoLists/${badRunListFileName} ) then
    cp  ${baseFolder}/picoLists/${badRunListFileName} picoList_badRuns.list
else
    echo "${badRunListFileName} does not exist in ${baseFolder} nor ${baseFolder}/picoLists"
    exit
endif

if ( -e LocalLibraries.zip ) then
    rm LocalLibraries.zip
endif 

if ( -d LocalLibraries.package ) then
    rm -rf LocalLibraries.package
endif 

# -- submit 

##### temporary hack until -u ie option becomes availible

set hackTemplate=submitPicoMEMaker_temp.xml 

if ( -e submitPicoMEMaker_temp.xml  ) then
    rm submitPicoMEMaker_temp.xml 
endif 

echo '<?xml version="1.0" encoding="utf-8" ?>' > $hackTemplate
echo '<\!DOCTYPE note [' >> $hackTemplate
echo '<\!ENTITY treeName "'${treeName}'">'    >> $hackTemplate
echo '<\!ENTITY rootMacro "'${rootMacro}'">' >> $hackTemplate
echo '<\!ENTITY prodId "'${productionId}'">' >> $hackTemplate
echo '<\!ENTITY basePath "'${baseFolder}'">' >> $hackTemplate
echo '<\!ENTITY listOfFiles "'${input}'">'   >> $hackTemplate
echo ']>' >> $hackTemplate

tail -n +2 submitPicoMEMaker.xml >> $hackTemplate

star-submit -u ie $hackTemplate

#star-submit-template -template submitPicoMEMaker.xml -entities listOfFiles=${input},basePath=${baseFolder},prodId=${productionId},mMode=${makerMode},treeName=${treeName},rootMacro=${rootMacro}

popd > /dev/null
