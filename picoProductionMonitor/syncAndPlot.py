#!/usr/common/usg/software/python/2.7.6/bin/python
__author__ = "Mustafa Mustafa"
__email__ = "mmustafa@lbl.gov"
import sys
import os
import fcntl
import time
import subprocess

# gWaitTime = 60*120 # 120 minutes before trying to submit again
gRcf = 'mstftsm@rftpexp.rhic.bnl.gov'
gLogsLocationAtRcf = '/star/institutions/lbl_prod/Run14_AuAu200GeV/production/CheckLog'
gLogsLocationAtPdsf = '.'
gDirectoryName = './CheckLog'

def main():

    # while True:
        directoryToSync = gRcf+':'+gLogsLocationAtRcf
        subprocess.call(('rsync','-avzhp',directoryToSync,gLogsLocationAtPdsf))
        subprocess.call(('./plotPicoProductionStatistics.py',gDirectoryName))

        # os.system("date")
        # print "... waiting for another ",gWaitTime/60," minutes before trying to submit again"
        # time.sleep(gWaitTime)


if __name__ == '__main__':
    main()
