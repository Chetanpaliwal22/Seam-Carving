# Seam-Carving
Image retargeting is the process of pasting the content of an image of certain size to a canvas of a different size. The goal is to take the content of Image and paste it to a canvas that is shorter along the x axis. Common methods include non-uniform scaling or cropping. However, both have their shortcomings. Non-uniform scaling may distort the proportions of the objects in the scene and cropping might remove important content. A better solution is seam-carving.

In this project, seam carving is implemented using dynamic programming and the OpenCV library.

Programming Language : C++ with OpenCv Library


Prerequisites:

cmake, opencv


To compile in the lab
——————————

run ./lab_config.sh
cd build
make

To compile on your own machine
———

cd build/
cmake ../
make


To run
——

In the build folder:
./sc image1 new_width new_height image2