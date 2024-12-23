
/*
 * main.cpp
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * Author: Lavina Jain
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file main.cpp
/// \brief Begins and ends simulation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NoC.h"
#include "../config/default.h"
#include "orion/orion_router_power.h"
#include "sstream"

ofstream eventlog;
ofstream results_log;
/*added*/
ofstream power_results_log;
/*end*/
///NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN/////////////////////////////////////////
//extern int fail[10][2],failno;
///NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN/////////////////////////////////////////

/****************************************************************************************/

ofstream latency_throughput_result1;

ULL max_latency=0;
ULL max_latency_GT=0;
ULL max_latency_BE=0;

//for Q-Router
ofstream qresults_log;
ofstream GTresults_log;
ofstream BEresults_log;
//sc_trace_file *tracefile;	// use to generate vcd trace
sc_clock *nw_clock;
string app_libname[MAX_NUM_TILES];

/****************************************************************************************/
string app_libname2[MAX_NUM_TILES];
/****************************************************************************************/
void itoa(int i, char str[]);

int sc_main(int argc, char *argv[])
{

    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
	system("rm -rf *.txt");
    system("rm -rf jitter/GT/*");
    system("rm -rf jitter/BE/*");
    cout<<"---------------------------------------------------------------------------"<<endl;
    cout<<"  NIRGAM: Simulator for NoC Interconnect RoutinG and Application Modeling\n";
    cout<<"---------------------------------------------------------------------------"<<endl;
    string DIRNAME = string("sim1");
    //string ALGO = string("XY");
    // open event log file
    string event_filename = string("log/nirgam/event.log");
    eventlog.open(event_filename.c_str());
    if (!eventlog)
    {
        cout<<"cannot open event.log "<<endl;
    }

//cout<<"hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
    // read simulator configuration (nirgam.config)
    string nirgam_filename = string("config/nirgam.config");
    ifstream fil1;
    fil1.open(nirgam_filename.c_str());


    cout<<"check";

    if (!fil1.is_open())
    {
        cout<<"case1";
        cout<<"nirgam.config does not exist, using default parameter values."<<endl;
    }
    else
    {
        cout<<"Case 2";
        //cout<<"Case 2";
        cout<<"Loading parameters from nirgam.config file...";
        while (!fil1.eof())
        {
            string name;
            fil1 >> name ;
            if (name=="LOG")
            {
                int value;
                fil1 >> value;
                LOG = value;
            }
            else if (name=="SIM_NUM")
            {
                ULL value;
                fil1 >> value;
                SIM_NUM = value;
            }
            else if (name=="WARMUP")
            {
                ULL value;
                fil1 >> value;
                WARMUP = value;
            }
            else if (name=="TG_NUM")
            {
                ULL value;
                fil1 >> value;
                TG_NUM = value;
            }
            else if (name=="SETTLE_NUM")
            {
                ULL value;
                fil1 >> value;
                settle_cycle = value;
            }
            else if (name=="RT_ALGO")
            {
                fil1 >> name;
                if (name == "XY")
                    RT_ALGO = XY;
                else if (name == "OE")
		  RT_ALGO = OE;
                else if (name == "SOURCE")
                    RT_ALGO = SOURCE;
		//else if(name == "WF")
		//	RT_ALGO = WF;
		else if(name == "NL")
			RT_ALGO = NL;
		else if(name =="WF")
			RT_ALGO= WF;
		//else if (name== "NG")
		//	RT_ALGO= NG;
		//else if (name== "FTWF")
		//	RT_ALGO= FTWF;
		else if (name== "FTNL")
			RT_ALGO= FTNL;
		else if (name== "FTNG")
			RT_ALGO= FTNG;
 		else if (name== "FTPROM")
			RT_ALGO= FTPROM;
		else if (name== "FTPROM3D")
			RT_ALGO= FTPROM3D;
		//	else if (name== "FTOE")
		//RT_ALGO= FTOE;
		 //PROM
                else if (name == "PROM")
                    RT_ALGO = PROM;
                //ERA
		/*   else if (name == "ER")
                    RT_ALGO = ER;
                else if (name == "QRT")
                    RT_ALGO = QRT;
                else if (name == "CRT")
                    RT_ALGO = CRT;
                //DYAD//
                else if (name== "DYAD")
		RT_ALGO= DYAD;*/
                ///////
                //MAXY//
		//   else if (name== "MAXY")
		//  RT_ALGO= MAXY;
                ///////
                //FTXY//
                //else if (name== "FTXY")
                  //  RT_ALGO= FTXY;
       		else if(name == "XYZ")
		  RT_ALGO = XYZ;
		else if(name == "ZXY")
			RT_ALGO = ZXY;					
		else if(name == "PROM3D")
		  	RT_ALGO = PROM3D;	
		/*	else if(name == "WSF")
			RT_ALGO = WSF;
		else if(name == "NUL")
			RT_ALGO = NUL;
		else if(name == "FTNUL")
			RT_ALGO = FTNUL;
		else if(name == "FTNF3D")
			RT_ALGO = FTNF3D;
		else if(name == "FTWSF")
		RT_ALGO = FTWSF;*/
		//else if(name == "FTXYZ")
		//RT_ALGO = FTXYZ;
		else if(name == "OE3D")
			RT_ALGO = OE3D;	
			
		//cout<<"\nFTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n";
                ///////
                //ALGO = name;
            }
            else if (name=="DIRNAME")
            {
                string value;
                fil1 >> value;
                DIRNAME = value;
            }
            else if (name=="TOPOLOGY")
            {
                fil1 >> name;
                if (name == "MESH")
                    TOPO = MESH;
                else if (name == "TORUS")
                    TOPO = TORUS;
                else if(name == "MESH3D")
			TOPO = MESH3D;	
		else if (name =="SPIDERGON")
			TOPO = SPIDERGON;
			cout<<"Spidergon"<<endl;				
            }
            else if (name=="NUM_ROWS")
            {
                int value;
                fil1 >> value;
                num_rows = value;
            }
            else if (name=="NUM_COLS")
            {
                int value;
                fil1 >> value;
                num_cols = value;
            }
             else if(name=="NUM_SLICE"){
		int value; 
		fil1 >> value; num_slice = value;
	    }
            else if (name=="NUM_BUFS")
            {
                int value;
                fil1 >> value;
                NUM_BUFS = value;
            }
            else if (name=="FLITSIZE")
            {
                int value;
                fil1 >> value;
                FLITSIZE = value;
            }
            else if (name=="HEAD_PAYLOAD")
            {
                int value;
                fil1 >> value;
                HEAD_PAYLOAD = value;
            }
            else if (name=="DATA_PAYLOAD")
            {
                int value;
                fil1 >> value;
                DATA_PAYLOAD = value;
            }
            else if (name=="CLK_FREQ")
            {
                double value;
                fil1 >> value;
                CLK_FREQ = value;
                CLK_PERIOD = (1/CLK_FREQ);
            }
            /*added*/
            else if (name=="INTERVAL")
            {
                ULL value;
                fil1 >> value;
                INTERVAL = value;
            }
            else if (name=="LINK_LEN")
            {
                double value;
                fil1 >> value;
                LINK_LEN = value;
            }
            /*end*/

            /**************reading BW_GT value from nirgam.config******************/
            else if (name=="BW_GT")
            {
                float value;
                fil1 >> value;
                BW_GT = value;
                qos=1;
            }

///NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN///////////////////////////////////////

            else if (name== "LINKFAIL")
            {
                int value;
                fil1>> value;
                failno=value;
                for (int k=0; k<value; k++)
                {
                    for (int m=0; m<2; m++)
                    {
                        fil1>>fail[k][m];
                        //cout<<"\nFAIL "<<fail[k][m];
                    }
                    fail[k][2]=0;
                    fail[k][3]=0;
                }
            }



///NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN//////////////////////////////////////

            else if (name == "ALPHA")
            {
                float value;
                fil1 >> value;
                ALPHA = value; //Q-Router
            }
            else if (name == "Q_SIMCOUNT")
            {
                ULL value;
                fil1 >> value;
                QXY_SIMCOUNT = value;
            }
            else if (name == "ESTBUF")
            {
                UI value;
                fil1 >> value;
                ESTBUF = value;
            }
            else if (name == "WAITS")
            {
                UI value;
                fil1>> value;
                WAITS = value;
            }
            else if (name == "BETA")
            {
                double value;
                fil1>> value;
                BETA = value;
            }
            else if (name == "DELTA")
            {
                double value;
                fil1>> value;
                DELTA = value;
            }
            else if (name == "PWAIT")
            {
                UI value;
                fil1>> value;
                PWAIT = value;
            }
        }
        //	cout<<"donnnnnnnnnnnnnnnn";//<<endl;
        //cout<<"m :(";
    }
    fil1.close();

	num_tiles = num_rows * num_cols * num_slice;	// compute number of tiles
	slice_size = num_rows * num_cols;

    nw_clock = new sc_clock("NW_CLOCK",CLK_PERIOD,SC_NS);	// create global clock


    // open log and result files
    DIRNAME = string("results/") + DIRNAME;
    string cmd = string("rm -rf ") + DIRNAME;
    system(cmd.c_str());


    cmd = string("mkdir ") + DIRNAME;
    system(cmd.c_str());
    string temp = cmd + string("/stats");
    system(temp.c_str());
    temp = cmd + string("/graphs");
    system(temp.c_str());

    temp = cmd + string("/graphs/qrt");
    system(temp.c_str());

    //cout<<"121";

///For -Q-Routing results
    ofstream gp_labels;
    // open event log file
    string qresults_filename =  DIRNAME;
    string label_filename = string("labels.plt");
    gp_labels.open(label_filename.c_str());
    if (!gp_labels.is_open())
    {
        cout<<"cannot open event.log "<<endl;
    }
    gp_labels<<"## This file is included for label"<<endl;
    gp_labels<<"cd \""<<qresults_filename<<"\""<<endl;
    gp_labels<<"set label 1 \"Routing Algorithm = "<<RT_ALGO<<"\" at graph 0.98, 0.88, 1 right"<<endl;
    gp_labels<<"set label 2 \"Value of ALPHA = "<<ALPHA<<"\" at graph 0.98, 0.85, 1 right"<<endl;
    gp_labels<<"set label 3 \"QXY_simcount = "<<QXY_SIMCOUNT<<"\" at graph 0.98, 0.82, 1 right"<<endl;
    gp_labels<<"set label 4 \""<<TOPO<<" "<<num_rows<<"x"<<num_cols<<"\" at graph 0.98, 0.79, 1 right"<<endl;
    gp_labels<<"set label 5 \"BUF_SIZE = "<<NUM_BUFS<<"\" at graph 0.98, 0.76, 1 right"<<endl;
    gp_labels<<"set label 6 \"EST_BUF = "<<ESTBUF<<"\" at graph 0.98, 0.73, 1 right"<<endl;
    gp_labels.close();


//////////

	string ylatency_filename = string("log/gnuplot/ylatency");
	ofstream ylatency_log;
	ylatency_log.open(ylatency_filename.c_str());
	if(!ylatency_log.is_open())
		cout<<"Cannot open ylatency"<<endl;
	
	string xlatency_filename = string("log/gnuplot/xlatency");
	ofstream xlatency_log;
	xlatency_log.open(xlatency_filename.c_str());
	if(!xlatency_log.is_open())
		cout<<"Cannot open xlatency"<<endl;
	
	string ylatency_flit_filename = string("log/gnuplot/ylatency_flit");
	ofstream ylatency_flit_log;
	ylatency_flit_log.open(ylatency_flit_filename.c_str());
	if(!ylatency_flit_log.is_open())
		cout<<"Cannot open ylatency_flit"<<endl;
	
	string xlatency_flit_filename = string("log/gnuplot/xlatency_flit");
	ofstream xlatency_flit_log;
	xlatency_flit_log.open(xlatency_flit_filename.c_str());
	if(!xlatency_flit_log.is_open())
		cout<<"Cannot open xlatency"<<endl;
	
	string ytput_filename = string("log/gnuplot/ytput");
	ofstream ytput_log;
	ytput_log.open(ytput_filename.c_str());
	if(!ytput_log.is_open())
		cout<<"Cannot open ytput"<<endl;
	
	string xtput_filename = string("log/gnuplot/xtput");
	ofstream xtput_log;
	xtput_log.open(xtput_filename.c_str());
	if(!xtput_log.is_open())
		cout<<"Cannot open xtput"<<endl;
	
	string mat_lat_pkt_filename = string("log/matlab/latency_pkt");
	ofstream mat_lat_pkt_log;
	mat_lat_pkt_log.open(mat_lat_pkt_filename.c_str());
	if(!mat_lat_pkt_log.is_open())
		cout<<"Cannot open "<<mat_lat_pkt_filename<<endl;
	
	string mat_lat_flit_filename = string("log/matlab/latency_flit");
	ofstream mat_lat_flit_log;
	mat_lat_flit_log.open(mat_lat_flit_filename.c_str());
	if(!mat_lat_flit_log.is_open())
		cout<<"Cannot open "<<mat_lat_flit_filename<<endl;
	
	string mat_tput_filename = string("log/matlab/throughput");
	ofstream mat_tput_log;
	mat_tput_log.open(mat_tput_filename.c_str());
	if(!mat_tput_log.is_open())
		cout<<"Cannot open "<<mat_tput_filename<<endl;

	///added
	string mat_power_filename = string("log/matlab/power_tile");
	ofstream mat_power_log;
	mat_power_log.open(mat_power_filename.c_str());
	if(!mat_power_log.is_open())
		cout<<"Cannot open "<<mat_power_filename<<endl;
	// added for LinkUsage
	char LinkUsage_str[50], slice_str[5];
	ofstream LinkUsage_log[16];
	for(int s=0; s<num_slice; s++)
	{
		strcpy(LinkUsage_str, "log/gnuplot/LinkUsage");
		itoa(s, slice_str);	strcat(LinkUsage_str, slice_str);
		string LinkUsage_file = string(LinkUsage_str);
		LinkUsage_log[s].open(LinkUsage_file.c_str());
		if(!LinkUsage_log[s].is_open())
			cout<<"Cannot open "<<LinkUsage_file.c_str()<<endl;
	}	
	// end of LinkUsage
	
	string results_file= DIRNAME + string("/stats/sim_results");
	results_log.open(results_file.c_str());
	if(!results_log.is_open())
		cout<<"Cannot open "<<results_file.c_str()<<endl;
	results_log<<"Tile\t"<<"Output\t\t"<<"Total no.\t"<<"Total no.\t"<<"avg. latency\t"<<"avg. latency\t"<<"average\n";
	results_log<<"ID\t"<<"channel\t\t"<<"of packets\t"<<"of flits\t"<<"per packet\t"<<"per flit\t"<<"throughput\n";
	results_log<<"\t\t\t\t\t\t\t(clock cycles)\t(clock cycles)\t(Gbps)\n";

	string qresults_file= DIRNAME + string("/stats/qsim_results");
	qresults_log.open(qresults_file.c_str());
	if(!qresults_log.is_open())
		cout<<"Cannot open "<<qresults_file.c_str()<<endl;
	qresults_log<<"#Simcount\t\t"<<"average latency\n";


	string GTresults_file= DIRNAME + string("/stats/GTsim_results");
	GTresults_log.open(GTresults_file.c_str());
	if(!GTresults_log.is_open())
		cout<<"Cannot open "<<GTresults_file.c_str()<<endl;
	GTresults_log<<"#Simcount\t\t"<<"average latency of GT\n";

	
	string BEresults_file= DIRNAME + string("/stats/BEsim_results");
	BEresults_log.open(BEresults_file.c_str());
	if(!BEresults_log.is_open())
		cout<<"Cannot open "<<BEresults_file.c_str()<<endl;
	BEresults_log<<"#Simcount\t\t"<<"average latency of BE\n";


	/*********************************************************************************/
	/*string results1_file= DIRNAME + string("/stats/latency_throughput_result1");
	latency_throughput_result1.open(results1_file.c_str());
	if(!latency_throughput_result1.is_open())
		cout<<"Cannot open "<<results_file.c_str()<<endl;
	latency_throughput_result1<<"CLK_CYCLE"<<"\t\tLatency_per_clk"<<endl; */
/*********************************************************************************/

	/*added*/
	string results_file1=DIRNAME + string("/stats/power_sim_results");
	power_results_log.open(results_file1.c_str());
	if(!power_results_log.is_open())
		cout<<"Cannot open "<<results_file1.c_str()<<endl;
	power_results_log<<"Tile ID\t\t\t\t\t\t"<<"Power estimation\n\n";
	/*end*/

	/* example to show how to generate vcd trace */
	//string trace_filename = string("log/vcdtrace/trace");
	//tracefile = sc_create_vcd_trace_file(trace_filename.c_str());

	// read application configuration (application.config)
	for(int i = 0; i < MAX_NUM_TILES; i++) {
		app_libname[i] = string("NULL");
	}
	string app_filename = string("config/application.config");
	ifstream app_fil;
	app_fil.open(app_filename.c_str());
	if(!app_fil.is_open()){
		cout<<"Error: File application.config could not be opened." << endl;
		exit(-1);
	}
	int id;
	string libname;
	while(!app_fil.eof()) {
		app_fil >> id >> libname;
		app_libname[id] = libname;
	}
	app_fil.close();

/**********************************************************************************************/
/*ifstream app_config_for_tt;//for trace traffic (tt) 

        string file2 = string("config/app_config_for_tt");
        app_config_for_tt.open(file2.c_str());
	
        while(app_config_for_tt){
                                
                app_config_for_tt >> id >> libname;
		app_libname2[id] = libname;
                //cout<<"ho ho ho "<<id<<libname;

                }
app_config_for_tt.close();*/
/**********************************************************************************************/
	
	// create NoC model
	NoC noc("noc", num_rows, num_cols, num_slice);  	//NoC noc("noc", num_rows, num_cols);
	// connect clock
	noc.switch_cntrl(*nw_clock);
		
	cout << "Network setup!" <<endl;
	cout<<"Start NIRGAM simulation!" <<endl;
	cout<<"---------------------------------------------------------------------------"<<endl;

	//sc_trace(tracefile, noc.switch_cntrl, "clk");
	sc_start();	// begin simulation

	//////////  3D NoC  /////////////////////////////////////////////////////////////////////////
	if(TOPO == MESH3D)
	{
				// generate matlab log
		float null = 0.0;
		for(int s=0; s < num_slice; s++)
		{
			for(int i = 0; i < num_cols; i++) {
				for(int j = 0; j < num_rows; j++) {
					mat_lat_pkt_log<<"\t"<<null;
					mat_lat_flit_log<<"\t"<<null;
					mat_tput_log<<"\t"<<null;
			
					if(j != num_rows - 1) {
						mat_lat_pkt_log<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency(S);
						mat_lat_flit_log<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency_flit(S);
						mat_tput_log<<"\t"<<(noc.nwtile3D[s][j][i])->return_avg_tput(S);
				
						mat_lat_pkt_log<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency(N);
						mat_lat_flit_log<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency_flit(N);
						mat_tput_log<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_avg_tput(N);
					}
				}
				mat_lat_pkt_log<<endl;
				mat_lat_flit_log<<endl;
				mat_tput_log<<endl;
			
		
				if(i != num_cols - 1) {
					for(int j = 0; j < num_rows; j++) {
						mat_lat_pkt_log<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency(E);
						mat_lat_flit_log<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency_flit(E);
						mat_tput_log<<"\t"<<(noc.nwtile3D[s][j][i])->return_avg_tput(E);
				
						if(j != num_rows - 1) {
							mat_lat_pkt_log<<"\t"<<null<<"\t"<<null;
							mat_lat_flit_log<<"\t"<<null<<"\t"<<null;
							mat_tput_log<<"\t"<<null<<"\t"<<null;
			
						}
					}
					mat_lat_pkt_log<<endl;
					mat_lat_flit_log<<endl;
					mat_tput_log<<endl;
					
			
					for(int j = 0; j < num_rows; j++) {
						mat_lat_pkt_log<<"\t"<<(noc.nwtile3D[s][j][i+1])->return_latency(W);
						mat_lat_flit_log<<"\t"<<(noc.nwtile3D[s][j][i+1])->return_latency_flit(W);
						mat_tput_log<<"\t"<<(noc.nwtile3D[s][j][i+1])->return_avg_tput(W);
				
						if(j != num_rows - 1) {
							mat_lat_pkt_log<<"\t"<<null<<"\t"<<null;
							mat_lat_flit_log<<"\t"<<null<<"\t"<<null;
							mat_tput_log<<"\t"<<null<<"\t"<<null;
			
						}
					}
					mat_lat_pkt_log<<endl;
					mat_lat_flit_log<<endl;
					mat_tput_log<<endl;
				}
			}
		}
		///added
		for(int s=0; s < num_slice; s++)
		{
			for(int j=0;j<num_cols; j++){
				for(int i=0;i<num_rows;i++){
					mat_power_log<<(noc.nwtile3D[s][i][j])->Pavg<<"\t";
				}
				mat_power_log<<endl;
			}	
		}
		/*added*/
		string power_filename = string("log/gnuplot/power");
		ofstream power_log;
		power_log.open(power_filename.c_str());
		if(!power_log.is_open())
			cout<<"Cannot open power"<<endl;
			/*end*/	

		// generate gnuplot logs
		for(int s=0; s < num_slice; s++)
		{
			for(int i = 0; i < num_cols; i++) {
				for(int j = 0; j < num_rows - 1; j++) {
					float x = (float)i;
					float y = (float)j + 0.2 + j;
					float null = 0.0;
					ylatency_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency(N)<<endl;
					ylatency_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency(N)<<endl;
					ylatency_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency(S)<<endl;
					ylatency_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency(S)<<endl;
					ylatency_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_log<<endl;
			
					ylatency_flit_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_flit_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency_flit(N)<<endl;
					ylatency_flit_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency_flit(N)<<endl;
					ylatency_flit_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_flit_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_flit_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency_flit(S)<<endl;
					ylatency_flit_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency_flit(S)<<endl;
					ylatency_flit_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_flit_log<<endl;
			
					ytput_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
					ytput_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_avg_tput(N)<<endl;
					ytput_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_avg_tput(N)<<endl;
					ytput_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
					ytput_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
					ytput_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_avg_tput(S)<<endl;
					ytput_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_avg_tput(S)<<endl;
					ytput_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
					ytput_log<<endl;
						
					y = (float)j + 0.8 + j;
			
					ylatency_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency(N)<<endl;
					ylatency_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency(N)<<endl;
					ylatency_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency(S)<<endl;
					ylatency_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency(S)<<endl;
					ylatency_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_log<<endl<<endl;
			
					ylatency_flit_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_flit_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency_flit(N)<<endl;
					ylatency_flit_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_latency_flit(N)<<endl;
					ylatency_flit_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_flit_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_flit_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency_flit(S)<<endl;
					ylatency_flit_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_latency_flit(S)<<endl;
					ylatency_flit_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
					ylatency_flit_log<<endl<<endl;
		
					ytput_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
					ytput_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_avg_tput(N)<<endl;
					ytput_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j+1][i])->return_avg_tput(N)<<endl;
					ytput_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
					ytput_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
					ytput_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_avg_tput(S)<<endl;
					ytput_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->return_avg_tput(S)<<endl;
					ytput_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
					ytput_log<<endl<<endl;
			
				}
			}
		}	
		/*added*/
		float k;
		for(int s=0; s < num_slice; s++)
		{
			for(int i = 0; i < num_cols; i++) {
				for(int j = 0; j < num_rows ; j++) {
					float x = (float)i;
					float y = (float)j + 0.2 + j;
					float null = 0.0;
					for(k=0.4;k<=.6;k=k+.01){
					power_log<<"\t"<<x+k<<"\t"<<y<<"\t"<<null<<endl;
					power_log<<"\t"<<(x +k)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->Pavg<<endl;
					}
					power_log<<endl;

					y = (float)j + 0.8 + j;
					for(k=.4;k<=.6;k=k+.01){
					power_log<<"\t"<<x+k<<"\t"<<y<<"\t"<<null<<endl;
					power_log<<"\t"<<(x + k)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->Pavg<<endl;
					}
					power_log<<endl<<endl;
					}
			}
		}
		/*end*/
		//added for LinkUsage
		//float k;
		char str_id[3];
	    	sprintf(str_id, "%d", id);		// id is read above from application.config
		string traffic_filename = string("config/traffic/tile-") + string(str_id);
	    	ifstream instream;
	    	instream.open(traffic_filename.c_str());
		int packet_size, num_flits;
	    	while (!instream.eof())
	    	{
			string field;
			instream >> field;
			if (field == "PKT_SIZE")  	// read packet size
			{
			    instream >> packet_size;
			}
		}
		num_flits = (int)((ceil)((float)(packet_size - HEAD_PAYLOAD)/(DATA_PAYLOAD)) + 1);
		for(int s=0; s < num_slice; s++){
			for(int i = 0; i < num_cols; i++) {
				for(int j = 0; j < num_rows ; j++) {
					float x = (float)i;
					float y = (float)j + 0.2 + j;
					float null = 0.0;
					for(k=0.4;k<=.6;k=k+.01){
						LinkUsage_log[s]<<"\t"<<x+k<<"\t"<<y<<"\t"<<null<<endl;
						LinkUsage_log[s]<<"\t"<<(x +k)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->LinkUsage/num_flits<<endl;
					}
					LinkUsage_log[s]<<endl;

					y = (float)j + 0.8 + j;
					for(k=.4;k<=.6;k=k+.01){
						LinkUsage_log[s]<<"\t"<<x+k<<"\t"<<y<<"\t"<<null<<endl;
						LinkUsage_log[s]<<"\t"<<(x + k)<<"\t"<<y<<"\t"<<(noc.nwtile3D[s][j][i])->LinkUsage/num_flits<<endl;
					}
					LinkUsage_log[s]<<endl<<endl;
				}
			}
		}
		//end of LinkUsage.

		for(int s=0; s < num_slice; s++)
		{
			for(int i = 0; i < num_rows; i++) {
				for(int j = 0; j < num_cols - 1; j++) {
					float x = (float)i + i;
					float y = (float)j + 0.2;

					float null = 0.0;
			
					xlatency_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile3D[s][i][j])->return_latency(E)<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile3D[s][i][j])->return_latency(E)<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_latency(W)<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_latency(W)<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
					xlatency_log<<endl;
			
					xlatency_flit_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile3D[s][i][j])->return_latency_flit(E)<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile3D[s][i][j])->return_latency_flit(E)<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_latency_flit(W)<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_latency_flit(W)<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
					xlatency_flit_log<<endl;
			
					xtput_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile3D[s][i][j])->return_avg_tput(E)<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile3D[s][i][j])->return_avg_tput(E)<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_avg_tput(W)<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_avg_tput(W)<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
					xtput_log<<endl;
			
					y = (float)j + 0.8;
			
					xlatency_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile3D[s][i][j])->return_latency(E)<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile3D[s][i][j])->return_latency(E)<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_latency(W)<<endl;		
					xlatency_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_latency(W)<<endl;
					xlatency_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
					xlatency_log<<endl<<endl;
			
					xlatency_flit_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile3D[s][i][j])->return_latency_flit(E)<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile3D[s][i][j])->return_latency_flit(E)<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_latency_flit(W)<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_latency_flit(W)<<endl;
					xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
					xlatency_flit_log<<endl<<endl;
			
					xtput_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile3D[s][i][j])->return_avg_tput(E)<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile3D[s][i][j])->return_avg_tput(E)<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_avg_tput(W)<<endl;		
					xtput_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile3D[s][i][j+1])->return_avg_tput(W)<<endl;
					xtput_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
					xtput_log<<endl<<endl;
			
				}
			}
		}
		// compute overall avg latency 
		ULL noc_total_latency = 0;
		ULL noc_total_flits = 0;
		ULL noc_total_packets = 0;
		float noc_total_throughput=0, temp;
		int num_destination=0;
		for(int s=0; s < num_slice; s++)
		{
			for(int i = 0; i < num_rows; i++) {
				for(int j = 0; j < num_cols; j++) {
					noc_total_latency += (noc.nwtile3D[s][i][j])->return_total_latency();
					noc_total_flits += (noc.nwtile3D[s][i][j])->return_total_flits();
					noc_total_packets += (noc.nwtile3D[s][i][j])->cal_no_of_pkts_per_tile();
					temp = (noc.nwtile3D[s][i][j])->return_total_throughput();
					noc_total_throughput+=  temp;
					if(temp > 0.00000)
						num_destination++;
				}
			}
		}	
		float noc_latency = (float)noc_total_latency / noc_total_flits;
		float noc_throughput=noc_total_throughput/num_tiles; ///num_destination;  //num_tiles; //float noc_throughput=noc_total_throughput/num_rows/num_cols;
		
	int noc_total_latency_GT = 0,noc_total_latency_BE = 0;
		int noc_total_flits_GT = 0,noc_total_flits_BE = 0;
		for(int s=0; s < num_slice; s++)
		{
			for(int i = 0; i < num_rows; i++) {
				for(int j = 0; j < num_cols; j++) {

					noc_total_latency_GT += (noc.nwtile3D[s][i][j])->return_total_latency_GT();
					if(max_latency_GT<(noc.nwtile3D[s][i][j])->max_latency_GT)
				            max_latency_GT=(noc.nwtile3D[s][i][j])->max_latency_GT;
					noc_total_flits_GT += (noc.nwtile3D[s][i][j])->return_total_flits_GT();
			
					noc_total_latency_BE += (noc.nwtile3D[s][i][j])->return_total_latency_BE();
					if(max_latency_BE<(noc.nwtile3D[s][i][j])->max_latency_BE)
				            max_latency_BE=(noc.nwtile3D[s][i][j])->max_latency_BE;
					noc_total_flits_BE += (noc.nwtile3D[s][i][j])->return_total_flits_BE();
				}
			}
		}
		float total_flits_gen= gengt+genbe;
		float total_flits_recvd = countGT+countBE;
		float noc_latency_GT = (float)noc_total_latency_GT / noc_total_flits_GT;

		float noc_latency_BE = (float)noc_total_latency_BE / noc_total_flits_BE;
	
		results_log<<"\nOverall average latency per channel (in clock cycles per flit) = "<<noc_latency<<endl;
		results_log<<"\nOverall average latency per channel(in clock cycles per packet) = "<<float(float(noc_total_latency)/noc_total_packets)<<endl;
		results_log<<"\nOverall total throughput is "<<noc_throughput<<endl;
		if(qos==1){
		results_log<<"\nOverall average latency per channel for GT traffic(in clock cycles per flit) = "<<noc_latency_GT<<endl;
		results_log<<"\nOverall average latency per channel for BE traffic(in clock cycles per flit) = "<<noc_latency_BE<<endl;
		}
		results_log<<"\nOverall average latency (in clock cycles per flit) = "<<((float)sum/count_total)<<endl;
		if(qos==1){
		results_log<<"\nOverall average latency for GT traffic(in clock cycles per flit) = "<<((float)sumGT/countGT)<<endl;
		results_log<<"\nOverall average latency for BE traffic(in clock cycles per flit) = "<<((float)sumBE/countBE)<<endl;
		results_log<<"\nTotal Flits Generated GT = "<<gengt<<endl;
		results_log<<"Total Flits Received GT = "<<countGT<<endl;
		results_log<<"\nTotal Flits Generated BE = "<<genbe<<endl;
		results_log<<"Total Flits Received BE = "<<countBE<<endl;
		}
		results_log<<"\nTotal Flits Generated = "<<total_flits_gen<<endl; cout<<"\nTotal Flits Generated = "<<total_flits_gen;
		results_log<<"Total Flits Received = "<<total_flits_recvd<<endl; cout<<"    Total Flits Received = "<<total_flits_recvd<<endl;

		/*added*/
		int id1;
		double Power;
		for(int s=0; s < num_slice; s++)
		{
			for(int i = 0; i < num_rows; i++) {
				for(int j = 0; j < num_cols; j++) {
					id1=(noc.nwtile3D[s][i][j])->tileID;	
					Power=((noc.nwtile3D[s][i][j])->Pavg);
					power_results_log<<id1<<"\t\t\t\t\t\t";
					power_results_log<<Power<<"\n";
			
				}
			}
		}
		double totalPower=0,totalLinkPower=0;
		for(int s=0; s < num_slice; s++)
		{
			for(int i = 0; i < num_rows; i++) {
				for(int j = 0; j < num_cols; j++) {
					id1=(noc.nwtile3D[s][i][j])->tileID;	
					totalPower+=(noc.nwtile3D[s][i][j])->Pavg;
					totalLinkPower+=(noc.nwtile3D[s][i][j])->Plink;
				}
			}
		}
		power_results_log<<endl<<endl<<"Total Netwok Power(In Watt):"<<totalPower;

        //added (Saving tile data in log/nirgam/tileDetail and used for power estimation using DSENT models)
		try{
        	system("mkdir log/nirgam/tileDetail");        	
        	}
        	catch(int n) {}
		try{
		system("rm -rf log/nirgam/tileDetail/*");
		}
		catch(int n){} 	        	
		
		
        	if(TOPO == MESH3D) {
		double buf_rl,buf_wl,c_tl,vc_reads,vc_writes,n_arbs,l_tl;	
        	for(int s=0; s < num_slice; s++) {
        		for (int i=0; i<num_rows; i++) {
			        for (int j=0; j<num_cols; j++) {
					ofstream tile_log;
					string results_file1;
			        	int id = (s*slice_size) + (i*num_cols) + j;
					std::stringstream ss;
					ss << id;
					string idNum = ss.str();
			        	results_file1 = string("log/nirgam/tileDetail/tile-") + idNum ;
					//cout<<results_file1<<endl;
					tile_log.open(results_file1.c_str());
					if(!results_log.is_open())
						cout<<"Cannot open "<<results_file1.c_str()<<endl;
					else{
						buf_rl=((double)(noc.nwtile3D[s][i][j])->return_buf_read());
		                		buf_wl=((double)(noc.nwtile3D[s][i][j])->return_buf_write());
			                	c_tl=((double)(noc.nwtile3D[s][i][j])->return_xbar_trav());
			                	vc_reads=((double)(noc.nwtile3D[s][i][j])->return_vc_reads());
			                	vc_writes=((double)(noc.nwtile3D[s][i][j])->return_vc_writes());
			                	n_arbs=((double)(noc.nwtile3D[s][i][j])->return_n_arb());
			                	l_tl=((double)(noc.nwtile3D[s][i][j])->return_link_trav());
							
						tile_log<<"NumberBufferReads = "<<buf_rl<<endl;
						tile_log<<"NumberBufferWrites = "<<buf_wl<<endl;
						tile_log<<"NumberCrossbarTraversals = "<<c_tl<<endl;
						tile_log<<"NumberVCReads = "<<vc_reads<<endl;
						tile_log<<"NumberVCWrites = "<<vc_writes<<endl;
						tile_log<<"NumberArb = "<<n_arbs<<endl;
						tile_log<<"NumberLinkTraversals = "<<l_tl<<endl;
					}

			        }
			    }
        	}   
        } 
        //end (Dsent)
		/*end*/
	/*	}
		max_latency=(max_latency_GT < max_latency_BE) ? max_latency_BE:max_latency_GT;

		results_log<<"\nMax latency(in clock cycles per flit) = "<<max_latency<<endl; 

		results_log<<"\nMax latency in case of GT traffic(in clock cycles per flit) = "<<max_latency_GT<<endl; 
		results_log<<"\nMax latency in case of BE traffic(in clock cycles per flit) = "<<max_latency_BE<<endl; 
	  */      
	
	//results_log<<"\nOverall average latency (in clock cycles per flit) = "<<noc_latency<<"\n Total flits:"<<noc_total_flits<<endl;
	

	///////////////////////******************JNNJJN***********/////////////////////////////////////////////////
	/////////////////////// 	EDITED 			/////////////////////////////////////////////////
	ifstream jitter_GTresults;
	ifstream jitter_BEresults;
	char pair[10];
	string fname;
	int c1,c2;
	double avg_GT_jitter=0,avg_BE_jitter=0;
	int GT_time_max=0,BE_time_max=0;
	int GT_time_min=99999,BE_time_min=99999;
	if(qos==1){
	results_log<<"\n\n GT Jitter";

	c1=0;
	for(int i=0;i<(num_rows*num_cols*num_slice);i++)
	{
		for(int j=0;j<(num_rows*num_cols*num_slice);j++)		
			{
			GT_time_max=0;
			GT_time_min=99999;
			if(i==j)
				continue;
			sprintf(pair,"%d-%d",i,j);
			fname=string("jitter/GT/pair-")+string(pair);
			jitter_GTresults.open(fname.c_str());
			if(jitter_GTresults)
				{
				c1++;
				int temp_GT;
				while(jitter_GTresults)
					{
					jitter_GTresults>>temp_GT;
					if(temp_GT>GT_time_max)
						{
						GT_time_max=temp_GT;
						}
					if(temp_GT<GT_time_min)
						{
						GT_time_min=temp_GT;
						}
				
					}
				avg_GT_jitter+=GT_time_max-GT_time_min;
				results_log<<"\njitter for "<<pair<<"(in clockcycles) = "<<(GT_time_max-GT_time_min);
				}
			jitter_GTresults.close();
			}
		}
		avg_GT_jitter/=c1;
	results_log<<"\nAverage GT Jitter " <<avg_GT_jitter<<"\n\n";
	results_log<<"\n\n BE Jitter";
	c2=0;
	for(int i=0;i<(num_rows*num_cols*num_slice);i++)
	{
		for(int j=0;j<(num_rows*num_cols*num_slice);j++)		
			{
			BE_time_max=0;
			BE_time_min=99999;
			if(i==j)
				continue;
			sprintf(pair,"%d-%d",i,j);
			fname=string("jitter/BE/pair-")+string(pair);
			jitter_BEresults.open(fname.c_str());
			if(jitter_BEresults)
				{
				c2++;
				int temp_BE;
				while(jitter_BEresults)
					{
			
					jitter_BEresults>>temp_BE;
					if(temp_BE>BE_time_max)
						{
						BE_time_max=temp_BE;
						}
					if(temp_BE<BE_time_min)
						{
						BE_time_min=temp_BE;
						}
				
					}
				avg_BE_jitter+=BE_time_max-BE_time_min;
				results_log<<"\njitter for "<<pair<<"(in clockcycles) = "<<(BE_time_max-BE_time_min);
				}
			jitter_BEresults.close();
	
			}
		}
		avg_BE_jitter/=c2;
	results_log<<"\nAverage BE Jitter " <<avg_BE_jitter;
	results_log<<"\nOverall average jitter "<<((avg_BE_jitter*c2)+(avg_GT_jitter*c1))/(c1+c2);
	}
	///////////////////////******************JNNJJN***********/////////////////////////////////////////////////


		// close log files
		//sc_close_vcd_trace_file(tracefile);
		/*added*/
		power_log.close();
		/*end*/
		eventlog.close();
		ylatency_log.close();
		xlatency_log.close();
		ylatency_flit_log.close();
		xlatency_flit_log.close();
		ytput_log.close();
		xtput_log.close();
		mat_lat_pkt_log.close();
		mat_lat_flit_log.close();
		mat_tput_log.close();
		////added
		mat_power_log.close();


		results_log.close();
		qresults_log.close();
		GTresults_log.close();
		BEresults_log.close();
		
		latency_throughput_result1.close();
		
		// generate gnuplot graphs
		system("mkdir results/graphs");
		system("gnuplot gnuplot/results_3D.gnu");
		cmd = string("mv results/graphs/* ") + DIRNAME + string("/graphs");
		system(cmd.c_str());
		system("rm -rf results/graphs");
	
		cout<<"---------------------------------------------------------------------------"<<endl;
		cout<<"Simulation complete!"<<endl;
		string str_event_log = string("nirgam/log/nirgam/event.log");
		cout<<"Event Log created in "<<str_event_log<<endl;
		string result_dir = string("nirgam/") + DIRNAME;
		cout<<"Simulation results and gnuplot graphs stored in "<<result_dir<<endl;
		string matlab_logs = string("nirgam/log/matlab");
		cout<<"Input data files for matlab created in "<<matlab_logs<<endl;
		string matlab_script = string("nirgam/matlab");
		cout<<"Run \"plot_graphs\" from directory "<<matlab_script<<" to generate graphs in matlab"<<endl;
		cout<<"Exit NIRGAM!"<<endl;
		cout<<"---------------------------------------------------------------------------"<<endl;
	}   // end of code for 3D NoC 
	else
	{
		// generate matlab log
		float null = 0.0;
		for(int i = 0; i < num_cols; i++) {
			for(int j = 0; j < num_rows; j++) {
				mat_lat_pkt_log<<"\t"<<null;
				mat_lat_flit_log<<"\t"<<null;
				mat_tput_log<<"\t"<<null;
			
				if(j != num_rows - 1) {
					mat_lat_pkt_log<<"\t"<<(noc.nwtile[j][i])->return_latency(S);
					mat_lat_flit_log<<"\t"<<(noc.nwtile[j][i])->return_latency_flit(S);
					mat_tput_log<<"\t"<<(noc.nwtile[j][i])->return_avg_tput(S);
				
					mat_lat_pkt_log<<"\t"<<(noc.nwtile[j+1][i])->return_latency(N);
					mat_lat_flit_log<<"\t"<<(noc.nwtile[j+1][i])->return_latency_flit(N);
					mat_tput_log<<"\t"<<(noc.nwtile[j+1][i])->return_avg_tput(N);
				}
			}
			mat_lat_pkt_log<<endl;
			mat_lat_flit_log<<endl;
			mat_tput_log<<endl;
			
		
			if(i != num_cols - 1) {
				for(int j = 0; j < num_rows; j++) {
					mat_lat_pkt_log<<"\t"<<(noc.nwtile[j][i])->return_latency(E);
					mat_lat_flit_log<<"\t"<<(noc.nwtile[j][i])->return_latency_flit(E);
					mat_tput_log<<"\t"<<(noc.nwtile[j][i])->return_avg_tput(E);
				
					if(j != num_rows - 1) {
						mat_lat_pkt_log<<"\t"<<null<<"\t"<<null;
						mat_lat_flit_log<<"\t"<<null<<"\t"<<null;
						mat_tput_log<<"\t"<<null<<"\t"<<null;
			
					}
				}
				mat_lat_pkt_log<<endl;
				mat_lat_flit_log<<endl;
				mat_tput_log<<endl;
					
			
				for(int j = 0; j < num_rows; j++) {
					mat_lat_pkt_log<<"\t"<<(noc.nwtile[j][i+1])->return_latency(W);
					mat_lat_flit_log<<"\t"<<(noc.nwtile[j][i+1])->return_latency_flit(W);
					mat_tput_log<<"\t"<<(noc.nwtile[j][i+1])->return_avg_tput(W);
				
					if(j != num_rows - 1) {
						mat_lat_pkt_log<<"\t"<<null<<"\t"<<null;
						mat_lat_flit_log<<"\t"<<null<<"\t"<<null;
						mat_tput_log<<"\t"<<null<<"\t"<<null;
			
					}
				}
				mat_lat_pkt_log<<endl;
				mat_lat_flit_log<<endl;
				mat_tput_log<<endl;
			}
		}

		///added
		for(int j=0;j<num_cols; j++){
			for(int i=0;i<num_rows;i++){
				mat_power_log<<(noc.nwtile[i][j])->Pavg<<"\t";
			}
			mat_power_log<<endl;
		}	
	

		/*added*/
		string power_filename = string("log/gnuplot/power");
		ofstream power_log;
		power_log.open(power_filename.c_str());
		if(!power_log.is_open())
			cout<<"Cannot open power"<<endl;
			/*end*/	

		// generate gnuplot logs
		for(int i = 0; i < num_cols; i++) {
			for(int j = 0; j < num_rows - 1; j++) {
				float x = (float)i;
				float y = (float)j + 0.2 + j;
				float null = 0.0;
				ylatency_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_latency(N)<<endl;
				ylatency_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_latency(N)<<endl;
				ylatency_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_latency(S)<<endl;
				ylatency_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_latency(S)<<endl;
				ylatency_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_log<<endl;
			
				ylatency_flit_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_flit_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_latency_flit(N)<<endl;
				ylatency_flit_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_latency_flit(N)<<endl;
				ylatency_flit_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_flit_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_flit_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_latency_flit(S)<<endl;
				ylatency_flit_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_latency_flit(S)<<endl;
				ylatency_flit_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_flit_log<<endl;
			
				ytput_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
				ytput_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_avg_tput(N)<<endl;
				ytput_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_avg_tput(N)<<endl;
				ytput_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
				ytput_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
				ytput_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_avg_tput(S)<<endl;
				ytput_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_avg_tput(S)<<endl;
				ytput_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
				ytput_log<<endl;
						
				y = (float)j + 0.8 + j;
			
				ylatency_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_latency(N)<<endl;
				ylatency_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_latency(N)<<endl;
				ylatency_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_latency(S)<<endl;
				ylatency_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_latency(S)<<endl;
				ylatency_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_log<<endl<<endl;
			
				ylatency_flit_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_flit_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_latency_flit(N)<<endl;
				ylatency_flit_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_latency_flit(N)<<endl;
				ylatency_flit_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_flit_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_flit_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_latency_flit(S)<<endl;
				ylatency_flit_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_latency_flit(S)<<endl;
				ylatency_flit_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
				ylatency_flit_log<<endl<<endl;
		
				ytput_log<<"\t"<<x+0.1<<"\t"<<y<<"\t"<<null<<endl;
				ytput_log<<"\t"<<(x + 0.1)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_avg_tput(N)<<endl;
				ytput_log<<"\t"<<(x + 0.3)<<"\t"<<y<<"\t"<<(noc.nwtile[j+1][i])->return_avg_tput(N)<<endl;
				ytput_log<<"\t"<<(x+0.3)<<"\t"<<y<<"\t"<<null<<endl;
				ytput_log<<"\t"<<(x+0.7)<<"\t"<<y<<"\t"<<null<<endl;
				ytput_log<<"\t"<<(x + 0.7)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_avg_tput(S)<<endl;
				ytput_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->return_avg_tput(S)<<endl;
				ytput_log<<"\t"<<(x+0.9)<<"\t"<<y<<"\t"<<null<<endl;
				ytput_log<<endl<<endl;
			
			}
		}
	
		/*added*/
		float k;
		for(int i = 0; i < num_cols; i++) {
			for(int j = 0; j < num_rows ; j++) {
				float x = (float)i;
				float y = (float)j + 0.2 + j;
				float null = 0.0;
				for(k=0.4;k<=.6;k=k+.01){
				power_log<<"\t"<<x+k<<"\t"<<y<<"\t"<<null<<endl;
				power_log<<"\t"<<(x +k)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->Pavg<<endl;
				}
				power_log<<endl;

				y = (float)j + 0.8 + j;
				for(k=.4;k<=.6;k=k+.01){
				power_log<<"\t"<<x+k<<"\t"<<y<<"\t"<<null<<endl;
				power_log<<"\t"<<(x + k)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->Pavg<<endl;
				}
				power_log<<endl<<endl;
				}
		}
		/*end*/
		//added for LinkUsage
		//float k;
		
		char str_id[3];
	    	sprintf(str_id, "%d", id);		// id is read above from application.config
		string traffic_filename = string("config/traffic/tile-") + string(str_id);
	    	ifstream instream;
	    	instream.open(traffic_filename.c_str());
		int packet_size, num_flits;
	    	while (!instream.eof())
	    	{
			string field;
			instream >> field;
			if (field == "PKT_SIZE")  	// read packet size
			{
			    instream >> packet_size;
			}
		}
		num_flits = (int)((ceil)((float)(packet_size - HEAD_PAYLOAD)/(DATA_PAYLOAD)) + 1);
		for(int i = 0; i < num_cols; i++) 
		{
			for(int j = 0; j < num_rows ; j++) 
			{
				float x = (float)i;
				float y = (float)j + 0.4 + j;
				float null = 0.0;
				for(k=0.4;k<=.6;k=k+.01)
				{
					LinkUsage_log[0]<<"\t"<<x+k<<"\t"<<y<<"\t"<<null<<endl;
					LinkUsage_log[0]<<"\t"<<(x +k)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->LinkUsage/num_flits<<endl;
				}
				LinkUsage_log[0]<<endl;

				y = (float)j + .8 + j;
				for(k=.4;k<=.6;k=k+.01)
				{
					LinkUsage_log[0]<<"\t"<<x+k<<"\t"<<y<<"\t"<<null<<endl;
					LinkUsage_log[0]<<"\t"<<(x + k)<<"\t"<<y<<"\t"<<(noc.nwtile[j][i])->LinkUsage/num_flits<<endl;
				}
				LinkUsage_log[0]<<endl<<endl;
				
			}
		}
		//end of LinkUsage.

		for(int i = 0; i < num_rows; i++) {
			for(int j = 0; j < num_cols - 1; j++) {
				float x = (float)i + i;
				float y = (float)j + 0.2;
				float null = 0.0;
			
				xlatency_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile[i][j])->return_latency(E)<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile[i][j])->return_latency(E)<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile[i][j+1])->return_latency(W)<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile[i][j+1])->return_latency(W)<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
				xlatency_log<<endl;
			
				xlatency_flit_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile[i][j])->return_latency_flit(E)<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile[i][j])->return_latency_flit(E)<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile[i][j+1])->return_latency_flit(W)<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile[i][j+1])->return_latency_flit(W)<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
				xlatency_flit_log<<endl;
			
				xtput_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile[i][j])->return_avg_tput(E)<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile[i][j])->return_avg_tput(E)<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile[i][j+1])->return_avg_tput(W)<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile[i][j+1])->return_avg_tput(W)<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
				xtput_log<<endl;
			
				y = (float)j + 0.8;
			
				xlatency_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile[i][j])->return_latency(E)<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile[i][j])->return_latency(E)<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile[i][j+1])->return_latency(W)<<endl;		
				xlatency_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile[i][j+1])->return_latency(W)<<endl;
				xlatency_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
				xlatency_log<<endl<<endl;
			
				xlatency_flit_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile[i][j])->return_latency_flit(E)<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile[i][j])->return_latency_flit(E)<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile[i][j+1])->return_latency_flit(W)<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile[i][j+1])->return_latency_flit(W)<<endl;
				xlatency_flit_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
				xlatency_flit_log<<endl<<endl;
			
				xtput_log<<"\t"<<y<<"\t"<<x+0.1<<"\t"<<null<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x + 0.1)<<"\t"<<(noc.nwtile[i][j])->return_avg_tput(E)<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x + 0.3)<<"\t"<<(noc.nwtile[i][j])->return_avg_tput(E)<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x+0.3)<<"\t"<<null<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x+0.7)<<"\t"<<null<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x + 0.7)<<"\t"<<(noc.nwtile[i][j+1])->return_avg_tput(W)<<endl;		
				xtput_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<(noc.nwtile[i][j+1])->return_avg_tput(W)<<endl;
				xtput_log<<"\t"<<y<<"\t"<<(x+0.9)<<"\t"<<null<<endl;
				xtput_log<<endl<<endl;
			
			}
		}
	
		// compute overall avg latency 
		ULL noc_total_latency = 0;
		ULL noc_total_flits = 0;
		ULL noc_total_packets = 0;
		float noc_total_throughput=0, temp;
		int num_destination=0;
		for(int i = 0; i < num_rows; i++) {
			for(int j = 0; j < num_cols; j++) {
				noc_total_latency += (noc.nwtile[i][j])->return_total_latency();
				noc_total_flits += (noc.nwtile[i][j])->return_total_flits();
				noc_total_packets += (noc.nwtile[i][j])->cal_no_of_pkts_per_tile();
				temp =(noc.nwtile[i][j])->return_total_throughput();
				// cout<<"test data"<<temp<<endl;
				noc_total_throughput+= temp;
				if(temp > 0.00000)
					num_destination++;
			}
		}
	
		float noc_latency = (float)noc_total_latency / noc_total_flits;
		float noc_throughput=noc_total_throughput/num_tiles; ///num_destination;  // num_rows/num_cols;
		
	int noc_total_latency_GT = 0,noc_total_latency_BE = 0;
		int noc_total_flits_GT = 0,noc_total_flits_BE = 0;
		for(int i = 0; i < num_rows; i++) {
			for(int j = 0; j < num_cols; j++) {

				noc_total_latency_GT += (noc.nwtile[i][j])->return_total_latency_GT();
				if(max_latency_GT<(noc.nwtile[i][j])->max_latency_GT)
		                    max_latency_GT=(noc.nwtile[i][j])->max_latency_GT;
				noc_total_flits_GT += (noc.nwtile[i][j])->return_total_flits_GT();
			
				noc_total_latency_BE += (noc.nwtile[i][j])->return_total_latency_BE();
				if(max_latency_BE<(noc.nwtile[i][j])->max_latency_BE)
		                    max_latency_BE=(noc.nwtile[i][j])->max_latency_BE;
				noc_total_flits_BE += (noc.nwtile[i][j])->return_total_flits_BE();
			}
		}
			float total_flits_gen= gengt+genbe;
			float total_flits_recvd = countGT+countBE;
		float noc_latency_GT = (float)noc_total_latency_GT / noc_total_flits_GT;

		float noc_latency_BE = (float)noc_total_latency_BE / noc_total_flits_BE;
	




		results_log<<"\nOverall average latency per channel (in clock cycles per flit) = "<<noc_latency<<endl;
		results_log<<"\nOverall average latency per channel(in clock cycles per packet) = "<<float(float(noc_total_latency)/noc_total_packets)<<endl;
		results_log<<"\nOverall total throughput is "<<noc_throughput<<endl;
		if(qos==1){
		results_log<<"\nOverall average latency per channel for GT traffic(in clock cycles per flit) = "<<noc_latency_GT<<endl;
		results_log<<"\nOverall average latency per channel for BE traffic(in clock cycles per flit) = "<<noc_latency_BE<<endl;
		}
		results_log<<"\nOverall average latency (in clock cycles per flit) = "<<((float)sum/count_total)<<endl;
		if(qos==1){
		results_log<<"\nOverall average latency for GT traffic(in clock cycles per flit) = "<<((float)sumGT/countGT)<<endl;
		results_log<<"\nOverall average latency for BE traffic(in clock cycles per flit) = "<<((float)sumBE/countBE)<<endl;
		results_log<<"\nTotal Flits Generated GT = "<<gengt<<endl;
		results_log<<"Total Flits Received GT = "<<countGT<<endl;
		results_log<<"\nTotal Flits Generated BE = "<<genbe<<endl;
		results_log<<"Total Flits Received BE = "<<countBE<<endl;
		}
		results_log<<"\nTotal Flits Generated = "<<total_flits_gen<<endl; cout<<"\nTotal Flits Generated = "<<total_flits_gen;
		results_log<<"Total Flits Received = "<<total_flits_recvd<<endl; cout<<"    Total Flits Received = "<<total_flits_recvd<<endl;
		/*added*/
		int id1;
		double Power;
		for(int i = 0; i < num_rows; i++) {
			for(int j = 0; j < num_cols; j++) {
				id1=(noc.nwtile[i][j])->tileID;	
				Power=((noc.nwtile[i][j])->Pavg);
				power_results_log<<id1<<"\t\t\t\t\t\t";
				power_results_log<<Power<<"\n";
			
			}
		}
		double totalPower=0,totalLinkPower=0;
		for(int i = 0; i < num_rows; i++) {
			for(int j = 0; j < num_cols; j++) {
				id1=(noc.nwtile[i][j])->tileID;	
				totalPower+=(noc.nwtile[i][j])->Pavg;
				totalLinkPower+=(noc.nwtile[i][j])->Plink;
			}
		}
		power_results_log<<endl<<endl<<"Total Netwok Power(In Watt):"<<totalPower;
		/*end*/
	/*	}
		max_latency=(max_latency_GT < max_latency_BE) ? max_latency_BE:max_latency_GT;

		results_log<<"\nMax latency(in clock cycles per flit) = "<<max_latency<<endl; 
		results_log<<"\nMax latency in case of GT traffic(in clock cycles per flit) = "<<max_latency_GT<<endl; 
		results_log<<"\nMax latency in case of BE traffic(in clock cycles per flit) = "<<max_latency_BE<<endl; 
	  */      
	
	//results_log<<"\nOverall average latency (in clock cycles per flit) = "<<noc_latency<<"\n Total flits:"<<noc_total_flits<<endl;
	

	///////////////////////******************JNNJJN***********/////////////////////////////////////////////////
	/////////////////////// 	EDITED 			/////////////////////////////////////////////////
	ifstream jitter_GTresults;
	ifstream jitter_BEresults;
	char pair[10];
	string fname;
	int c1,c2;
	double avg_GT_jitter=0,avg_BE_jitter=0;
	int GT_time_max=0,BE_time_max=0;
	int GT_time_min=99999,BE_time_min=99999;
	if(qos==1){
	results_log<<"\n\n GT Jitter";

	c1=0;
	for(int i=0;i<(num_rows*num_cols);i++)
		{
		for(int j=0;j<(num_rows*num_cols);j++)		
			{
			GT_time_max=0;
			GT_time_min=99999;
			if(i==j)
				continue;
			sprintf(pair,"%d-%d",i,j);
			fname=string("jitter/GT/pair-")+string(pair);
			jitter_GTresults.open(fname.c_str());
			if(jitter_GTresults)
				{
				c1++;
				int temp_GT;
				while(jitter_GTresults)
					{
					jitter_GTresults>>temp_GT;
					if(temp_GT>GT_time_max)
						{
						GT_time_max=temp_GT;
						}
					if(temp_GT<GT_time_min)
						{
						GT_time_min=temp_GT;
						}
				
					}
				avg_GT_jitter+=GT_time_max-GT_time_min;
				results_log<<"\njitter for "<<pair<<"(in clockcycles) = "<<(GT_time_max-GT_time_min);
				}
			jitter_GTresults.close();
			}
		}
		avg_GT_jitter/=c1;
	results_log<<"\nAverage GT Jitter " <<avg_GT_jitter<<"\n\n";
	results_log<<"\n\n BE Jitter";
	c2=0;
	for(int i=0;i<(num_rows*num_cols);i++)
		{
		for(int j=0;j<(num_rows*num_cols);j++)		
			{
			BE_time_max=0;
			BE_time_min=99999;
			if(i==j)
				continue;
			sprintf(pair,"%d-%d",i,j);
			fname=string("jitter/BE/pair-")+string(pair);
			jitter_BEresults.open(fname.c_str());
			if(jitter_BEresults)
				{
				c2++;
				int temp_BE;
				while(jitter_BEresults)
					{
			
					jitter_BEresults>>temp_BE;
					if(temp_BE>BE_time_max)
						{
						BE_time_max=temp_BE;
						}
					if(temp_BE<BE_time_min)
						{
						BE_time_min=temp_BE;
						}
				
					}
				avg_BE_jitter+=BE_time_max-BE_time_min;
				results_log<<"\njitter for "<<pair<<"(in clockcycles) = "<<(BE_time_max-BE_time_min);
				}
			jitter_BEresults.close();
	
			}
		}
		avg_BE_jitter/=c2;
	results_log<<"\nAverage BE Jitter " <<avg_BE_jitter;
	results_log<<"\nOverall average jitter "<<((avg_BE_jitter*c2)+(avg_GT_jitter*c1))/(c1+c2);
	}
	///////////////////////******************JNNJJN***********/////////////////////////////////////////////////


		// close log files
		//sc_close_vcd_trace_file(tracefile);
		/*added*/
		power_log.close();
		/*end*/
		eventlog.close();
		ylatency_log.close();
		xlatency_log.close();
		ylatency_flit_log.close();
		xlatency_flit_log.close();
		ytput_log.close();
		xtput_log.close();
		mat_lat_pkt_log.close();
		mat_lat_flit_log.close();
		mat_tput_log.close();
		////added
		mat_power_log.close();


		results_log.close();
		qresults_log.close();
		GTresults_log.close();
		BEresults_log.close();
		
		latency_throughput_result1.close();
		
		// generate gnuplot graphs
		system("mkdir results/graphs");
		system("gnuplot gnuplot/results.gnu");
		cmd = string("mv results/graphs/* ") + DIRNAME + string("/graphs");
		system(cmd.c_str());
		system("rm -rf results/graphs");

	
		cout<<"---------------------------------------------------------------------------"<<endl;
		cout<<"Simulation complete!"<<endl;
		string str_event_log = string("nirgam/log/nirgam/event.log");
		cout<<"Event Log created in "<<str_event_log<<endl;
		string result_dir = string("nirgam/") + DIRNAME;
		cout<<"Simulation results and gnuplot graphs stored in "<<result_dir<<endl;
		string matlab_logs = string("nirgam/log/matlab");
		cout<<"Input data files for matlab created in "<<matlab_logs<<endl;
		string matlab_script = string("nirgam/matlab");
		cout<<"Run \"plot_graphs\" from directory "<<matlab_script<<" to generate graphs in matlab"<<endl;
		cout<<"Exit NIRGAM!"<<endl;
		cout<<"---------------------------------------------------------------------------"<<endl;
		return 0;
	}
}
void itoa(int i, char str[])
{
	char num[999];
	int j, k=0;
	while(i>0)
	{
		j=i%10;
		num[k++]=(char)(j+48);
		i/=10;
	}	
	for(j=0, k--; k>=0; k--, j++)
		str[j]=num[k];
	str[j]='\0';
}
