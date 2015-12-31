# Sudoku-Solver
Learning to use Open CV to recognize a sudoku puzzle and solve it. This project uses the tutorial from AI Shack: http://aishack.in/tutorials/sudoku-grabber-opencv-plot/ to extract information from the image. I will then see if it is possible to output an image with the solution.

##Achieved Goal
The goal was to take an image like this:

![Sudoku Puzzle] 
(https://raw.githubusercontent.com/mjchao/Sudoku-Solver/master/puzzle.jpg)

and output a solution like this:

![Sudoku Solution]
(https://raw.githubusercontent.com/mjchao/Sudoku-Solver/master/solution.jpg)

The program is currently capable of doing this for the specific puzzle shown above.

#Process
In this project, I first followed the AI Shack tutorial for extracting an undistorted puzzle from an arbitrary image. For example, I started with the initial picture

![Initial puzzle]
(https://raw.githubusercontent.com/mjchao/Sudoku-Solver/master/puzzle.jpg)

and extracted the following undistorted puzzle:

![Undistorted puzzle]
(https://raw.githubusercontent.com/mjchao/Sudoku-Solver/master/undistorted.jpg)

All the work after this was done without a tutorial. 

##Neural Network Training
Next, I attempted to train a neural network for classifying images of digits. The MNIST dataset for handwritten digits didn't seem to work - perhaps the handwritten digits weren't similar enough to the print digits. I then wrote a separate program to generated the training and test data using all the fonts on my computer. This resulted in training sets like the one shown below:

<img src="https://raw.githubusercontent.com/mjchao/Sudoku-Solver/master/PhotoOCRTraining/train/1-tiles.png" height="1000">

There was approximately 79% accuracy on the test set. Perhaps it would be a good idea to generate a larger training set and train a deeper neural network on a machine with more computational power. It may also be a good idea to consider other learning algorithms.

##Additional Ad-Hoc Heuristics
The neural network that I trained worked for the most part, but not as smoothly as I would have liked. The NN used all pixels in the 28x28 training images as features. The training data overlayed white digits at the center of a black background. The NN got confused when presented with extraneous white pixels on the border. This was a consequence of the gridlines getting captured in the images. So, I had to implement several more ad-hoc heuristics to extract the digits from the Sudoku cells, overlay them on a black background, and then ask the NN to classify the isolated digits. This turned out okay.

##Difficulties
As of right now, the program has difficulty dealing with slightly-curled-up paper and recognizing digits written in a thin font. Curled up paper causes cells to get unaligned in the undistorted puzzle image and throws off the heursitics for extracting the digits. The extracted digits tend to get thrown off the center and the digit-isolation heursitic sometimes mistakes the gridlines as the digits. The thin fonts are a problem because the digit-isolation heuristic assumes that the biggest white blob in an image is the digit. Sometimes, digits written in a thin font become disconnected when the image is shrunk to 28x28 so that the NN can classify it. To deal with the thin fonts, one would have to toggle on an option to dilate the image to make the font thicker.

##Puzzle Solver
Once the digits were extracted, it was a simple task to write a recursive backtracking algorithm to find a solution. The only problem is that if the digits were read incorrectly, the solution would be incorrect or would never be found. Currently, the program can recognize all the digits in the puzzle shown above and solve it successfully. It currently does not generalize well to other puzzles. For most puzzles, it misclassifies a few digits and if we don't correctly read the puzzle, we won't be able to solve it.

##Failed Example
Here is an example of a puzzle that failed:

![Failed puzzle]
(https://github.com/mjchao/Sudoku-Solver/raw/master/puzzle5.jpg)

![Failed solution]
(https://github.com/mjchao/Sudoku-Solver/raw/master/solution5.jpg)

The program misread several numbers and believed the puzzle was

<pre>
4 3 7 ? 8 8 ? ? ? 
? ? ? 7 ? ? 8 ? 7 
? 8 ? ? ? 5 ? 8 ? 
? 4 ? ? ? 1 ? ? ? 
8 ? 1 ? 5 ? 6 ? 9 
? ? ? 6 ? ? ? 1 ? 
? 7 ? 5 ? ? ? 9 ? 
7 ? 5 ? ? 6 ? ? ? 
? ? ? 9 8 ? 1 5 6 
</pre>
Consequently, it could not find a valid solution. This is probably due to bad training data. I will be trying to find better ways to generate good training data.
