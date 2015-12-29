# Sudoku-Solver
Learning to use Open CV to recognize a sudoku puzzle and solve it. This project uses the tutorial from AI Shack: http://aishack.in/tutorials/sudoku-grabber-opencv-plot/ to extract information from the image. I will then see if it is possible to output an image with the solution.

The goal is to take an image like this:

![Sudoku Puzzle] 
(https://raw.githubusercontent.com/mjchao/Sudoku-Solver/master/puzzle.jpg)

and output a solution.

#Process
In this project, I first followed the AI Shack tutorial for extracting an undistorted puzzle from an arbitrary image. Next, I attempted to train a neural network for classifying images of digits. The MNIST dataset for handwritten digits didn't seem to work - perhaps the handwritten digits weren't similar enough to the print digits. So, I wrote a separate progrma to generated the training and test data. As of right now, the program has difficulty dealing with slight distortions such as slightly-curled-up paper and recognizing digits written in a thin font.


