import pickle
import hashlib
import numpy as np


class ComparisonClass:
    def __init__(self, name, value):
        self.name = name
        self.value = value

    def __eq__(self, other):
        """
        Custom comparison method which compares the value and name of the object and returns True if they are both equal.
        Args:
            other: The other object to compare with.
        Returns:
            bool: True if both objects are equal, False otherwise.
        """
        # First determien if the other object even is a ComparisonClass
        if isinstance(other, ComparisonClass):
            # Compare the hash values
            value_comp = False
            if self.value == other.value:
                value_comp = True

            # Compare the name values too
            name_comp = False
            if self.name == other.name:
                name_comp = True

            # Return True if both hash and name are equal
            if value_comp and name_comp:
                return True

        # If both objects are not a comparison class or the hash values and/or names are not equal return False
        return False


def pickle_hash_object(input_object):
    """
    Non method to hash an object using the SHA-256 algorithm. This had to be done as redefining the __hash__ method would
    not work as python wants __hash__ to return an integer.
    Args:
        obj: The object to hash.
    Returns:
        str: The SHA-256 hash of the object. (hexadecimal string)
    """
    # Pickle that mf
    pickled_object = pickle.dumps(input_object)

    # Hash dat bad boi real hard
    hashed_object = hashlib.sha256(pickled_object).hexdigest()

    # Return the hash, UwU
    return hashed_object


"""
Disclaimer/Source notice:
Suitable test cases aquired using Claude 3.7 sonnet using the github copilot plugin in visual studio code. 
They were aquired using autocomplete after writing each test case in the dictionaries.
It was then further modified to suit the needs of this project.
"""
pickle_normal_test_cases = {
    "simple_int": 42,
    "simple_float": 3.14,
    "simple_string": "Hello, pickle!",
    "simple_bool": False,
    "simple_none": None,
    "simple_list": [1, 2, 3, 4, 5],
    "simple_tuple": (10, 20, 30),
    "simple_dict": {"key1": "value1", "key2": 42},
    "simple_set": {1, 2, 3, 4},
    "bytes_data": b"binary data",
    "custom_object": ComparisonClass("test_object", 12345),
    "nested_structure": {"name": "nested", "data": [1, 2, {"inner": True}]},
    "empty_containers": {"list": [], "dict": {}, "tuple": (), "set": set()},
}

pickle_edge_test_cases = {
    "inf": float("inf"),
    "neg_inf": float("-inf"),
    "nan": float("nan"),
    "numpy_pos_zero": np.float64(0.0),
    "numpy_neg_zero": np.float64(-0.0),
    "numpy_pos_inf": np.float64(np.inf),
    "numpy_neg_inf": np.float64(-np.inf),
    "numpy_nan": np.float64(np.nan),
    "very_large": 10**1000, 
    "very_small": 1e-100,
    "emoji": "😀🌍🚀",
    "multi_language": "English, 中文, Русский, العربية",
    "complex_num": complex(3, 4),
    "custom_objects": [ComparisonClass(f"obj_{i}", i) for i in range(5)],
    "large_dict": {str(i): i for i in range(1000)},
    "byte_array": bytearray(b"mutable bytes"),
    "frozen_set": frozenset([1, 2, 3])
}


def pickle_and_store():
    """ 
    Pickles, hashes and stores the results of the test cases in a dictionary.
    Returns:
        dict: A dict
    """
    results = {}

    for test_name, test_case in pickle_normal_test_cases.items():
        try:
            # Pickle and hash using the helper
            pickled_hashed_object = pickle_hash_object(test_case)

            # Store in results
            results[test_name] = pickled_hashed_object

        except Exception as e:
            results[test_name] = f"Error: {str(e)}"

    for test_name, test_case in pickle_edge_test_cases.items():
        try:
            # Pickle and hash using the helper
            pickled_hashed_object = pickle_hash_object(test_case)

            # Store in results
            results[test_name] = pickled_hashed_object

        except Exception as e:
            results[test_name] = f"Error: {str(e)}"

    return results
