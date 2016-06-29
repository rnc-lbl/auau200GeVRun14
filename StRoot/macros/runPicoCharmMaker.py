#!/common/star/star64/opt/star/sl64_gcc447/bin/python
__author__ = "Mustafa Mustafa"
__email__ = "mmustafa@lbl.gov"
""" a script to run runPicoCharmMaker.C over a list of files in one job """

import sys
import time
import datetime
import subprocess
from subprocess import PIPE,Popen

D0DIR   = '/project/projectdirs/starprod/hft/Run14/AuAu/200GeV/Pico16a/P16id/D0/'
KPiXDIR = '/project/projectdirs/starprod/hft/Run14/AuAu/200GeV/Pico16a/P16id/KPiX/'
HISTDIR = '/global/project/projectdirs/starprod/rnc/mustafa/Run14/AuAu/200GeV/Pico16a/P16id/charmProduction/picoD0Hists/'

def main():

    inputFilesList = sys.argv[1]
    file = open(inputFilesList,'r')
    listOfFiles = file.readlines()
    file.close()
    listOfFiles[:] = map(str.rstrip,listOfFiles)

    now = time.strftime("%c")
    timeZone = time.strftime("%Z")
    startTime = datetime.datetime.now()

    print 'Start time %s %s'%(now,timeZone)

    for f in listOfFiles:
        baseName = getBaseName(f)
        run = getRunNumber(baseName)
        day  = getDayNumber(run)

        command = 'runPicoCharmMaker.C(\"%s\",\"%s\")'%(f,baseName)
        print command
        subprocess.call(['root4star','-l','-b','-q','-x',command])

        moveFile(day, run, baseName, 'picoD0.root', D0DIR)
        moveFile(day, run, baseName, 'picoD0.hists.root', HISTDIR)
        moveFile(day, run, baseName, 'picoKPiX.root', KPiXDIR)

    now = time.strftime("%c")
    timeZone = time.strftime("%Z")
    endTime = datetime.datetime.now()

    print 'End time %s %s'%(now,timeZone)
    print 'Number of files %i'%(len(listOfFiles))
    print 'Time difference = ' 
    print (endTime - startTime).seconds


def moveFile(day, run, baseName, fileExtension, dir):

    dayDir = '%s/%s'%(dir,day)
    prodDir = '%s/%s/%s'%(dir,day,run)
    outFile = '%s.%s'%(baseName,fileExtension)

    subprocess.call(['mkdir','-p',prodDir])

    subprocess.call(['chgrp','rhstar',dayDir])
    subprocess.call(['chmod','g+xr',dayDir])

    subprocess.call(['chgrp','rhstar',prodDir])
    subprocess.call(['chmod','g+xr',prodDir])

    subprocess.call(['chgrp','rhstar',outFile])
    subprocess.call(['chmod','g+r',outFile])

    subprocess.call(['cp','-p',outFile,prodDir])
    subprocess.call(['rm','-f',outFile])

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
