"""
Docstring for server
Namn: Simon Lindqvist: siln22@student.bth.se, Abdalrahman Mohammed: abmm22@student.bth.se
"""



from collections import OrderedDict
from typing import List, Tuple, Dict
import argparse
import os
import json
from datetime import datetime
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision.transforms as transforms
from datasets.utils.logging import disable_progress_bar
from torch.utils.data import DataLoader
from sklearn.metrics import f1_score, cohen_kappa_score, roc_auc_score
from sklearn.preprocessing import label_binarize
import flwr as fl
from flwr.common import Metrics, ndarrays_to_parameters
from flwr.server.strategy import FedAvg, FedProx
from flwr_datasets import FederatedDataset
from strategies import FedMedian, FedClip

# Global device
DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"[SERVER] Using device: {DEVICE}")
disable_progress_bar()


# Model
class Model(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)
        self.pool = nn.MaxPool2d(2, 2)
        self.dropout = nn.Dropout(0.25)
        self.fc1 = nn.Linear(64 * 8 * 8, 256)
        self.fc2 = nn.Linear(256, 10)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        x = self.pool(F.relu(self.conv1(x)))  # 32x16x16
        x = self.pool(F.relu(self.conv2(x)))  # 64x8x8
        x = x.view(-1, 64 * 8 * 8)
        x = self.dropout(F.relu(self.fc1(x)))
        x = self.fc2(x)
        return x


# Parameter helpers
def get_parameters(net: nn.Module) -> List[np.ndarray]:
    return [val.cpu().numpy() for _, val in net.state_dict().items()]


def set_parameters(net: nn.Module, parameters: List[np.ndarray]) -> None:
    params_dict = zip(net.state_dict().keys(), parameters)
    state_dict = OrderedDict({k: torch.tensor(v) for k, v in params_dict})
    net.load_state_dict(state_dict, strict=True)


# Evaluation
def test(net: nn.Module, testloader: DataLoader):
    criterion = nn.CrossEntropyLoss()
    net.eval()

    running_loss = 0.0
    correct = 0
    total = 0
    all_predictions = []
    all_labels = []
    all_probs = []

    with torch.no_grad():
        for batch in testloader:
            images = batch["img"].to(DEVICE)
            labels = batch["label"].to(DEVICE)
            outputs = net(images)
            loss = criterion(outputs, labels)

            running_loss += loss.item() * labels.size(0)
            probs = F.softmax(outputs, dim=1)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
            
            all_predictions.extend(predicted.cpu().numpy())
            all_labels.extend(labels.cpu().numpy())
            all_probs.extend(probs.cpu().numpy())

    epoch_loss = running_loss / total
    epoch_acc = correct / total
    return epoch_loss, epoch_acc, np.array(all_predictions), np.array(all_labels), np.array(all_probs)


class MetricsTracker:
    def __init__(self):
        self.rounds: List[int] = []
        self.losses: List[float] = []
        self.accuracies: List[float] = []
        self.f1_scores: List[float] = []
        self.kappa_scores: List[float] = []
        self.roc_auc_scores: List[float] = []

    def add_metrics(self, round_num: int, loss: float, accuracy: float, f1: float, kappa: float, roc_auc: float):
        self.rounds.append(round_num)
        self.losses.append(loss)
        self.accuracies.append(accuracy)
        self.f1_scores.append(f1)
        self.kappa_scores.append(kappa)
        self.roc_auc_scores.append(roc_auc)
    
    def to_dict(self):
        return {
            "rounds": self.rounds,
            "losses": self.losses,
            "accuracies": self.accuracies,
            "f1_scores": self.f1_scores,
            "kappa_scores": self.kappa_scores,
            "roc_auc_scores": self.roc_auc_scores
        }


def gen_evaluate_fn(testloader: DataLoader, metrics_tracker: MetricsTracker):
    def evaluate(server_round: int, parameters_ndarrays, config: Dict):
        net = Model().to(DEVICE)
        set_parameters(net, parameters_ndarrays)
        loss, accuracy, predictions, labels, probs = test(net, testloader)
        
        # Calculate additional metrics
        f1 = f1_score(labels, predictions, average='weighted')
        kappa = cohen_kappa_score(labels, predictions)
        
        # ROC-AUC for multiclass (one-vs-rest)
        labels_binarized = label_binarize(labels, classes=np.arange(10))
        roc_auc = roc_auc_score(labels_binarized, probs, average='weighted', multi_class='ovr')
        
        metrics_tracker.add_metrics(server_round, loss, accuracy, f1, kappa, roc_auc)
        return float(loss), {
            "accuracy": float(accuracy),
            "f1": float(f1),
            "kappa": float(kappa),
            "roc_auc": float(roc_auc)
        }

    return evaluate


