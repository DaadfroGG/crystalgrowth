import numpy
import PIL as Pillow
import matplotlib.pyplot

# Step 1: Load an image 
image = Pillow.Image.open("the bois.png")

# Step 2: Convert the image to a numpy array
image_array = numpy.array(image)

# Step 3: plot the image
matplotlib.pyplot.imshow(image_array)

# Step 4: Show the plot
matplotlib.pyplot.show()
