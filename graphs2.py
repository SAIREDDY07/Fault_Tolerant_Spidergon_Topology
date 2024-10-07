import matplotlib.pyplot as plt
import json
import re
import os

def read_results(file_path):
    results = {}
    with open(file_path, 'r') as file:
        content = file.read()

    topologies = re.split(r'\nTopology: ', content)
    
    for topo_data in topologies[1:]:
        topo_name, rest = topo_data.split('\n', 1)
        patterns = re.split(r'\nTraffic Pattern: ', rest)

        results[topo_name] = {}
        for pattern_data in patterns[1:]:
            pattern_name, algos_data = pattern_data.split('\n', 1)
            algos = re.split(r'\nRouting Algorithm: ', algos_data)

            results[topo_name][pattern_name] = {}
            for algo_data in algos[1:]:
                algo_name, json_data = algo_data.split('\n', 1)
                
                # Replace 'None' with 'null' to make it valid JSON
                json_data_cleaned = json_data.replace('None', 'null')
                
                # Attempt to load JSON data and catch errors
                try:
                    json_data_cleaned = re.sub(r'(?<=[^,{])\s*(\n|\r|\t)+\s*', '', json_data_cleaned).strip()
                    algo_results = json.loads(json_data_cleaned)
                except json.JSONDecodeError as e:
                    print(f"Error decoding JSON for {topo_name} - {pattern_name} - {algo_name}: {e}")
                    print(f"Problematic JSON data: {json_data_cleaned}")
                    continue
                
                results[topo_name][pattern_name][algo_name] = algo_results

    return results

def plot_results(results, output_dir='comparison_graphs'):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for topo, pattern_data in results.items():
        for pattern, algos in pattern_data.items():
            plt.figure(figsize=(18, 6))

            # Latency vs PIR
            plt.subplot(1, 3, 1)
            for algo, data in algos.items():
                pir = data.get("PIR", [])
                latency = [v for v in data.get("Global_Latency", []) if v is not None]
                if pir and latency:
                    sorted_latency = sorted(latency)
                    plt.plot(pir[:len(sorted_latency)], sorted_latency, label=f'{algo} - Latency')

            plt.title(f'Latency vs PIR for {pattern} in {topo}')
            plt.xlabel('Load')
            plt.ylabel('Latency')
            plt.legend()
            plt.grid(True)

            # Throughput vs PIR
            plt.subplot(1, 3, 2)
            for algo, data in algos.items():
                pir = data.get("PIR", [])
                throughput = [v for v in data.get("Global_ThroughPut", []) if v is not None]
                if pir and throughput:
                    sorted_throughput = sorted(throughput)
                    plt.plot(pir[:len(sorted_throughput)], sorted_throughput, label=f'{algo} - Throughput')

            plt.title(f'Throughput vs Load for {pattern} in {topo}')
            plt.xlabel('PIR')
            plt.ylabel('Throughput')
            plt.legend()
            plt.grid(True)

            # Power vs PIR
            plt.subplot(1, 3, 3)
            for algo, data in algos.items():
                pir = data.get("PIR", [])
                power = [v for v in data.get("Power", []) if v is not None]
                if pir and power:
                    sorted_power = sorted(power)
                    plt.plot(pir[:len(sorted_power)], sorted_power, label=f'{algo} - Power')

            plt.title(f'Power vs Load for {pattern} in {topo}')
            plt.xlabel('Load')
            plt.ylabel('Power (W)')
            plt.legend()
            plt.grid(True)

            plt.tight_layout()
            plt.savefig(f'{output_dir}/{topo}_{pattern}.png')
            plt.close()

def plot_power_results(results, output_dir='power_graphs'):
    mesh_dir = os.path.join(output_dir, 'MESH')
    spidergon_dir = os.path.join(output_dir, 'SPIDERGON')

    if not os.path.exists(mesh_dir):
        os.makedirs(mesh_dir)
    if not os.path.exists(spidergon_dir):
        os.makedirs(spidergon_dir)

    for topo, pattern_data in results.items():
        for pattern, algos in pattern_data.items():
            if 'MESH' in topo:
                plt.figure(figsize=(5, 5))
                for algo, data in algos.items():
                    pir = data.get("PIR", [])
                    power = [v for v in data.get("Power", []) if v is not None]
                    if pir and power:
                        if algo == 'PROM':
                            algo = 'OE'
                        sorted_power = sorted(power)
                        plt.plot(pir[:len(sorted_power)], sorted_power, label=f'{algo} - Power')

                plt.title(f'Power vs Load for {pattern} in {topo}')
                plt.xlabel('Load (%)')
                plt.ylabel('Power (W)')
                plt.legend()
                plt.grid(True)
                plt.tight_layout()
                plt.savefig(f'{mesh_dir}/{topo}_{pattern}_Power.png')
                plt.close()

            elif 'SPIDERGON' in topo:
                plt.figure(figsize=(5, 5))
                for algo, data in algos.items():
                    pir = data.get("PIR", [])
                    power = [v for v in data.get("Power", []) if v is not None]
                    if pir and power:
                        if algo == 'PROM':
                            algo = 'OE'
                        sorted_power = sorted(power)
                        plt.plot(pir[:len(sorted_power)], sorted_power, label=f'{algo} - Power')

                plt.title(f'Power vs Load for {pattern} in {topo}')
                plt.xlabel('Load (%)')
                plt.ylabel('Power (W)')
                plt.legend()
                plt.grid(True)
                plt.tight_layout()
                plt.savefig(f'{spidergon_dir}/{topo}_{pattern}_Power.png')
                plt.close()

def main():
    results_file = 'result_data.txt'
    results = read_results(results_file)
    plot_results(results)
    plot_power_results(results)

if __name__ == "__main__":
    main()