# Plotting
def save_experiment_results(metrics_tracker: MetricsTracker, partition_type: str, num_attackers: int, aggregation: str, num_clients: int, num_rounds: int, defense: str = "none", output_dir: str = "results"):
    """Save experiment results to JSON and return the filepath."""
    os.makedirs(output_dir, exist_ok=True)

    malicious_str = ("none" if num_attackers == 0 else f"first_{num_attackers}_clients")
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"{partition_type}_{aggregation}_defense_{defense}_malicious_{malicious_str}_{timestamp}"
    
    # Save metrics to JSON
    json_data = {
        "experiment_config": {
            "partition_type": partition_type,
            "aggregation": aggregation,
            "defense": defense,
            "num_attackers": num_attackers,
            "num_clients": num_clients,
            "num_rounds": num_rounds,
            "timestamp": timestamp
        },
        "metrics": metrics_tracker.to_dict(),
        "final_results": {
            "accuracy": metrics_tracker.accuracies[-1] if metrics_tracker.accuracies else 0.0,
            "f1_score": metrics_tracker.f1_scores[-1] if metrics_tracker.f1_scores else 0.0,
            "kappa": metrics_tracker.kappa_scores[-1] if metrics_tracker.kappa_scores else 0.0,
            "roc_auc": metrics_tracker.roc_auc_scores[-1] if metrics_tracker.roc_auc_scores else 0.0,
            "loss": metrics_tracker.losses[-1] if metrics_tracker.losses else 0.0
        }
    }
    
    json_filepath = os.path.join(output_dir, f"{filename}.json")
    with open(json_filepath, 'w') as f:
        json.dump(json_data, f, indent=2)
    
    print(f"[SERVER] Saved metrics to: {json_filepath}")
    print(
        f"[SERVER] Final metrics:\n"
        f"  Accuracy: {metrics_tracker.accuracies[-1]:.4f}\n"
        f"  F1 Score: {metrics_tracker.f1_scores[-1]:.4f}\n"
        f"  Kappa: {metrics_tracker.kappa_scores[-1]:.4f}\n"
        f"  ROC-AUC: {metrics_tracker.roc_auc_scores[-1]:.4f}\n"
        f"  Loss: {metrics_tracker.losses[-1]:.4f}"
    )
    
    return json_filepath


# Main
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--server-address", type=str, default="0.0.0.0:8080")
    parser.add_argument("--partition-type", type=str, choices=["iid", "non-iid"], required=True)
    parser.add_argument("--aggregation", type=str, choices=["fedavg", "fedprox"], required=True)
    parser.add_argument("--defense", type=str, choices=["none", "median", "clip"], default="none")
    parser.add_argument("--clip-threshold", type=float, default=5.0, help="Clipping threshold for clip defense")
    parser.add_argument("--num-clients", type=int, default=5)
    parser.add_argument("--num-rounds", type=int, default=50)
    parser.add_argument("--num-attackers", type=int, default=0)
    parser.add_argument("--batch-size", type=int, default=32)
    parser.add_argument("--proximal-mu", type=float, default=0.1)
    args = parser.parse_args()

    print("\n" + "=" * 80)
    print(
        f"[SERVER] Running: partition={args.partition_type} | "
        f"aggregation={args.aggregation} | defense={args.defense} | attackers={args.num_attackers}"
    )
    print("=" * 80)
    print(f"[SERVER] Device: {DEVICE}")
    print(f"[SERVER] Clients: {args.num_clients}, Rounds: {args.num_rounds}")

    # CIFAR-10 test set (same transforms as clients)
    pytorch_transforms = transforms.Compose(
        [
            transforms.ToTensor(),
            transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
        ]
    )

    def apply_transforms(batch):
        batch["img"] = [pytorch_transforms(img) for img in batch["img"]]
        return batch

    fds = FederatedDataset(
        dataset="uoft-cs/cifar10",
        partitioners={"train": args.num_clients}  # simple IID partitioner for train
    )
    testset = fds.load_split("test").with_transform(apply_transforms)
    testloader = DataLoader(testset, batch_size=args.batch_size)

    # Initial global model
    net = Model().to(DEVICE)
    initial_parameters = ndarrays_to_parameters(get_parameters(net))

    # Metrics tracker and evaluate_fn
    metrics_tracker = MetricsTracker()
    evaluate_fn = gen_evaluate_fn(testloader, metrics_tracker)

    # Strategy selection with defense
    strategy_kwargs = {
        "fraction_fit": 1.0,
        "fraction_evaluate": 1.0,
        "min_fit_clients": args.num_clients,
        "min_evaluate_clients": max(1, args.num_clients // 2),
        "min_available_clients": args.num_clients,
        "evaluate_fn": evaluate_fn,
        "initial_parameters": initial_parameters,
    }

    if args.defense == "median":
        strategy = FedMedian(**strategy_kwargs)
        print(f"[SERVER] Using FedMedian (coordinate-wise median aggregation)")
    elif args.defense == "clip":
        strategy_kwargs["clip_threshold"] = args.clip_threshold
        strategy = FedClip(**strategy_kwargs)
        print(f"[SERVER] Using FedClip (clipping threshold={args.clip_threshold})")
    else:  # no defense
        if args.aggregation == "fedavg":
            strategy = FedAvg(**strategy_kwargs)
            print(f"[SERVER] Using FedAvg (no defense)")
        else:
            strategy_kwargs["proximal_mu"] = args.proximal_mu
            strategy = FedProx(**strategy_kwargs)
            print(f"[SERVER] Using FedProx (proximal_mu={args.proximal_mu}, no defense)")

    # Start Flower server (blocks until training is done)
    fl.server.start_server(
        server_address=args.server_address,
        config=fl.server.ServerConfig(num_rounds=args.num_rounds),
        strategy=strategy,
    )

    # After training, save results
    save_experiment_results(
        metrics_tracker=metrics_tracker,
        partition_type=args.partition_type,
        num_attackers=args.num_attackers,
        aggregation=args.aggregation,
        num_clients=args.num_clients,
        num_rounds=args.num_rounds,
        defense=args.defense,
    )
    print("[SERVER] Training complete.")


if __name__ == "__main__":
    main()
