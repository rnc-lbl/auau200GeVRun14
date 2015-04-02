#!/usr/common/usg/software/python/2.7.6/bin/python
__author__ = "Mustafa Mustafa"
__email__ = "mmustafa@lbl.gov"
import sys
import subprocess

# gWaitTime = 60*120 # 120 minutes before trying to submit again
gRcf = '@rftpexp.rhic.bnl.gov'
gLogsLocationAtRcf = '/star/institutions/lbl_prod/Run14_AuAu200GeV/production/CheckLog'
gLogsLocationAtPdsf = '.'
gDirectoryName = './CheckLog'

def main():

    if len(sys.argv) != 2:
        print "Usage %s rcfUserName"%sys.argv[0]
        return

    userName = sys.argv[1]
    directoryToSync = userName + gRcf+':'+gLogsLocationAtRcf
    subprocess.call(('rsync','-Oavzhp',directoryToSync,gLogsLocationAtPdsf))
    subprocess.call(('./plotPicoProductionStatistics.py',gDirectoryName))


if __name__ == '__main__':
    main()
