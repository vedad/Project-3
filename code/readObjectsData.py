#!/usr/bin/env python
"""
Created on Tir 22 Oct 2013

Script for reading and plotting data from celestial objects outputted from C++
simulations of the solar system.

@author Kristoffer Braekken
"""
from matplotlib import pyplot as plt
import numpy as np
import os,sys

"""Constants"""

OBJECTS_PATH = '../data/objects/'

"""Classes"""

class ObjectData(object):
    """
    Contains the data for one single object.
    """
    def __init__(self, datafile):
        """
        Name for object is found in datafile.

        @param datafile The file containing positional data.
        """
        self.name = self._findName(datafile)
        self._read(datafile)

    def _read(self, datafile):
        """
        Reads datafile and puts into arrays.
        """
        inData = open(datafile, 'r')
        x = []
        y = []

        # First read till end of header
        inData.readline()

        for line in inData:
            columns = line.split()
            x.append(columns[0])
            y.append(columns[1])

        self.x,self.y = np.asarray(x), np.asarray(y)
        inData.close()

    def _findName(self, datafile):
        """
        Finds the name of the object from the datafile.

        @param datafile The file containing data.
        """
        inData = open(datafile, 'r')

        for line in inData:
            if line.startswith('Positions'):
                inData.close()
                return line.strip().split(':')[1].strip()
        
        # Rapid err msg, no time dude
        print 'If you see this the datafile has wrong syntax.'
        sys.exit(1)

"""Methods"""

if __name__ == '__main__':
    """
    Running script will loop through all files in objects folder and add them
    to the plot along with name as label.
    """
    files = os.listdir(OBJECTS_PATH)

    fig = plt.figure()
    fig.suptitle('The solar system')
    ax = fig.add_subplot(111)
    ax.set_xlabel('$x$')
    ax.set_ylabel('$y$')
    ax.grid('on')

    for datafile in files:
        data = ObjectData(os.path.join(OBJECTS_PATH,datafile))
        ax.plot(data.x,data.y,label=data.name)

    ax.legend(loc='best')

    plt.show()
