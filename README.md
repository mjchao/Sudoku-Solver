# Sudoku-Solver
Learning to use Open CV to recognize a sudoku puzzle and solve it. This project uses the tutorial from AI Shack: http://aishack.in/tutorials/sudoku-grabber-opencv-plot/ to extract information from the image. I will then see if it is possible to output an image with the solution.

The goal is to take an image like this:

![Sudoku Puzzle] 
(https://raw.githubusercontent.com/mjchao/Sudoku-Solver/master/puzzle.jpg)

and output a solution like this:

![Sudoku Solution]
(https://raw.githubusercontent.com/mjchao/Sudoku-Solver/master/solution.jpg)

#Process
In this project, I first followed the AI Shack tutorial for extracting an undistorted puzzle from an arbitrary image. Next, I attempted to train a neural network for classifying images of digits. The MNIST dataset for handwritten digits didn't seem to work - perhaps the handwritten digits weren't similar enough to the print digits. I then wrote a separate program to generated the training and test data using all the fonts on my computer. The neural network worked for the most part, but not as smoothly as I would have liked. The NN used all pixels in the 28x28 training images as features. The training data overlayed white digits at the center of a black background. The NN could not classify digits with extraneous white pixels on the borders and so I had to implement several more ad-hoc heuristics to extract the digits from the Sudoku cells, overlay them on a black background, and then ask the NN to classify the isolated digits. This ended up succeeding.

As of right now, the program has difficulty dealing with slightly-curled-up paper and recognizing digits written in a thin font. Curled up paper causes cells to get unaligned in the undistorted puzzle image and throws off the heursitics for extracting the digits. The extracted digits tend to get thrown off the center and the digit-isolation heursitic sometimes mistakes the gridlines as the digits. The thin fonts are a problem because the digit-isolation heuristic assumes that the biggest white blob in an image is the digit. Sometimes, digits written in a thin font become disconnected when the image is shrunk to 28x28 so that the NN can classify it. To deal with the thin fonts, one would have to toggle on an option to dilate the image to make the font thicker.

Currently, the program can recognize all the digits in the puzzle shown above.


