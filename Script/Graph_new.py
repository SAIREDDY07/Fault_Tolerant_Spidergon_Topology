#!/usr/bin/env python3

import os
import sys
import matplotlib.pyplot as plt
import numpy as np

RT_ALGO = ['PROM']
DATA_PATTERN = ['Bursty']
TRAFFIC = [ 'TRANSPOSE']
LOAD = ['10', '100', '10']
PATH = os.path.abspath('../#Nirgam(Spidergon)/')
SOURCES = []
ROWS = 4
COLS = 4


def generateGraph():
    global RT_ALGO, TRAFFIC, LOAD, PATH, SOURCES, ROWS, COLS, DATA_PATTERN
    dataFilePath = os.path.abspath('../Results' + str(ROWS) + str(COLS))
    print(dataFilePath)
    GraphPath = os.path.abspath('../Graphs/Results_' + 'dim_' + str(ROWS) + str(COLS))
    print(dataFilePath)
    dirs = os.listdir('..')
    # if 'Results' + '_dim_' + str(ROWS) + str(COLS) not in dirs:
    if 'Results' + str(ROWS) + str(COLS) not in dirs:
        print('No Results Directory Available...\nExiting...\n')
        sys.exit()
    if not os.path.exists('../Graphs'):
        os.mkdir('../Graphs')
    if not os.path.exists(GraphPath):
        os.mkdir(GraphPath)
    os.chdir(dataFilePath)
    Latency = {}
    Throughput = {}
    for traffic in TRAFFIC:
        for pattern in DATA_PATTERN:
            for algo in RT_ALGO:
                print('Reading ' + traffic + '-' + pattern + '-' + algo + ' ...')
                try:
                    LatencyDataFileHandle = open(traffic + '-' + pattern + '-' + algo + '-' + 'Latency.dat', 'r')
                    ThroughputDataFileHandle = open(traffic + '-' + pattern + '-' + algo + '-' + 'Throughput.dat', 'r')
                    if LatencyDataFileHandle and ThroughputDataFileHandle:
                        Latency[traffic + '-' + pattern + '-' + algo + '-Latency'] = []
                        Throughput[traffic + '-' + pattern + '-' + algo + '-Throughput'] = []
                        LatencyLines = LatencyDataFileHandle.readlines()
                        ThroughputLines = ThroughputDataFileHandle.readlines()
                        for line in LatencyLines:
                            Latency[traffic + '-' + pattern + '-' + algo + '-Latency'].append(float(line.split('\t')[-1][:-1]))
                        for line in ThroughputLines:
                            Throughput[traffic + '-' + pattern + '-' + algo + '-Throughput'].append(float(line.split('\t')[-1][:-1]))
                    LatencyDataFileHandle.close()
                    ThroughputDataFileHandle.close()
                except:
                    print('File Not Found')

            try:
                ax = plt.subplot(211)
                ax.set_xscale('linear')

                plt.title(str(ROWS) + str(COLS) + '\n' + traffic + ' traffic with ' + pattern + ' pattern', fontsize=12, color='g')

                plt.ylabel('Latency')
                plt.xlabel('Load (in %)')
                plt.grid(True)
                load = []
                for l in range(int(LOAD[0]), int(LOAD[1]) + 1, int(LOAD[2])):
                    load.append(l)
                for algo in RT_ALGO:
                    ax.plot(np.array(load), np.array(Latency[traffic + '-' + pattern + '-' + algo + '-Latency']), label=algo)
                ax.legend(loc=0)

                ax = plt.subplot(212)
                ax.set_xscale('linear')
                plt.ylabel('Throughput')
                plt.xlabel('Load (in %)')
                plt.grid(True)
                for algo in RT_ALGO:
                    ax.plot(np.array(load), np.array(Throughput[traffic + '-' + pattern + '-' + algo + '-Throughput']), label=algo)
                ax.legend(loc=0)

                plt.savefig(GraphPath + '/' + traffic + '-' + pattern + 'PROM.png')
                plt.close()
            except:
                print('\nGraph Error')


def main():
    generateGraph()
    print('\n')


if __name__ == '__main__':
    main()
