# mnist-1lnn
A simple 1-layer neural network to recognize handwritten single digit numbers from the MNIST image files.


### Compile and run source code

The repository comes with a pre-configured `makefile`. You can compile the source simply by typing

```
$ make
```

in the project directory. The binary will be created inside the `/bin` folder and can be executed via

```
$ ./bin/mnist-1lnn
```

### Documentation

The  `/doc` folder contains a doxygen configuration file. 
When you run it with doxygen it will create updated [HTML documentation](https://rawgit.com/mmlind/mnist-1lnn/master/doc/html/index.html) in the `/doc/html` folder.

### Screenshots

The `/screenshots` folder contains screenshots of running the program on my 2010 MacBook Pro.


### MNIST Database

The `/data` folder contains the original MNIST database files.

### for running under xcode

Need to use cross platfrom external build system
Import
change the main off the main and not subdir /bin  (may be scheme can help but not sure how)
Create a Build scheme
Any existing binary will create a no the right os message and ask you to set minimum platform
Just delete the old binary it would be ok
but once build you have to delete from outside
or you can delete the run scheme and add doc and ... it seems strange how it works

sometimes work sometimes not; just go to console and run it over there instead

in this case I remove back to original
