"""
Docstring for run_all
Namn: Simon Lindqvist: siln22@student.bth.se, Abdalrahman Mohammed: abmm22@student.bth.se
"""



import subprocess
import sys
import time
from pathlib import Path
from create_comparison_plots import load_experiment_results, create_comparison_plot

PYTHON = sys.executable


def run_single_experiment(partition_type: str, num_attackers: int, aggregation: str, defense: str = "none", num_clients: int = 5, num_rounds: int = 50, batch_size: int = 128, proximal_mu: float = 0.1, clip_threshold: float = 5.0, server_address: str = "127.0.0.1:8080"):
    print(
        f"\n=== Experiment: partition={partition_type}, "
        f"agg={aggregation}, defense={defense}, attackers={num_attackers} ==="
    )

    # Start server
    server_cmd = [
        PYTHON,
        "server.py",
        "--server-address",
        server_address,
        "--partition-type",
        partition_type,
        "--aggregation",
        aggregation,
        "--defense",
        defense,
        "--num-clients",
        str(num_clients),
        "--num-rounds",
        str(num_rounds),
        "--num-attackers",
        str(num_attackers),
        "--batch-size",
        str(batch_size),
        "--proximal-mu",
        str(proximal_mu),
        "--clip-threshold",
        str(clip_threshold),
    ]
    server_proc = subprocess.Popen(server_cmd)

    # Give server a moment to start
    time.sleep(5)

    # Start clients
    client_procs = []
    for cid in range(num_clients):
        client_cmd = [
            PYTHON,
            "client.py",
            "--server-address",
            server_address,
            "--cid",
            str(cid),
            "--num-clients",
            str(num_clients),
            "--num-attackers",
            str(num_attackers),
            "--partition-type",
            partition_type,
            "--batch-size",
            str(batch_size),
        ]
        p = subprocess.Popen(client_cmd)
        client_procs.append(p)

    # Wait for server to finish (after num_rounds)
    server_proc.wait()

    # Ensure all clients exit
    for p in client_procs:
        p.wait()

    print("=== Experiment finished ===\n")


def main():
    Path("results").mkdir(exist_ok=True)

    partition_types = ["iid", "non-iid"]
    malicious_options = [0, 1, 2]  # number of malicious clients (0,1,2)
    aggregations = ["fedavg", "fedprox"]
    defenses = ["none", "median", "clip"]

    num_clients = 5
    num_rounds = 50
    batch_size = 32
    proximal_mu = 0.1
    clip_threshold = 5.0

    total_experiments = (
        len(partition_types) * len(malicious_options) * 
        len(aggregations) * len(defenses)
    )
    exp_idx = 0

    for partition_type in partition_types:
        for num_attackers in malicious_options:
            for aggregation in aggregations:
                for defense in defenses:
                    exp_idx += 1
                    print(f"Running experiment {exp_idx}/{total_experiments}")
                    run_single_experiment(
                        partition_type=partition_type,
                        num_attackers=num_attackers,
                        aggregation=aggregation,
                        defense=defense,
                        num_clients=num_clients,
                        num_rounds=num_rounds,
                        batch_size=batch_size,
                        proximal_mu=proximal_mu,
                        clip_threshold=clip_threshold,
                    )

    print("\nAll experiments completed. Generating comparison plots...")
    
    # Generate comparison plots
    experiments = load_experiment_results()
    combinations = [
        ('iid', 'fedavg'),
        ('iid', 'fedprox'),
        ('non-iid', 'fedavg'),
        ('non-iid', 'fedprox')
    ]
    
    # Generate plots for each defense type
    for defense in defenses:
        print(f"\nGenerating plots for defense: {defense}")
        for partition_type, aggregation in combinations:
            create_comparison_plot(experiments, partition_type, aggregation, defense)
    
    print("\nAll done! Results and comparison plots are in the 'results' folder.")


if __name__ == "__main__":
    main()
