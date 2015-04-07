#!/bin/csh

#   Submit fileLists for classes derived from 
#    StPicoHFMaker;
#
#
# ###############################################


# -- prepare folder
mkdir -p report err log list production

# -- baseFolder of job
set baseFolder=/project/projectdirs/star/rnc/jthaeder/analysis/200GeV/lambdaC

# --input file 
set input=${baseFolder}/test.list

# -- production Id (kAnalyse / kRead)
set productionId=`date +%F_%H-%M`

# -- production Id (kWrite)
set treeName=hfTree

# -- set maker mode
#    0 - kAnalyse, 
#    1 - kWrite
#    2 - kRead
set makerMode=1

# -- set root macro
set rootMacro=runPicoHFLambdaCMaker.C

# ###############################################

# -- submit 
star-submit-template -template submitPicoHFMaker.xml -entities listOfFiles=${input},basePath=${baseFolder},prodId=${productionId},mMode=${makerMode},treeName=${treeName},rootMacro=${rootMacro}

