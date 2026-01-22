"""
Docstring for create_comparison_plots
Namn: Simon Lindqvist: siln22@student.bth.se, Abdalrahman Mohammed: abmm22@student.bth.se
"""


import json
import os
from typing import Dict, List
import matplotlib.pyplot as plt
from datetime import datetime


def load_experiment_results(results_dir: str = "results") -> List[Dict]:
    """Load all JSON experiment results from the results directory."""
    experiments = []
    
    if not os.path.exists(results_dir):
        print(f"Results directory '{results_dir}' not found.")
        return experiments
    
    for filename in os.listdir(results_dir):
        if filename.endswith('.json'):
            filepath = os.path.join(results_dir, filename)
            with open(filepath, 'r') as f:
                experiments.append(json.load(f))
    
    return experiments


def create_comparison_plot(experiments: List[Dict], partition_type: str, aggregation: str, defense: str = "none", output_dir: str = "results"):
    """Create a comparison plot for a specific partition type, aggregation strategy, and defense."""
    # Filter experiments for this configuration
    filtered = [
        exp for exp in experiments 
        if exp['experiment_config']['partition_type'] == partition_type 
        and exp['experiment_config']['aggregation'] == aggregation
        and exp['experiment_config'].get('defense', 'none') == defense
    ]
    
    if not filtered:
        print(f"No experiments found for {partition_type} + {aggregation} + defense={defense}")
        return
    
    # Sort by number of attackers
    filtered.sort(key=lambda x: x['experiment_config']['num_attackers'])
    
    # Create figure with 2x3 grid (5 metrics)
    fig = plt.figure(figsize=(18, 10))
    
    colors = ['#2E86AB', '#E63946', '#F77F00']  # Blue, Red, Orange for 0, 1, 2 attackers
    markers = ['o', 's', '^']
    
    # Plot each metric
    metrics_info = [
        ('accuracies', 'Accuracy', 1),
        ('f1_scores', 'F1 Score (Weighted)', 2),
        ('kappa_scores', "Cohen's Kappa", 3),
        ('roc_auc_scores', 'ROC-AUC (Weighted OvR)', 4),
        ('losses', 'Loss', 5)
    ]
    
    for metric_key, metric_label, subplot_idx in metrics_info:
        ax = plt.subplot(2, 3, subplot_idx)
        
        for idx, exp in enumerate(filtered):
            num_attackers = exp['experiment_config']['num_attackers']
            rounds = exp['metrics']['rounds']
            values = exp['metrics'][metric_key]
            
            label = f"{num_attackers} malicious client{'s' if num_attackers != 1 else ''}"
            color = colors[num_attackers] if num_attackers < len(colors) else colors[-1]
            marker = markers[num_attackers] if num_attackers < len(markers) else markers[-1]
            
            ax.plot(rounds, values, marker=marker, linewidth=2, label=label, 
                   color=color, markersize=4, alpha=0.8)
        
        ax.set_xlabel('Round', fontsize=11)
        ax.set_ylabel(metric_label, fontsize=11)
        ax.set_title(f'{metric_label} over Rounds', fontsize=11, fontweight='bold')
        ax.legend(loc='best', fontsize=9)
        ax.grid(True, alpha=0.3)
    
    # Add overall title
    fig.suptitle(
        f'{partition_type.upper()} | {aggregation.upper()} | Defense: {defense.upper()}',
        fontsize=16,
        fontweight='bold',
        y=0.98
    )
    
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    
    # Save plot
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"comparison_{partition_type}_{aggregation}_defense_{defense}_{timestamp}.png"
    filepath = os.path.join(output_dir, filename)
    plt.savefig(filepath, dpi=150, bbox_inches='tight')
    plt.close()
    
    print(f"Saved comparison plot: {filepath}")


def main():
    """Generate comparison plots for all partition type, aggregation, and defense combinations."""
    print("Loading experiment results...")
    experiments = load_experiment_results()
    
    if not experiments:
        print("No experiment results found. Please run experiments first.")
        return
    
    print(f"Found {len(experiments)} experiment(s)")
    
    # Get unique defenses from experiments
    defenses = set(exp['experiment_config'].get('defense', 'none') for exp in experiments)
    print(f"Defenses found: {sorted(defenses)}")
    
    # Generate comparison plots for each combination
    combinations = [
        ('iid', 'fedavg'),
        ('iid', 'fedprox'),
        ('non-iid', 'fedavg'),
        ('non-iid', 'fedprox')
    ]
    
    print("\nGenerating comparison plots...")
    for defense in sorted(defenses):
        print(f"\nDefense: {defense}")
        for partition_type, aggregation in combinations:
            create_comparison_plot(experiments, partition_type, aggregation, defense)
    
    print("\nAll comparison plots generated successfully!")


if __name__ == "__main__":
    main()
