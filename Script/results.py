import os
import subprocess
import re

RT_ALGO = ['PROM', 'XY', 'WF', 'NL']
TOPO = ["MESH", "SPIDERGON"]
Traffic_Pattern = ["TRANSPOSE", "RANDOM"]

result_Data = {}
config_path = "../config/nirgam.config"
results_dir = "../results"  # Adjust as per your directory structure

def run_simulation():
    for topo in TOPO:
        modify_topo(config_path, topo)
        result_Data[topo] = {}
        for pattern in Traffic_Pattern:
            tile_path = os.path.abspath("../config/traffic/tile-0.txt")
            edit_pattern_parameter(tile_path, pattern)
            result_Data[topo][pattern] = {}

            for RT in RT_ALGO:
                modify_rt_algo(config_path, RT)
                result_path = f'{results_dir}/{topo}_4X4_{RT}/stats/sim_results.txt'
                
                if not os.path.exists(os.path.dirname(result_path)):
                    os.makedirs(os.path.dirname(result_path))
                
                for load_value in range(1, 101):
                    edit_load_parameter(tile_path, load_value)
                    script_path = "../config/traffic/changeTile.py"
                    os.system(f"python3 {script_path}")

                    # Example usage
                    #run_command(".././nirgam")
                    os.system('/mnt/hgfs/Shared Folder/#Nirgam_Spidergon/./nirgam')

                    # //TODO: EXTRACT overall latency and overall throughput from the text file and add that to the dictionary
                    global_latency, global_throughput = extract_metrics(result_path)
                    result_Data[topo][pattern][RT] = {
                        "PIR": load_value,
                        "Global_Latency": global_latency,
                        "Global_ThroughPut": global_throughput
                    }


def run_command(command):
    try:
        # subprocess.run waits for the command to complete
        result = subprocess.run(command, shell=True, check=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(result.stdout)  # Print standard output if needed
        print(result.stderr)  # Print standard error if needed
    except subprocess.CalledProcessError as e:
        print(f"Command '{e.cmd}' returned non-zero exit status {e.returncode}.")
        print(e.stderr)  # Print standard error in case of failure


def extract_metrics(result_path):
    try:
        with open(result_path, 'r') as file:
            content = file.read()

        throughput_pattern = r'Overall total throughput\s*=\s*([\d.]+)'
        latency_pattern = r'Overall average latency\s*\(in clock cycles per flit\)\s*=\s*([\d.]+)'

        throughput = float(re.search(throughput_pattern, content).group(1))
        latency = float(re.search(latency_pattern, content).group(1))

        return throughput, latency
    except FileNotFoundError:
        print(f"File '{result_path}' not found.")
    except Exception as e:
        print(f"An error occurred while extracting metrics: {e}")
        return None, None


def modify_topo(config_path, topo):
    try:
        with open(config_path, 'r') as file:
            lines = file.readlines()

        with open(config_path, 'w') as file:
            for line in lines:
                if line.startswith('TOPOLOGY'):
                    file.write(f'TOPOLOGY {topo}\n')
                elif line.startswith('DIRNAME'):
                    file.write(f'DIRNAME {topo}_4X4 ')
                else:
                    file.write(line)

        print(f"Modified TOPOLOGY to {topo} in {config_path}.")
    except FileNotFoundError:
        print(f"File '{config_path}' not found.")
    except Exception as e:
        print(f"An error occurred while modifying TOPOLOGY: {e}")


def modify_rt_algo(config_path, rt_algo_value):
    try:
        with open(config_path, 'r') as file:
            lines = file.readlines()

        with open(config_path, 'w') as file:
            for line in lines:
                if line.startswith('RT_ALGO'):
                    file.write(f'RT_ALGO {rt_algo_value}\n')
                elif line.startswith('DIRNAME'):
                    file.write(line + f'{rt_algo_value}')
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
            raise ValueError("LOAD parameter not found in the file.")

        # Replace the LOAD line with the new value
        lines[load_param_index] = f"LOAD {new_pattern}\n"

        with open(file_path, 'w') as file:
            file.writelines(lines)

        print(f"Updated LOAD to {new_pattern} in {file_path}.")
    
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
