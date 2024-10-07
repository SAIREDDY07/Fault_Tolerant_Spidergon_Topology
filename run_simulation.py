#!/usr/local/bin/env python3
import os
import subprocess
import re
import json


RT_ALGO = ['PROM', 'XY', 'WF', 'NL']
# RT_ALGO = ['PROM', 'XY']
TOPO = ["MESH","SPIDERGON"]
Traffic_Pattern = ["TRANSPOSE", "BIT_SHUFFLE"]

result_Data = {}
config_path = "config/nirgam.config"
results_dir = "results"  # Adjust as per your directory structure

def run_simulation():
    for topo in TOPO:
        modify_topo(config_path, topo)
        result_Data[topo] = {}
        for pattern in Traffic_Pattern:
            tile_path = os.path.abspath("config/traffic/tile-0")
            edit_pattern_parameter(tile_path, pattern)
            result_Data[topo][pattern] = {}

            for RT in RT_ALGO:
                modify_rt_algo(config_path, RT, topo)
                result_path = f'{results_dir}/{topo}_4X4_{RT}'
                print("This is the result Path->  " + result_path)
                
                pir = []
                latency = []
                throughput = []
                power = []
                for load_value in range(1, 101, 1):
                    edit_load_parameter(tile_path, load_value)
                    script_path = "config/traffic/changeTile.py"
                    os.system(f"python3 {script_path}")

                    # Example usage
                    # run_command(".././nirgam")
                    os.system('./nirgam')

                    global_latency, global_throughput = extract_metrics(result_path + '/stats/sim_results')
                    power_values = extract_power_metrics(result_path + '/stats/power_sim_results')
                    pir.append(load_value)
                    latency.append(global_latency)
                    throughput.append(global_throughput)
                    power.append(power_values)
                
                latency = [value if value is not None else 0 for value in latency]
                power = [value if value is not None else 0 for value in power]
                throughput = [value if value is not None else 0 for value in throughput]

                latency.sort()
                power.sort()
                throughput.sort()

                result_Data[topo][pattern][RT] = {
                    "PIR": pir,
                    "Global_Latency": latency,
                    "Global_ThroughPut": throughput,
                    "Power": power
                }
    write_result_data()



def run_command(command):
    try:
        # subprocess.run waits for the command to complete
        result = subprocess.run(command, shell=True, check=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(result.stdout)  # Print standard output if needed
        print(result.stderr)  # Print standard error if needed
    except subprocess.CalledProcessError as e:
        print(f"Command '{e.cmd}' returned non-zero exit status {e.returncode}.")
        print(e.stderr)  # Print standard error in case of failure

def write_result_data():
    try:
        with open('result_data.txt', 'w') as file:
            file.write("Results:\n")
            for topo, topo_data in result_Data.items():
                file.write(f"\nTopology: {topo}\n")
                for pattern, pattern_data in topo_data.items():
                    file.write(f"\nTraffic Pattern: {pattern}\n")
                    
                    for RT, RT_data in pattern_data.items():
                        file.write(f"\nRouting Algorithm: {RT}\n")
                        
                        pir = ", ".join(map(str, RT_data["PIR"]))
                        latency = ", ".join(map(str, RT_data["Global_Latency"]))
                        throughput = ", ".join(map(str, RT_data["Global_ThroughPut"]))
                        power = ", ".join(map(str, RT_data["Power"]))
                        file.write(f'{{"PIR": [{pir}]\n,"Power": [{power}]\n,"Global_Latency": [{latency}]\n, "Global_ThroughPut": [{throughput}]\n}}\n')
                    
                    file.write("\n")
            print("Results written to result_data.txt.")
    except Exception as e:
        print(f"An error occurred while writing result data: {e}")

def extract_metrics(result_path):
    try:
        with open(result_path, 'r') as file:
            content = file.read()

        throughput_pattern = r'Overall total throughput\s*is\s*([\d.]+)'
        latency_pattern = r'Overall average latency\s*\(in clock cycles per flit\)\s*=\s*([\d.]+)'

        throughput = float(re.search(throughput_pattern, content).group(1))
        latency = float(re.search(latency_pattern, content).group(1))

        return throughput, latency
    except FileNotFoundError:
        print(f"File '{result_path}' not found.")
    except Exception as e:
        print(f"An error occurred while extracting metrics: {e}")
        return None, None

def extract_power_metrics(result_path):
    try:
        with open(result_path, 'r') as file:
            content = file.read()

        power_pattern = r"Total Netwok Power\(In Watt\):([\d.]+)"
        # throughput_pattern = r'Overall total throughput\s*is\s*([\d.]+)'
        # latency_pattern = r'Overall average latency\s*\(in clock cycles per flit\)\s*=\s*([\d.]+)'

        power = float(re.search(power_pattern, content).group(1))
        # latency = float(re.search(latency_pattern, content).group(1))

        return power
    except FileNotFoundError:
        print(f"File '{result_path}' not found.")
    except Exception as e:
        print(f"An error occurred while extracting power metrics: {e}")
        return None
def modify_topo(config_path, topo):
    try:
        with open(config_path, 'r') as file:
            lines = file.readlines()

        with open(config_path, 'w') as file:
            for line in lines:
                if line.startswith('TOPOLOGY'):
                    file.write(f'TOPOLOGY {topo}\n')
                else:
                    file.write(line)

        print(f"Modified TOPOLOGY to {topo} in {config_path}.")
    except FileNotFoundError:
        print(f"File '{config_path}' not found.")
    except Exception as e:
        print(f"An error occurred while modifying TOPOLOGY: {e}")


def modify_rt_algo(config_path, rt_algo_value,topo):
    try:
        with open(config_path, 'r') as file:
            lines = file.readlines()

        with open(config_path, 'w') as file:
            for line in lines:
                if line.startswith('RT_ALGO'):
                    file.write(f'RT_ALGO {rt_algo_value}\n')
                elif line.startswith('DIRNAME'):
                    file.write( f'DIRNAME {topo}_4X4_{rt_algo_value}\n')
                else:
                    file.write(line)
        

        print(f"Modified RT_ALGO to {rt_algo_value} in {config_path}.")
    except FileNotFoundError:
        print(f"File '{config_path}' not found.")
    except Exception as e:
        print(f"An error occurred while modifying RT_ALGO: {e}")


def edit_pattern_parameter(file_path, new_pattern):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

        load_param_index = None
        for i, line in enumerate(lines):
            if line.startswith("DESTINATION"):
                load_param_index = i
                break

        if load_param_index is None:
            raise ValueError("DESTINATION parameter not found in the file.")

        # Replace the LOAD line with the new value
        lines[load_param_index] = f"DESTINATION {new_pattern}\n"

        with open(file_path, 'w') as file:
            file.writelines(lines)

        print(f"Updated DESTINATION to {new_pattern} in {file_path}.")
    
    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
    except ValueError as ve:
        print(ve)
    except Exception as e:
        print(f"An error occurred: {e}")


def edit_load_parameter(file_path, new_load_value):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

        load_param_index = None
        for i, line in enumerate(lines):
            if line.startswith("LOAD"):
                load_param_index = i
                break

        if load_param_index is None:
            raise ValueError("LOAD parameter not found in the file.")

        # Replace the LOAD line with the new value
        lines[load_param_index] = f"LOAD {new_load_value}\n"

        with open(file_path, 'w') as file:
            file.writelines(lines)

        print(f"Updated LOAD to {new_load_value} in {file_path}.")

    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
    except ValueError as ve:
        print(ve)
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == '__main__':
    run_simulation()

