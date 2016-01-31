# Object Detector Framework#

Unix project for the automatic detection of several objects.

## Dependencies ##
* OpenCV 2.4.9
* libconfig++
* easylogging++ (included)

## Use ##

Build the main program by running 'make' in the base directory and run './odf' to process all images placed in the benchmark folder, results will be saved in 'benchmark_out'. You can use options '-d' and '-o' to specify the directory with the pictures to process and where to save results, respectively.

You can set which detectors to use in the execution by editing the './config/sample.conf' file. In this file you can set several of the application's options as well as the detectors.

At the moment the only detector implemented is a cascade classifier based detector.
