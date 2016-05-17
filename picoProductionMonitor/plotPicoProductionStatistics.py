#!/usr/common/usg/software/python/2.7.6/bin/python
__author__ = "Mustafa Mustafa"
__email__ = "mmustafa@lbl.gov"
import sys
import os
import time
import datetime
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

gLogsExtension = 'nEventsCheck.log'
gNumberOfEventsVsDayFileName = 'numberOfEventsVsDay.png'
gTotalNumberOfEventsVsDayFileName = 'totalNumberOfEventsVsDay.png'

def main():
    logsDirectory = sys.argv[1]

    listOfLogs = getListOfFiles(logsDirectory)

    totalMuDstEvents = 0
    totalPicoEvents = 0

    nEventsVsDay = {}

    for f in listOfLogs:
        elements = getLogElements(f)

        if not elements['picoFileDeleted']:
            totalPicoEvents += elements['nPicoEvents']
            totalMuDstEvents += elements['nMuDstEvents']
            if not elements['prodDay'] in nEventsVsDay: nEventsVsDay[elements['prodDay']] = 0
            nEventsVsDay[elements['prodDay']] += elements['nPicoEvents']
        else:
            print f

    print "totalPicoEvents/totalMuDstEvents = ",totalPicoEvents/float(totalMuDstEvents)

    plotNumberOfEventsVsDay(nEventsVsDay.items())
    makeIndexFile(totalMuDstEvents,totalPicoEvents)

def makeIndexFile(nMuDstEvents,nPicoEvents):
    now = time.strftime("%c")
    timeZone = time.strftime("%Z")
    
    os.system('rm -f index.md index.html')
    os.system('echo \#\#Total number of produced picoDst events = %i >> index2.md'%nPicoEvents)
    os.system('echo ![]\(%s\) >> index2.md'%gTotalNumberOfEventsVsDayFileName)
    os.system('echo ![]\(%s\) >> index2.md'%gNumberOfEventsVsDayFileName)
    os.system('echo \#\#\#\#Number of Produced PicoDst Events/Number of Processed MuDst Events = %1.10f >> index2.md'%(nPicoEvents/float(nMuDstEvents)))
    os.system('echo \ \ >> index2.md')
    os.system('echo \ \ *Fit is for last 7 days only.>> index2.md')
    os.system('echo \ \ >> index2.md')
    os.system('echo \ \ Last updated on %s %s >> index2.md'%(now,timeZone))
    os.system('cat index2.md indexProduction1.md > index.md')
    os.system('rm -f index2.md')
    os.system('./markdown index.md > index.html')
    os.system('chmod a+rw index.md')
    os.system('chmod a+rw index.html')

def plotNumberOfEventsVsDay(nEventsVsDay):

    nEventsVsDay = sorted(nEventsVsDay, key=lambda x: datetime.datetime.strptime(x[0], '%m/%d/%Y'))
    x = [datetime.datetime.strptime(d[0],'%m/%d/%Y').date() for d in nEventsVsDay]
    y = [v[1]/1.e6 for v in nEventsVsDay]

    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%m/%d'))
    # plt.gca().xaxis.set_major_locator(mdates.DayLocator(bymonthday=range(1,32,3)))
    plt.scatter(x,y)
    plt.gcf().autofmt_xdate()
    plt.gca().set_ylim([0,70])
    plt.gca().yaxis.grid() 
    plt.gca().xaxis.grid() 
    plt.ylabel('nEvents (m)/Day')
    plt.savefig(gNumberOfEventsVsDayFileName)
    os.system('chmod a+rw '+gNumberOfEventsVsDayFileName)
    plt.close()

    yy = [y[0]]
    for i in xrange(1,len(y)):
        yy.append(y[i]+yy[i-1])

    # projection fit
    xNum = mdates.date2num(x[-7:])
    fit = np.polyfit(xNum,yy[-7:],1)
    projectionDate = mdates.date2num(datetime.datetime.strptime('08/24/2015','%m/%d/%Y').date())
    xx = np.linspace(xNum.min(),projectionDate,300)
    dd = mdates.num2date(xx)

    plotText = '                                                                                                   *Average is %1.2fM events per day'%fit[0]
    plotTextX = datetime.datetime.strptime('04/01/2015','%m/%d/%Y').date()

    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%m/%d'))
    plt.scatter(x,yy)
    plt.plot(dd,fit[0]*xx+fit[1],'r-')
    plt.text(plotTextX,1405,plotText,color='r',fontsize=16)
    plt.gca().xaxis_date()
    plt.gcf().autofmt_xdate()
    plt.gca().set_ylim([50,1600])
    plt.gca().yaxis.grid() 
    plt.gca().xaxis.grid() 
    plt.ylabel('Total nEvents (m)')
    plt.show()
    plt.savefig(gTotalNumberOfEventsVsDayFileName)
    os.system('chmod a+rw '+gTotalNumberOfEventsVsDayFileName)


def getLogElements(f):

    elements = {'muFileName': 0, 'nMuDstEvents' : 0, 'nPicoEvents' : 0, 'picoFileDeleted' : False}
    timestamp = time.localtime(os.path.getmtime(f))
    elements['prodDay'] = '%s/%s/%s' % (timestamp.tm_mon,timestamp.tm_mday,timestamp.tm_year)

    for line in open(f):
        if "muFileName" in line: elements['muFileName'] = line.split(' ')[1]
        if "nMuDstEvents" in line: elements['nMuDstEvents'] = int(line.split(' ')[2])
        if "nPicoEvents" in line: elements['nPicoEvents'] = int(line.split(' ')[2])
        if "rm -f" in line: elements['picoFileDeleted'] = True

    return elements

def getListOfFiles(dir):

    listOfLogs = []
    for root,dirs,files in os.walk(dir):
        for f in files:
            if(f.find(gLogsExtension)): listOfLogs.append(os.path.join(root,f))

    return listOfLogs

if __name__ == '__main__':
    main()
