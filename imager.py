import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# File name
input_file = 'data_rounded.csv'

# Read the CSV and store x, y values
x_values = []
y_values = []

with open(input_file, mode='r') as infile:
    reader = csv.reader(infile)
    for row in reader:
        if row:  # Skip empty rows
            x, y = map(float, row)
            # Convert from meters to kilometers (divide by 1000)
            x_values.append(x / 1000)
            y_values.append(y / 1000)

# Diameter of Earth and Moon's distance from Earth (in kilometers)
earth_diameter = (2 * 6378137) / 1000  # Diameter of Earth in kilometers
moon_distance = (3.8 * 10**8) / 1000  # Moon's distance from Earth in kilometers

# Calculate the relative size of the Earth
earth_marker_size = (earth_diameter / moon_distance) * 100000  # Scale factor for visibility

# Set up the figure and axis
fig, ax = plt.subplots(figsize=(10, 6))

# Set the background color of the plot to black
fig.patch.set_facecolor('black')  # Set figure background color to black
ax.set_facecolor('black')  # Set axes background color to black

# Plot static Earth at the origin (we only plot this once)
ax.scatter(0, 0, color='green', s=earth_marker_size, label='Earth (to scale)')

# Set up the moving moon (start with an empty point) and set moon color to white
moon, = ax.plot([], [], 'o', color='white', label='Moon')  # 'o' for circular marker

# Graph details with white text
ax.set_title("Moon Trajectory Over Time", fontsize=14, color='white')
ax.set_xlabel("X values (km)", fontsize=12, color='white')  # X-axis in kilometers
ax.set_ylabel("Y values (km)", fontsize=12, color='white')  # Y-axis in kilometers
ax.axhline(0, color='white', linewidth=0.8, linestyle='--')  # Horizontal line at y=0
ax.axvline(0, color='white', linewidth=0.8, linestyle='--')  # Vertical line at x=0

# Set grid lines and ticks to white
ax.grid(True, linestyle='--', alpha=0.7, color='white')
ax.tick_params(axis='both', colors='white')  # Set tick labels color to white
ax.legend(fontsize=12, facecolor='black', edgecolor='white', labelcolor='white')  # White legend text

# Set the limits based on the moon's orbit range
ax.set_xlim(min(x_values) - 10, max(x_values) + 10)  # Set X-axis limits
ax.set_ylim(min(y_values) - 10, max(y_values) + 10)  # Set Y-axis limits

# Initialize function for FuncAnimation
def init():
    moon.set_data([], [])  # Empty data at the start
    return moon,

# Update function for each frame
def update(frame):
    # Update the moon's position for the current frame
    moon.set_data([x_values[frame]], [y_values[frame]])  # Wrap in lists for set_data()
    return moon,

# Create the animation
ani = FuncAnimation(fig, update, frames=len(x_values), init_func=init, blit=True, interval=50)

# Show the animation
plt.tight_layout()
plt.show()







