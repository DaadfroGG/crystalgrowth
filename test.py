import numpy as np
from scipy.spatial import Voronoi, voronoi_plot_2d
import matplotlib.pyplot as plt

# Step 1: Generate random points
np.random.seed(42)
points = np.random.rand(40, 2)

# Step 2: Generate Voronoi diagram
vor = Voronoi(points)

# Step 3: Relaxation (optional, depending on your preference)
for _ in range(5):
    vor = Voronoi(vor.points)

# Step 4: Plot the Voronoi diagram
voronoi_plot_2d(vor)

# Step 5-7: Further processing and customization can be added here

plt.show()
