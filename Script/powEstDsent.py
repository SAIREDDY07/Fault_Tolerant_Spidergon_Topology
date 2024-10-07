import os
PATH = os.path.abspath('../')
powerDissTilePath = PATH + "/log/dsent/power"
tilePath = PATH + "/log/nirgam/tileDetail"
dsentRouterConfig = "router-events.cfg"
dsentLinkConfig = "electrical-link-events.cfg"
bufwrite = 'NumberBufferWrites'	 	
bufread = 'NumberBufferReads'
crossbar = 'NumberCrossbarTraversals'
linkcount = 'NumberLinkTraversals'
#buffread and buffwrite is extracted from /log/nirgam/event.log file
#crossbar traversal details is extracted from /log/traffic/tile-i from each tile i

def main(dirName, tileNum):
	
	try:
		os.mkdir(PATH + "/log")
	except:
		pass

	try:
		os.mkdir(PATH + "/log/dsent")
	except:
		pass

	try:
		os.system("rm -rf ../log/dsent/power")
	except:
		pass	
	
	
	calculatePowerFromDsent(tileNum)


def calculatePowerFromDsent(tileNum):
	routerConfigFile = open(PATH + '/core/dsent/dsent-core/configs/' + dsentRouterConfig, "r")
	linkConfigFile = open(PATH + '/core/dsent/dsent-core/configs/' + dsentLinkConfig, "r")
	list_of_lines = routerConfigFile.readlines()
	
	linkRouterFileList_of_lines = linkConfigFile.readlines()
	

	os.chdir(PATH + '/core/dsent/dsent-core')
	for i in range(0, tileNum):
		tilefp = open(tilePath + '/tile-'+str(i), 'r')
		lines = tilefp.readlines()
		readline = list_of_lines[24].split(" ")
		writeline = list_of_lines[25].split(" ")
		crossbarline = list_of_lines[27].split(" ")
		linkline = linkRouterFileList_of_lines[19].split(" ")
		for line in lines:
			line = line.split(" ")
			if bufread in line:
			#	print(line)
				readline[-1]=line[-1]
			if bufwrite in line:
			#	print(line)				
				writeline[-1]=line[-1]
			if crossbar in line:
				crossbarline[-1]=line[-1]
			if linkcount in line:
				linkline[-1]=line[-1]
	
		writeline = " ".join(writeline)
		readline = " ".join(readline)
		crossbarline = " ".join(crossbarline)
		linkline = " ".join(linkline)
	
		list_of_lines[24] = readline
		list_of_lines[25] = writeline
		list_of_lines[27] = crossbarline
		linkRouterFileList_of_lines[19] = linkline

		routerConfigFile1 = open(PATH + '/core/dsent/dsent-core/configs/' + dsentRouterConfig, "w")
		routerConfigFile1.writelines(list_of_lines)
		routerConfigFile1.close()
	
		linkConfigFile1 = open(PATH + '/core/dsent/dsent-core/configs/' + dsentLinkConfig, "w")
		linkConfigFile1.writelines(linkRouterFileList_of_lines)
		linkConfigFile1.close()
				
				
		try:
			os.mkdir(powerDissTilePath)
		except:
			pass

		filename = "tile-" + str(i)
		os.system("./dsent.exe -cfg configs/"+dsentRouterConfig+" > " + powerDissTilePath + "/" +filename)
		os.system("./dsent.exe -cfg configs/"+dsentLinkConfig+" > "+powerDissTilePath+"/"+filename + "_link")		
					
		print('Power calculation for tile-'+str(i)+' done')
		
		

		
