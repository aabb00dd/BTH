"""
Custom flower strategies for defenses
Namn: Simon Lindqvist: siln22@student.bth.se, Abdalrahman Mohammed: abmm22@student.bth.se
"""

from typing import List, Tuple, Dict, Optional, Union
import numpy as np
from flwr.common import FitRes, Parameters, Scalar, ndarrays_to_parameters, parameters_to_ndarrays
from flwr.server.client_proxy import ClientProxy
from flwr.server.strategy import FedAvg


class FedMedian(FedAvg):
    """
    Fed Median defense implementation.
    """

    def aggregate_fit(self, server_round: int, results: List[Tuple[ClientProxy, FitRes]], failures: List[Union[Tuple[ClientProxy, FitRes], BaseException]]):
        """Aggregate model weights using coordinate-wise median."""
        if not results:
            return None, {}

        # Extract weights from results
        weights_list = [parameters_to_ndarrays(fit_res.parameters) for _, fit_res in results]

        # Apply median aggregation
        aggregated_weights = aggregate_median(weights_list)
        
        parameters_aggregated = ndarrays_to_parameters(aggregated_weights)

        # Aggregate custom metrics if available
        metrics_aggregated = {}
        if self.fit_metrics_aggregation_fn:
            fit_metrics = [(res.num_examples, res.metrics) for _, res in results]
            metrics_aggregated = self.fit_metrics_aggregation_fn(fit_metrics)

        return parameters_aggregated, metrics_aggregated


class FedClip(FedAvg):
    """
    Fed Clip defense implementation.
    """

    def __init__(self, clip_threshold: float = 5.0, *args, **kwargs):
        """
        Args:
            clip_threshold: Maximum allowed L2 norm difference from median
        """
        super().__init__(*args, **kwargs)
        self.clip_threshold = clip_threshold

    def aggregate_fit(self, server_round: int, results: List[Tuple[ClientProxy, FitRes]], failures: List[Union[Tuple[ClientProxy, FitRes], BaseException]]):
        """Aggregate model weights with clipping defense."""
        if not results:
            return None, {}

        # Extract weights and num_examples
        weights_results = [
            (parameters_to_ndarrays(fit_res.parameters), fit_res.num_examples)
            for _, fit_res in results
        ]

        # Apply clipping defense
        clipped_weights = clip_updates(
            [weights for weights, _ in weights_results],
            self.clip_threshold
        )

        # Weighted average after clipping
        aggregated_weights = weighted_average(
            [(w, num) for w, (_, num) in zip(clipped_weights, weights_results)]
        )

        parameters_aggregated = ndarrays_to_parameters(aggregated_weights)

        # Aggregate custom metrics if available
        metrics_aggregated = {}
        if self.fit_metrics_aggregation_fn:
            fit_metrics = [(res.num_examples, res.metrics) for _, res in results]
            metrics_aggregated = self.fit_metrics_aggregation_fn(fit_metrics)

        return parameters_aggregated, metrics_aggregated


# --- Helper functions --- #
def aggregate_median(weights_list: List[List[np.ndarray]]) -> List[np.ndarray]:
    """Compute coordinate-wise median across all client weights."""
    aggregated = []
    num_layers = len(weights_list[0])
    
    for layer_idx in range(num_layers):
        # Stack this layer from all clients: shape (num_clients, *layer_shape)
        layer_stack = np.array([weights[layer_idx] for weights in weights_list])
        # Compute median across clients (axis 0)
        median_layer = np.median(layer_stack, axis=0)
        aggregated.append(median_layer)
    
    return aggregated


def clip_updates(weights_list: List[List[np.ndarray]], clip_threshold: float):
    """Clip client updates based on L2 norm distance from median."""
    # Compute median as reference point
    median_weights = aggregate_median(weights_list)
    
    clipped_results = []
    for client_weights in weights_list:
        clipped_client = []
        
        for client_layer, median_layer in zip(client_weights, median_weights):
            # Calculate difference from median
            diff = client_layer - median_layer
            norm = np.linalg.norm(diff.flatten())
            
            if norm > clip_threshold:
                # Scale down the difference to threshold
                clipped_diff = diff * (clip_threshold / norm)
                clipped_layer = median_layer + clipped_diff
            else:
                clipped_layer = client_layer
            
            clipped_client.append(clipped_layer)
        
        clipped_results.append(clipped_client)
    
    return clipped_results


def weighted_average(weights_results: List[Tuple[List[np.ndarray], int]]):
    """Compute weighted average of model parameters."""
    # Calculate total examples
    num_examples_total = sum(num_examples for _, num_examples in weights_results)
    
    # Compute weighted layers
    num_layers = len(weights_results[0][0])
    weighted_layers = []
    
    for layer_idx in range(num_layers):
        # Weighted sum for this layer
        layer_sum = np.zeros_like(weights_results[0][0][layer_idx])
        for weights, num_examples in weights_results:
            layer_sum += weights[layer_idx] * num_examples
        
        # Normalize by total examples
        weighted_layers.append(layer_sum / num_examples_total)
    
    return weighted_layers
