# a proof-of-concept for `potato engine` :]
## THIS IS A WIP.
## NOTE: 64-bit CPUs are guaranteed to work with this, as long as they're relatively new (This project was built with an AMD Phenom(tm) II X4 965 (4) @ 3.40 GHz, so I guess any CPU that was made past 2009 should work.)
# Make
## Usage:  
```make```: Compiles the project into an x86_64 binary with the necessary assets in a ready-to-compress folder titled build/.  
```make arm```: Compiles the project into an ARM binary with the necessary assets in a ready-to-compress folder titled build/arm/.  
```make clean```: Deletes the build/ directory for fresh beginnings.  
```make run```: Runs the x86_64 binary in ```build/chaser```.  
```make runarm```: Runs the ARM binary in ```build/arm/chaser```.