def createDataForMatlabGraph(rows, cols, sliceNum, dirName):
	
	try:
		os.mkdir(PATH + "/log/dsent/matlab")
	except:
		pass
	
	if os.path.exists(PATH + '/log/dsent/results/' + dirName) == False:
		os.system('mkdir '+ PATH + '/log/dsent/results/' + dirName)

	powerFP = open(PATH + "/log/dsent/matlab/power_tile", "w")

	powerTablestat = open(PATH + '/log/dsent/results/' + dirName + '/power_table_stat', "w")
	powerTablestat.write('TILEID' + '\t\t\t' + 'POWER_ESTIMATION' + '\t\n')
	
	totalPower = 0

	for i in range(0, cols*sliceNum):
		for j in range(0, rows):
			tilepRFP = open(PATH + '/log/dsent/power/tile-' + str(i*rows+j), 'r')
			tilepLFP = open(PATH + '/log/dsent/power/tile-' + str(i*rows+j) + "_link", 'r')
			lines = tilepRFP.readlines()
			linkPowLines = tilepLFP.readlines()
			dPOW = 0
			try:
				dPOW = float(lines[49].split(" ")[-1])+float(linkPowLines[7].split(" ")[-1])
				
			except:
				pass

			powerFP.write(str(dPOW) + '\t')
			powerTablestat.write(str(i*rows+j)+'\t\t\t'+str(dPOW)+'\t\n')
			totalPower += dPOW
		powerFP.write('\n')

	os.system('mv ' + PATH + '/log/dsent/matlab/* ' + PATH + '/log/dsent/results/'+ dirName)
	powerTablestat.write('\nTotal Network Power(In Watt):'+ str(totalPower))
		

def gnuplotData(NUM_ROWS, NUM_COLS, NUM_SLICE, dirName):
	try:
		os.mkdir(PATH + "/log/dsent/gnuplot")
	except:
		pass
	
	power = []

	matlabDataFP = open(PATH + '/log/dsent/results/'+dirName+'/power_tile','r')
	readlines = matlabDataFP.readlines()
	for line in readlines:
		line = line.split("\t")
		power.append(line[0:-1])
		
			
	power_log = open(PATH + "/log/dsent/gnuplot/power", 'w')
	for s in range(NUM_SLICE):
		for i in range(NUM_COLS):
			for j in range(NUM_ROWS):
				x = i
				y = 2*j+0.2
				null = 0
				k = 0.4
				while(k<0.61):
					power_log.write("\t" + str(x+k) + "\t" + str(y) + "\t" + str(null) + "\n");
					power_log.write("\t" +str(x+k)+ "\t" + str(y) + "\t" + str(power[NUM_COLS*s + i][j]) + "\n")
					k += 0.01

				power_log.write("\n");
	
				y = 2*j+0.8
				k = 0.4
				while(k < 0.61):
					power_log.write("\t" + str(x+k) + "\t" + str(y) + "\t" + str(null) + "\n");
					power_log.write("\t" +str(x+k)+ "\t" + str(y) + "\t" + str(power[NUM_COLS*s + i][j]) + "\n")
					k += 0.01
					
	
				power_log.write("\n\n");


	os.chdir(PATH + '/log/dsent')
	if os.path.exists('results') == False:
		os.system('mkdir results')

	if os.path.exists('results/graph') == False:
		os.system('mkdir results/graph')

	
	os.chdir(PATH)
	os.system('gnuplot gnuplot/dsentResults.gnu')
	os.chdir(PATH + '/log/dsent')
	os.system('mv results/graph/* results/' +dirName)
	os.system('rm -rf results/graph')
	power_log.close()

if __name__ == '__main__':
	configFile = open(PATH + "/config/nirgam.config", "r")
	lines = configFile.readlines()
	NUM_ROWS = 1
	NUM_COLS = 1
	NUM_SLICE = 1
	dirName = ''
	for line in lines:
		line = line.split(' ')
		if('NUM_ROWS' in line):
			NUM_ROWS = int(line[1])
		if('NUM_COLS' in line):
			NUM_COLS = int(line[1])
		if('NUM_SLICE' in line):
			NUM_SLICE = int(line[1])
		if('DIRNAME' in line):
			dirName = line[1]
	
	tileNum = NUM_ROWS * NUM_COLS * NUM_SLICE
	dirName = dirName[0:-1]
	
	print('Starting  power estimation for each tile using Dsent')
	
	#this function ensuring the directories created before use and calling power estimation function
	main(dirName, tileNum) 

	#It helps in collecting dynamic power from power dir and calculating total power estimation in network.
	#It also formats data for injecting in matlab scripts.
	createDataForMatlabGraph(NUM_ROWS, NUM_COLS, NUM_SLICE, dirName)
	
	#It formats data for gnuplot script
	gnuplotData(NUM_ROWS, NUM_COLS, NUM_SLICE, dirName)

	print('Power estimation complete!')
	print('power stat for each tile stored in nirgam/log/dsent/results/'+dirName+'/power_table_stat')
	print('gnuplot graphs stored in nirgam/log/dsent/results/' +dirName)
	print('input data file(power_tile) for matlab created in nirgam/log/dsent/results/'+dirName)







