import os
import numpy as np
import matplotlib.pyplot as plt
import cv2


people_folder = ".\Data analysis/Face Image analysis/People"
people_names = os.listdir(people_folder)
all_means = []
all_stds = []

for name in people_names:
    person_images = []
    for filename in os.listdir(os.path.join(people_folder, name)):
        img = cv2.imread(os.path.join(people_folder, name, filename), cv2.IMREAD_GRAYSCALE)
        person_images.append(img)
    mean = np.mean(person_images, axis=(1, 2))
    std_dev = np.std(person_images, axis=(1, 2))
    all_means.append(mean)
    all_stds.append(std_dev)

avg_mean = np.mean(all_means, axis=1)
avg_std = np.mean(all_stds, axis=1)


test_folder = ".\Data analysis\Face Image analysis\Test"
test_means = []
test_stds = []

for filename in os.listdir(test_folder):
    img = cv2.imread(os.path.join(test_folder, filename), cv2.IMREAD_GRAYSCALE)
    mean = np.mean(img)
    std = np.std(img)
    test_means.append(mean)
    test_stds.append(std)


distances = np.zeros((len(test_means), len(people_names)))
for i, (test_mean, test_std) in enumerate(zip(test_means, test_stds)):
    for j, (person_mean, person_std) in enumerate(zip(avg_mean, avg_std)):
        distance = np.sqrt((test_mean - person_mean) ** 2 + (test_std - person_std) ** 2)
        distances[i, j] = distance


print("Table 1. Calculated distances of images in test to the means of other people:")
print("{:<10}".format("Image"), end="")
for name in people_names:
    print("{:<10}".format(name), end="")
print()
for i, row in enumerate(distances):
    print("{:<10}".format(f"Image {i+1}"), end="")
    for distance in row:
        print("{:<10.4f}".format(distance), end="")
    print()


colors = ["Red", "Green", "Blue", "Brown", "Black"]
plt.figure(figsize=(10, 6))
for i in range(len(people_names)):
    plt.scatter(all_means[i], all_stds[i], color=colors[i], label=people_names[i])
    plt.scatter(avg_mean[i], avg_std[i], color=colors[i], marker="x", label=people_names[i] + "_Mean")
plt.xlabel("Mean Intensity Value")
plt.ylabel("Standard Deviation")
plt.title("Mean Intensity Value vs Standard Deviation")
plt.legend()
plt.grid()
plt.show()
