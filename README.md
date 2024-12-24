# Chain of Dimensions Calc

This project calculates dimensions in a chain of dimensions based on user input from a graphical interface. The application is designed using the raygui library, providing a simple and intuitive front panel for users to interact with.

## Features

- Takes from user the nominal value and its upper and lower deviation for the increasing dimension:

![Increasing Dim](images/cote_maritoare.png)

- Takes up to 5 reducing dimensions and their deviations

![Reducing Dims](images/cote_micsoratoare.png)

- you can add or remove reducing dimensions with buttons next to "Cote Micsoratoare"

![add/delete](images/add_or_delete.png)

- Display of calculated value of the unknown dimension in the chain of dimensions

![result](images/result.png)

- Visual Representations of Reducing dimensions and L out of the Increasing Dimensions after pressing Calculate
![example](images/example.png)

## Dependencies

This project requires the raylib and raygui to be installed

## Build & Run

**gcc main.c -o main.exe -lraylib**
**./main.exe**

![build](images/build.png)
