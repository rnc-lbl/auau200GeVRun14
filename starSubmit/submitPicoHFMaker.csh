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
#   - the rootMacro is expected in StRoot/macros
#
# ###############################################

# -- baseFolder of job
set baseFolder=/path/to/your/directory

# --input file 
#    makerMode 0,1 : list must contain picoDst.root files
#    makerMode 2   : list must contain picoHFtree.root files
set input=${baseFolder}/test.list

# -- set maker mode
#    0 - kAnalyse, 
#    1 - kWrite
#    2 - kRead
set makerMode=0

# -- set root macro
set rootMacro=runPicoHFMyAnaMaker.C

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

if ( ! -e ${input} ) then
    echo "Filelist ${input} does not exist"
    exit
endif

if ( ! -e ${baseFolder}/starSubmit/submitPicoHFMaker.xml ) then
    echo "XML submitPicoHFMaker.xml does not exist"
    exit
else
    ln -sf ${baseFolder}/starSubmit/submitPicoHFMaker.xml 
endif

if ( ${makerMode} == 0 || ${makerMode} == 1 ) then
    head -n 2 ${input} | grep ".picoDst.root" > /dev/null
    if ( $? != 0 ) then
	echo "Filelist does not contain picoDsts!"
	exit
    endif
else if ( ${makerMode} == 2 ) then
    head -n 2 ${input} | grep ".picoHFtree.root" > /dev/null
    if ( $? != 0 ) then
	echo "Filelist does not contain HFtrees!"
	exit
    endif
endif

if ( -e LocalLibraries.zip ) then
    rm LocalLibraries.zip
endif 

if ( -d LocalLibraries.package ) then
    rm LocalLibraries.package
endif 

# -- submit 
star-submit-template -template submitPicoHFMaker.xml -entities listOfFiles=${input},basePath=${baseFolder},prodId=${productionId},mMode=${makerMode},treeName=${treeName},rootMacro=${rootMacro}

popd > /dev/null
