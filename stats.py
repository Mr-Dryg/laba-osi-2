import os
import subprocess
import numpy as np
import random
import matplotlib.pyplot as plt

def create_matrix_file(filename, rows=1000, cols=1000):
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    
    with open(filename, 'w') as f:
        f.write(f"{rows} {cols}\n")
        for i in range(rows):
            row = []
            for j in range(cols):
                random_num = random.uniform(0, 100)
                row.append(f"{random_num:.6f}")
            f.write(" ".join(row) + "\n")

def parse_execution_time(stdout: str):
    # num_threads = 1: abs_time = 14.0820; threads_time = 6.8481
    key_1 = 'abs_time = '
    ind_1 = stdout.index(key_1) + len(key_1)
    # key_2 = 'threads_time = '
    # ind_2 = stdout.index(key_2) + len(key_2)
    abs_time = float(stdout[ind_1:stdout.index(';')])
    # threads_time = float(stdout[ind_2:])
    return abs_time

def measure_execution_time(num_starts, programm_file, k, matrix_file, iterations, erosion_res_file, dilation_res_file):
    try:        
        execution_times = []
        for _ in range(num_starts):
            result = subprocess.run(
                [programm_file, str(k), matrix_file, str(iterations), erosion_res_file, dilation_res_file],
                capture_output=True,
                text=True,
                check=True
            )
            
            t = parse_execution_time(result.stdout)
            execution_times.append(t)
            
            print(result.stdout, end='')
            
        return np.median(execution_times)
        
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при выполнении программы с k={k}: {e}")
        return None
    except FileNotFoundError:
        print(f"Ошибка: программа {programm_file} не найдена!")
        return None

def calculate_speedup_and_efficiency(execution_times):
    speedups = []
    efficiencies = []
    if not execution_times or execution_times[0] is None:
        return speedups, efficiencies
    
    T1 = execution_times[0]
    for k, time_value in enumerate(execution_times, start=1):
        speedup = T1 / time_value
        efficiency = speedup / k
            
        speedups.append(speedup)
        efficiencies.append(efficiency)
    
    return speedups, efficiencies

def plot_speedup(k_values, speedups):
    fig, ax = plt.subplots()
    
    ax.plot(k_values, speedups, 'bo-', linewidth=2, markersize=6, label='Общее время')
    ax.plot(k_values, k_values, 'k--', linewidth=1, label='Идеальное ускорение')
    ax.set_xlabel('Количество потоков (k)', fontsize=12)
    ax.set_ylabel('Ускорение', fontsize=12)
    ax.set_title('Зависимость ускорения от количества потоков', fontsize=14)
    ax.grid(True, alpha=0.3)
    ax.legend()
    ax.set_xticks(k_values)
    
    plt.tight_layout()
    os.makedirs('plots', exist_ok=True)
    plt.savefig('plots/speedup.png', dpi=300, bbox_inches='tight')
    plt.close()

def plot_efficiency(k_values, efficiencies):
    fig, ax = plt.subplots()
    
    ax.plot(k_values, efficiencies, 'bo-', linewidth=2, markersize=6, label='Общее время')
    ax.set_xlabel('Количество потоков (k)', fontsize=12)
    ax.set_ylabel('Эффективность', fontsize=12)
    ax.set_title('Зависимость эффективности от количества потоков', fontsize=14)
    ax.grid(True, alpha=0.3)
    ax.legend()
    ax.set_xticks(k_values)
    
    plt.tight_layout()
    os.makedirs('plots', exist_ok=True)
    plt.savefig('plots/efficiency.png', dpi=300, bbox_inches='tight')
    plt.close()

def plot_execution_time(k_values, execution_times):
    fig, ax = plt.subplots()
    
    ax.plot(k_values, execution_times, 'bo-', linewidth=2, markersize=6, label='Общее время')
    ax.set_xlabel('Количество потоков (k)', fontsize=12)
    ax.set_ylabel('Время выполнения (секунды)', fontsize=12)
    ax.set_title('Время выполнения программы', fontsize=14)
    ax.grid(True, alpha=0.3)
    ax.legend()
    ax.set_xticks(k_values)
    
    plt.tight_layout()
    os.makedirs('plots', exist_ok=True)
    plt.savefig('plots/execution_time.png', dpi=300, bbox_inches='tight')
    plt.close()

def plot_karp_flatt(k_values, speedups):
    def karp_flatt(p, s):
        if p == 1:
            return 0.0
        return (1.0 / s - 1.0 / p) / (1.0 - 1.0 / p)

    k_values_plot = k_values[1:]
    speedups_plot = speedups[1:]
    fig, ax = plt.subplots()
    
    ax.plot(k_values_plot, [karp_flatt(p, s) for p, s in zip(k_values_plot, speedups_plot)], 
            'bo-', linewidth=2, markersize=6)
    ax.set_xlabel('Количество потоков (k)', fontsize=12)
    ax.set_ylabel('Показатель распараллеливания кода', fontsize=12)
    ax.set_title('Метрика Карпа-Флатта', fontsize=14)
    ax.grid(True, alpha=0.3)
    ax.set_xticks(k_values_plot)
    
    plt.tight_layout()
    os.makedirs('plots', exist_ok=True)
    plt.savefig('plots/karp_flatt.png', dpi=300, bbox_inches='tight')
    plt.close()

if __name__ == "__main__":
    num_starts = 3
    programm_file = 'build/main'
    matrix_file = 'build/resources/matrix.txt'
    res_directory = 'build/res'
    erosion_res_file = f'{res_directory}/erosion_res.txt'
    dilation_res_file = f'{res_directory}/dilation_res.txt'
    k_values = list(range(1, 17))
    matrix_size = 200
    iterations = 120
    
    create_matrix_file(matrix_file, matrix_size, matrix_size)
    
    if not os.path.exists(programm_file):
        print(f"Ошибка: программа {programm_file} не найдена в текущей директории!")
    else:
        execution_times = []
        
        for k in k_values:
            time_val = measure_execution_time(num_starts, programm_file, k, matrix_file, iterations, erosion_res_file, dilation_res_file)
            execution_times.append(time_val)
        
        speedups, efficiencies = calculate_speedup_and_efficiency(execution_times)
        
        plot_speedup(k_values, speedups)
        plot_efficiency(k_values, efficiencies)
        plot_execution_time(k_values, execution_times)
        plot_karp_flatt(k_values, speedups)