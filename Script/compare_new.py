#!/usr/bin/env python

import os, commands, sys

RT_ALGO = [ 'XY' ]
DATA_PATTERN = ['Bursty']
TRAFFIC = ['RANDOM', 'TRANSPOSE']
LOAD = ['10', '100', '10']
PATH = os.path.abspath('../')
SOURCES = []
ROWS = 4
COLS = 4 
# SLICES = 4

def changeRoutingAlgo(algo, nirgam):
	global ROWS, COLS
	nirgamFileHandle = open(nirgam + '/config/nirgam.config', 'w')
	tags = ['TOPOLOGY', 'NUM_ROWS', 'NUM_COLS', 'LOG', 'RT_ALGO', 'WARMUP', 'DIRNAME', 'SIM_NUM', 'TG_NUM', 'NUM_BUFS']
	value = ['SPIDERGON', '', '', '4', '', '50', 'Results_Spidergon', '10000', '3000', '10']
	i = 0
	for tag in tags:
		if tag == 'NUM_ROWS':
			value[i] = str(ROWS)
		if tag == 'NUM_COLS':
			value[i] = str(COLS)
		# if tag == 'NUM_SLICE':
		# 	value[i] = str(SLICES)
		if tag == 'RT_ALGO':
			value[i] = algo
		nirgamFileHandle.write(tag+' '+value[i]+'\n')
		i = i + 1
	nirgamFileHandle.close()

def changeSources(pattern, nirgam):
	global ROWS, COLS, SOURCES
	AppFileHandle = open(nirgam + '/config/application.config', 'w')
	if not SOURCES:
		for tile in range(0, ROWS*COLS):
			AppFileHandle.write(str(tile)+' '+ pattern +'.so'+'\n')
	else:
		for tile in SOURCES:
			AppFileHandle.write(str(tile)+' ' + pattern + '.so'+'\n')		
	AppFileHandle.close()

def changeLoadInTiles(load, traffic, nirgam):
	global ROWS, COLS, SOURCES
	tags = ['PKT_SIZE', 'LOAD', 'DESTINATION', 'AVG_BURST_LEN', 'AVG_OFFTIME', 'FLIT_INTERVAL']
	value = ['20', '', '', '3', '3', '3']
	if not SOURCES:
		for tile in range(0, ROWS*COLS):
			TileFileHandle = open(nirgam + '/config/traffic/tile-' + str(tile), 'w')
			i = 0
			for tag in tags:
				if tag == 'LOAD':
					value[i] = str(load)
				if tag == 'DESTINATION':
					value[i] = traffic
				TileFileHandle.write(tag+' '+value[i]+'\n')
				i = i + 1
			TileFileHandle.close()
	else:
		for tile in SOURCES:
			TileFileHandle = open(nirgam + '/config/traffic/tile-' + str(tile), 'w')
			i = 0
			for tag in tags:
				if tag == 'LOAD':
					value[i] = str(load)
				if tag == 'DESTINATION':
					value[i] = traffic
				TileFileHandle.write(tag+' '+value[i]+'\n')
				i = i + 1
			TileFileHandle.close()

def runSimulation():
	global RT_ALGO, TRAFFIC, LOAD, PATH, SOURCES, ROWS, COLS, SLICES
	dataFilePath = os.path.abspath('../Results'+str(ROWS)+str(COLS))
	nirgamPath = PATH
	simulationResultFilePath = nirgamPath + '/results/Results_Spidergon/stats/sim_results'
	dirs = os.listdir('..')
	if 'Results'+str(ROWS)+str(COLS) not in dirs:
		os.mkdir(dataFilePath)
	os.chdir(dataFilePath)
	for traffic in TRAFFIC:
		for pattern in DATA_PATTERN:
			for algo in RT_ALGO:
				LatencyDataFileHandle = open(traffic + '-' + pattern + '-' + algo + '-' + 'Latency.dat', 'w')
				ThroughputDataFileHandle = open(traffic + '-' + pattern + '-' + algo + '-' + 'Throughput.dat', 'w')
				changeRoutingAlgo(algo, nirgamPath)
				changeSources(pattern, nirgamPath)
				for load in range(int(LOAD[0]), int(LOAD[1]) + 1, int(LOAD[2])):
					changeLoadInTiles(load, traffic, nirgamPath)
					curDir = os.path.abspath(os.curdir)
					os.chdir(nirgamPath)
					avgLatency = avgThroughput = 0
					print 'Running simulation for', traffic, 'traffic with ', pattern, 'pattern, ', algo, 'algorithm at /n load value', load, '...'
					for i in range(0, 5):
						print '\tfor', i + 1, 'time...'
						status, output = commands.getstatusoutput('./nirgam')
					#	print output
						simulationResultFileHandle = open(simulationResultFilePath, 'r')
						Lines = simulationResultFileHandle.readlines()
						for line in Lines:

							line = line.split(' ')
							val = line[-1]
							tag = ' '.join(line[:-1])
							if tag == 'Overall average latency (in clock cycles per flit) =':
								avgLatency = avgLatency + float(val[:-1])
							if tag == 'Overall total throughput is':
#								print '\nThroughput : ', val[:-1]
								avgThroughput = avgThroughput + float(val[:-1])
						simulationResultFileHandle.close()
					avgLatency = avgLatency/(i + 1)
					avgThroughput = avgThroughput/(i + 1)
					print '\t', (i+1), 'Avg Latency =', avgLatency, '; Avg Throughput =', avgThroughput
					LatencyDataFileHandle.write(str(load)+'\t'+str('%.*f' % (5, avgLatency))+'\n')
					ThroughputDataFileHandle.write(str(load)+'\t'+str('%.*f' % (5, avgThroughput))+'\n')
					os.chdir(curDir)
				LatencyDataFileHandle.close()
				ThroughputDataFileHandle.close()

def main():
#	output = commands.getoutput('export SC_SIGNAL_WRITE_CHECK=DISABLE')
	try:
		runSimulation()
	except Exception as e:
		print 'User Interruption ...\nExiting ...'+str(e)

	print '\n',

if __name__ == '__main__':
	main()
