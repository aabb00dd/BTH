"""
Docstring for client
Namn: Simon Lindqvist: siln22@student.bth.se, Abdalrahman Mohammed: abmm22@student.bth.se
"""


from collections import OrderedDict
from typing import List, Tuple, Optional
import argparse

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
from flwr.client import NumPyClient
from flwr_datasets import FederatedDataset
from flwr_datasets.partitioner import DirichletPartitioner

# Global device
DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
disable_progress_bar()


# Model
class Model(nn.Module):
    def __init__(self) -> None:
        super().__init__()
        self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)
        self.pool = nn.MaxPool2d(2, 2)
        self.dropout = nn.Dropout(0.25)
        self.fc1 = nn.Linear(64 * 8 * 8, 256)
        self.fc2 = nn.Linear(256, 10)

    def forward(self, x: torch.Tensor):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = x.view(-1, 64 * 8 * 8)
        x = self.dropout(F.relu(self.fc1(x)))
        x = self.fc2(x)
        return x


# Dataset / loading
def get_federated_dataset(num_clients: int, non_iid: bool):
    if non_iid:
        partitioner = DirichletPartitioner(
            num_partitions=num_clients, partition_by="label", alpha=0.5
        )
        fds = FederatedDataset(dataset="uoft-cs/cifar10", partitioners={"train": partitioner})
    else:
        fds = FederatedDataset(dataset="uoft-cs/cifar10", partitioners={"train": num_clients})
    return fds


def load_datasets(fds: FederatedDataset, partition_id: int, batch_size: int):
    partition = fds.load_partition(partition_id, split="train")
    partition_train_test = partition.train_test_split(test_size=0.2, seed=42)

    pytorch_transforms = transforms.Compose(
        [
            transforms.ToTensor(),
            transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
        ]
    )

    def apply_transforms(batch):
        batch["img"] = [pytorch_transforms(img) for img in batch["img"]]
        return batch

    partition_train_test = partition_train_test.with_transform(apply_transforms)
    trainloader = DataLoader(
        partition_train_test["train"], batch_size=batch_size, shuffle=True
    )
    valloader = DataLoader(partition_train_test["test"], batch_size=batch_size)

    return trainloader, valloader


# Parameter helpers
def get_parameters(net: nn.Module) -> List[np.ndarray]:
    return [val.cpu().numpy() for _, val in net.state_dict().items()]


def set_parameters(net: nn.Module, parameters: List[np.ndarray]) -> None:
    params_dict = zip(net.state_dict().keys(), parameters)
    state_dict = OrderedDict({k: torch.tensor(v) for k, v in params_dict})
    net.load_state_dict(state_dict, strict=True)


# Label flipping (attack)
def flip_labels(labels: torch.Tensor, num_classes: int = 10) -> torch.Tensor:
    return (labels + 1) % num_classes


# Training / evaluation
def train_one_epoch(net: nn.Module,trainloader: DataLoader, proximal_mu: float = 0.0, global_params: Optional[List[torch.Tensor]] = None, poisoned: bool = False):
    criterion = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(net.parameters())
    net.train()

    running_loss = 0.0
    correct = 0
    total = 0

    for batch in trainloader:
        images = batch["img"].to(DEVICE)
        labels = batch["label"].to(DEVICE)

        if poisoned:
            labels = flip_labels(labels)

        optimizer.zero_grad()
        outputs = net(images)
        loss = criterion(outputs, labels)

        if proximal_mu > 0.0 and global_params is not None:
            proximal_term = 0.0
            for w, w_t in zip(net.parameters(), global_params):
                proximal_term += (w - w_t).norm(2)
            loss = loss + (proximal_mu / 2.0) * proximal_term

        loss.backward()
        optimizer.step()

        running_loss += loss.item() * labels.size(0)
        _, predicted = torch.max(outputs.data, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item()

    epoch_loss = running_loss / total
    epoch_acc = correct / total
    return epoch_loss, epoch_acc


def test(net: nn.Module, valloader: DataLoader):
    criterion = nn.CrossEntropyLoss()
    net.eval()

    running_loss = 0.0
    correct = 0
    total = 0
    all_predictions = []
    all_labels = []
    all_probs = []

    with torch.no_grad():
        for batch in valloader:
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
    
    # Calculate additional metrics
    predictions_arr = np.array(all_predictions)
    labels_arr = np.array(all_labels)
    probs_arr = np.array(all_probs)
    
    f1 = f1_score(labels_arr, predictions_arr, average='weighted')
    kappa = cohen_kappa_score(labels_arr, predictions_arr)
    
    # ROC-AUC for multiclass (one-vs-rest)
    labels_binarized = label_binarize(labels_arr, classes=np.arange(10))
    roc_auc = roc_auc_score(labels_binarized, probs_arr, average='weighted', multi_class='ovr')
    
    return epoch_loss, epoch_acc, f1, kappa, roc_auc


# Flower client
class CifarClient(NumPyClient):
    def __init__(self,net: nn.Module,trainloader: DataLoader,valloader: DataLoader,poisoned: bool = False,):
        self.net = net
        self.trainloader = trainloader
        self.valloader = valloader
        self.poisoned = poisoned

    def get_parameters(self, config):  # type: ignore[override]
        return get_parameters(self.net)

    def fit(self, parameters, config):  # type: ignore[override]
        set_parameters(self.net, parameters)
        self.net.to(DEVICE)

        global_params: List[torch.Tensor] = [
            p.detach().clone() for p in self.net.parameters()
        ]

        proximal_mu = float(config.get("proximal_mu", 0.0))

        train_one_epoch(
            self.net,
            self.trainloader,
            proximal_mu=proximal_mu,
            global_params=global_params,
            poisoned=self.poisoned,
        )

        return get_parameters(self.net), len(self.trainloader.dataset), {}

    def evaluate(self, parameters, config):  # type: ignore[override]
        set_parameters(self.net, parameters)
        self.net.to(DEVICE)

        loss, accuracy, f1, kappa, roc_auc = test(self.net, self.valloader)
        return float(loss), len(self.valloader.dataset), {
            "accuracy": float(accuracy),
            "f1": float(f1),
            "kappa": float(kappa),
            "roc_auc": float(roc_auc)
        }


# Main
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--server-address", type=str, default="127.0.0.1:8080")
    parser.add_argument("--cid", type=int, required=True, help="Client ID (0..N-1)")
    parser.add_argument("--num-clients", type=int, default=5)
    parser.add_argument("--num-attackers", type=int, default=0)
    parser.add_argument("--partition-type", type=str, choices=["iid", "non-iid"], required=True)
    parser.add_argument("--batch-size", type=int, default=32)
    args = parser.parse_args()

    non_iid = args.partition_type == "non-iid"
    poisoned = args.cid < args.num_attackers

    print(
        f"[CLIENT {args.cid}] Starting "
        f"(partition={args.partition_type}, poisoned={poisoned})"
    )

    fds = get_federated_dataset(num_clients=args.num_clients, non_iid=non_iid)
    trainloader, valloader = load_datasets(fds, args.cid, args.batch_size)

    net = Model().to(DEVICE)
    client = CifarClient(net, trainloader, valloader, poisoned=poisoned)

    fl.client.start_numpy_client(
        server_address=args.server_address,
        client=client,
    )


if __name__ == "__main__":
    main()
