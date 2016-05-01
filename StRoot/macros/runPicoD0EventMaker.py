#!/common/star/star64/opt/star/sl64_gcc447/bin/python
__author__ = "Mustafa Mustafa"
__email__ = "mmustafa@lbl.gov"
""" a script to run runPicoD0EventMaker.C over a list of files in one job """

import sys
import subprocess
from subprocess import PIPE,Popen

EOUTDIR  = '/project/projectdirs/starprod/hft/d0tree/Run14/AuAu/200GeV/kfProd2/P16id/'
EKFVTXDIR  = '/project/projectdirs/starprod/hft/kfVertex/Run14/AuAu/200GeV/physics2/P16id/'
HISTDIR  = '/global/project/projectdirs/starprod/rnc/mustafa/d0tree/Run14/AuAu/200GeV/kfProd2/P16id/picoD0Hists/'

def main():

    inputFilesList = sys.argv[1]
    file = open(inputFilesList,'r')
    listOfFiles = file.readlines()
    file.close()
    listOfFiles[:] = map(str.rstrip,listOfFiles)

    for f in listOfFiles:
        baseName = getBaseName(f)
        run = getRunNumber(baseName)
        day  = getDayNumber(run)

        command = 'runPicoD0EventMaker.C(\"%s\",\"%s\")'%(f,baseName)
        print command
        subprocess.call(['root4star','-l','-b','-q','-x',command])

        dayDir = '%s/%s'%(EOUTDIR,day)
        prodDir = '%s/%s/%s'%(EOUTDIR,day,run)
        histDir = '%s/%s/%s'%(HISTDIR,day,run)
        kfVtxDir = '%s/%s/%s'%(EKFVTXDIR,day,run)

        outFile = '%s.picoD0.root'%baseName
        outHist = '%s.picoD0.hists.root'%baseName
        outKfVtx = '%s.kfVertex.root'%baseName

        subprocess.call(['mkdir','-p',prodDir])
        subprocess.call(['mkdir','-p',histDir])
        subprocess.call(['mkdir','-p',kfVtxDir])

        subprocess.call(['chgrp','rhstar',dayDir])
        subprocess.call(['chgrp','rhstar',prodDir])
        subprocess.call(['chgrp','rhstar',kfVtxDir])

        subprocess.call(['chmod','g+xr',dayDir])
        subprocess.call(['chmod','g+xr',prodDir])
        subprocess.call(['chmod','g+xr',kfVtxDir])

        subprocess.call(['chmod','g+r',outFile])
        subprocess.call(['chmod','g+r',outKfVtx])

        subprocess.call(['cp','-p',outFile,prodDir])
        subprocess.call(['cp','-p',outHist,histDir])
        subprocess.call(['cp','-p',outKfVtx,kfVtxDir])

def getBaseName(file):
    return file[file.find('st_'):file.find('.')]
 
def getRunNumber(baseName):
    idx = baseName.find('_raw')
    if idx > 0:
        return int(baseName[idx-8:idx])
    else:
        return -1
            
def getDayNumber(runnumber):
    if(runnumber<1e7): return 0
            
    return int((runnumber%1e6)/1e3)

if __name__ == '__main__':
    main()
