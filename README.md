# Parallel & Sequential Work

This performs the following tasks, first in a sequential method, then a parallel method.

1. Load in Image 1.

2. Load in Image 2.

3. Create Image 3, by taking the RGB values from pixel (x,y) from Image 1, and subtracting the RGB values from pixel (x,y) from Image 2, then placing that at pixel (x,y) on Image 3.

4. Perform a Binary Threshold application, taking pixel (x,y) from Image 3, and making it white if it is not black.

5. Save Image 3.

6. Calculate the total number of white pixels, and then finding the percentage of white pixels to black pixels.

7. Place a random red pixel somewhere in the image, then finding where the pixel is located and outputting it to the console.