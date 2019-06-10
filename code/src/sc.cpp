
/*Reference:
https://docs.opencv.org/2.4.13.7/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html
*/

#include "sc.h"
#include "vector"

using namespace cv;
using namespace std;

//vector<int> energy;
//int energyImage[10000][10000];


int
calculateMinThree (int a, int b, int c)
{
  int smallest;
  if (a <= b && a <= c)
    {
      smallest = a;
    }
  else if (b <= c && b <= a)
    {
      smallest = b;
    }
  else
    {
      smallest = c;
    }
  return smallest;
}

int
calculateMinTwo (int a, int b)
{
  int smallest;
  if (a <= b)
    {
      smallest = a;
    }
  else
    {
      smallest = b;
    }
  return smallest;
}


//Calculate Vertical Energy
bool
calculate_vertical_energy (Mat & in_image, Mat & outImage, Mat & seamImage)
{

  Mat src = in_image.clone ();
  Mat src_gray;
  cvtColor (src, src_gray, CV_BGR2GRAY);

  Mat grad;

// For grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  // For Gradient X
  Sobel (src_gray, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
  convertScaleAbs (grad_x, abs_grad_x);

  // For Gradient Y
  Sobel (src_gray, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
  convertScaleAbs (grad_y, abs_grad_y);

  /// Gradient Image Final
  addWeighted (abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

  // imshow( "Grad Image", grad );
//waitKey(0);

//Calculating Energy
  int numberRows = in_image.rows;
  int numberCols = in_image.cols;

  int energyImage[numberRows][numberCols];



  for (int j = 0; j < numberCols; j++)
    {
      energyImage[0][j] = grad.at < uchar > (0, j);
    }

  for (int i = 1; i < numberRows; i++)
    {
      for (int j = 0; j < numberCols; j++)
	{
	  if (j == 0)
	    {
	      int min =
		calculateMinTwo (energyImage[i - 1][j],
				 energyImage[i - 1][j + 1]);
	      //       cout << "Min of two: == " << min << endl;
	      energyImage[i][j] = min + grad.at < uchar > (i, j);

	    }
	  if (j > 0 && j < (numberCols - 1))
	    {
	      int min =
		calculateMinThree (energyImage[i - 1][j - 1],
				   energyImage[i - 1][j],
				   energyImage[i - 1][j + 1]);

	      //    cout << "Min of three: >< " << min << endl;

	      energyImage[i][j] = min + grad.at < uchar > (i, j);
	    }
	  if (j == (numberCols - 1))
	    {
	      int min =
		calculateMinTwo (energyImage[i - 1][j - 1],
				 energyImage[i - 1][j]);
	      //      cout << "Min of two: ==3 " << min << endl;
	      energyImage[i][j] = min + grad.at < uchar > (i, j);
	    }
	}
    }


  Mat energyImageMat;



// create an image slighly smaller
  outImage = Mat (numberRows, numberCols - 1, CV_8UC3);

//seamImage = Mat(numberRows, numberCols, CV_8UC3);

//cout<<numberRows<<" "<<numberCols;
  //int outputImage[4][4];

  int min = energyImage[numberRows - 1][0];

  int minimumColumnIndex = 0;

  for (int i = 0; i < numberCols; i++)
    {
      if (min > energyImage[numberRows - 1][i])
	{
	  min = energyImage[numberRows - 1][i];
	  minimumColumnIndex = i;
	}
    }
//cout<<"Min Column Index: "<<minimumColumnIndex;

//cout<< "Min Col Index: "<<minimumColumnIndex<<endl;
//cout<<"TEst: "<<inImage.at<Vec3b>((numberRows-1),(numberCols-1))<<endl;

  for (int i = (in_image.rows - 1); i >= 0; i--)
    {

      int skipIndex = 0;
      for (int j = 0; j < (in_image.cols - 1); j++)
	{
	  if (j != minimumColumnIndex)
	    {
//cout<<"print "<<i<<"col Index "<<skipIndex<<" j "<<j<<endl;
	      outImage.at < Vec3b > (i, skipIndex++) =
		in_image.at < Vec3b > (i, j);


	    }
	  else
	    {
	      Vec3b pixel;
	      pixel[0] = 0;
	      pixel[1] = 0;
	      pixel[2] = 0;
	      seamImage.at < Vec3b > (i, skipIndex) = pixel;
	    }
	}

      if (i > 0)
	{

	  if (minimumColumnIndex == 0)
	    {

	      if (energyImage[i - 1][minimumColumnIndex + 1] <
		  energyImage[i - 1][minimumColumnIndex])
		{
		  minimumColumnIndex = minimumColumnIndex + 1;
		}
	    }
	  else if (minimumColumnIndex == numberCols - 1)
	    {
	      if (energyImage[i - 1][minimumColumnIndex - 1] <
		  energyImage[i - 1][minimumColumnIndex])
		{
		  minimumColumnIndex = minimumColumnIndex - 1;
		}
	    }
	  else
	    {
	      if (energyImage[i - 1][minimumColumnIndex - 1] <
		  calculateMinTwo (energyImage[i - 1][minimumColumnIndex],
				   energyImage[i - 1][minimumColumnIndex +
						      1]))
		{
		  minimumColumnIndex = minimumColumnIndex - 1;
		}
	      else if (energyImage[i - 1][minimumColumnIndex + 1] <
		       calculateMinTwo (energyImage[i - 1]
					[minimumColumnIndex - 1],
					energyImage[i -
						    1][minimumColumnIndex]))
		{
		  minimumColumnIndex = minimumColumnIndex + 1;
		}
	    }
	}
    }


  return true;
}

//Calculate Horizontal Energy
bool
calculate_horizontal_energy (Mat & in_image, Mat & outImage, Mat & seamImage)
{

  Mat src = in_image.clone ();
  Mat src_gray;
  cvtColor (src, src_gray, CV_BGR2GRAY);

  Mat grad;

/// For grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  // For Gradient X
  Sobel (src_gray, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
  convertScaleAbs (grad_x, abs_grad_x);

  // For Gradient Y
  Sobel (src_gray, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
  convertScaleAbs (grad_y, abs_grad_y);

  // Final Gradient
  addWeighted (abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

 //  imshow( "Grad Image", grad );
//waitKey(0);

//Calculating Energy
  int numberRows = in_image.rows;
  int numberCols = in_image.cols;

  int energyImage[numberRows][numberCols];


  for (int i = 0; i < numberRows; i++)
    {
      energyImage[i][0] = grad.at < uchar > (i, 0);
    }

  for (int j = 1; j < numberCols; j++)
    {
      for (int i = 0; i < numberRows; i++)
	{
	  if (i == 0)
	    {
	      int min =
		calculateMinTwo (energyImage[i][j - 1],
				 energyImage[i + 1][j - 1]);
	      //       cout << "Min of two: == " << min << endl;
	      energyImage[i][j] = min + grad.at < uchar > (i, j);

	    }
else if (i == (numberRows -1))
	    {
	      int min =
		calculateMinTwo (energyImage[i - 1][j - 1],
				 energyImage[i][j - 1]);
	      //      cout << "Min of two: ==3 " << min << endl;
	      energyImage[i][j] = min + grad.at < uchar > (i, j);
	    }	  
else if (i > 0 && i < (numberRows-1))
	    {
	      int min =
		calculateMinThree (energyImage[i - 1][j - 1],
				   energyImage[i][j - 1],
				   energyImage[i + 1][j - 1]);

	      //    cout << "Min of three: >< " << min << endl;

	      energyImage[i][j] = min + grad.at < uchar > (i, j);
	    }
	  
	}
    }


//Mat energyImageMat;


// create an image slighly smaller
  outImage = Mat (numberRows - 1, numberCols, CV_8UC3);

  int min = energyImage[0][numberCols-1];

  int minimumRowIndex = 0;

//cout<<"Final: "<<energyImage[0][numberCols]<<endl;
//cout<<"Second Last: "<<energyImage[0][numberCols-1]<<endl;
  for (int i = 0; i < numberRows; i++)
    {
      if (min >= energyImage[i][numberCols-1])
	{
	  min = energyImage[i][numberCols-1];
	  minimumRowIndex = i;
	}
    }

//cout<<"Min Row Index: "<<minimumRowIndex;
//cout<< "Min Col Index: "<<minimumColumnIndex<<endl;
//cout<<"TEst: "<<inImage.at<Vec3b>((numberRows-1),(numberCols-1))<<endl;

  for (int j = (numberCols - 1); j >= 0; j--)
    {

      int skipIndex = 0;
      for (int i = 0; i < numberRows ; i++)
	{
	  if (i != minimumRowIndex)
	    {
//cout<<"print "<<i<<"col Index "<<colIndex<<" j "<<j<<endl;
	      outImage.at < Vec3b > (skipIndex++, j) =
		in_image.at < Vec3b > (i, j);
	    }
	  else
	    {
	      Vec3b pixel;
	      pixel[0] = 0;
	      pixel[1] = 0;
	      pixel[2] = 0;
	      seamImage.at < Vec3b > (skipIndex, j) = pixel;
	    }
	}

//cout<<"Done.";

      if (j > 0)
	{

	  if (minimumRowIndex == 0)
	    {

	      if (energyImage[minimumRowIndex + 1][j - 1] <
		  energyImage[minimumRowIndex][j - 1])
		{
		  minimumRowIndex = minimumRowIndex + 1;
		}
	    }
	  else if (minimumRowIndex == numberRows - 1)
	    {
	      if (energyImage[minimumRowIndex - 1][j - 1] <
		  energyImage[minimumRowIndex][j - 1])
		{
		  minimumRowIndex = minimumRowIndex - 1;
		}
	    }
	  else
	    {
	      if (energyImage[minimumRowIndex - 1][j - 1] <
		  calculateMinTwo (energyImage[minimumRowIndex][j - 1],
				   energyImage[minimumRowIndex + 1][j - 1]))
		{
		  minimumRowIndex = minimumRowIndex - 1;
		}
	      else if (energyImage[minimumRowIndex + 1][j - 1] <
		       calculateMinTwo (energyImage[minimumRowIndex - 1]
					[j - 1],
					energyImage[minimumRowIndex][j - 1]))
		{
		  minimumRowIndex = minimumRowIndex + 1;
		}
	    }
	}
    }


  return true;
}


bool
seam_carving (Mat & in_image, int new_width, int new_height, Mat & out_image)
{

  // some sanity checks
  // Check 1 -> new_width <= in_image.cols

//cout<<"New Width"<<in_image.cols;
//cout<<"New Height"<<in_image.rows;

  if (new_width > in_image.cols)
    {
      cout <<
	"Invalid request!!! new_width has to be smaller than the current size!"
	<< endl;
      return false;
    }
  if (new_height > in_image.rows)
    {
      cout <<
	"Invalid request!!! ne_height has to be smaller than the current size!"
	<< endl;
      return false;
    }

  if (new_width <= 0)
    {
      cout << "Invalid request!!! new_width has to be positive!" << endl;
      return false;

    }

  if (new_height <= 0)
    {
      cout << "Invalid request!!! new_height has to be positive!" << endl;
      return false;

    }


  return seam_carving_trivial (in_image, new_width, new_height, out_image);
}


// seam carves by removing trivial seams
bool
seam_carving_trivial (Mat & in_image, int new_width, int new_height,
		      Mat & out_image)
{

  cout << "Start";
//Calling that method


  Mat iimage = in_image.clone ();
  Mat oimage = in_image.clone ();

  Mat seamImage = in_image.clone ();


  while (iimage.cols != new_width || iimage.rows != new_height)
    {
//while(in_image.cols!=new_height){

      // horizontal seam if needed
      //if(in_image.rows>new_height){
      if (iimage.cols > new_width)
	{
	  calculate_vertical_energy (iimage, oimage, seamImage);
//cout<<" : "<<in_image.cols<<endl;
//reduce_horizontal_seam_trivial(iimage, oimage);
	  iimage = oimage.clone ();
	}

      if (iimage.rows > new_height)
	{
	  calculate_horizontal_energy (iimage, oimage, seamImage);
//cout<<" : "<<in_image.rows<<endl;
//reduce_horizontal_seam_trivial(iimage, oimage);
	  iimage = oimage.clone ();
	}
    }


/*
while(in_image.rows!=new_width){
        
if(in_image.rows>new_width){
calculate_horizontal_energy(in_image,oimage);
//cout<<" : "<<in_image.rows<<endl;
//reduce_horizontal_seam_trivial(iimage, oimage);
            in_image = oimage.clone();
        }
}
*/
 // imshow ("Final Output Image After Seam Remove", in_image);
 // imshow (" Output Image  Seam ", seamImage);

  /* while(iimage.rows!=new_height || iimage.cols!=new_width){
     // horizontal seam if needed
     if(in_image.rows>new_height){

     calculate_horizontal_energy(iimage,oimage);
     //cout<<" : "<<in_image.cols<<endl;
     //reduce_horizontal_seam_trivial(iimage, oimage);
     iimage = oimage.clone();
     }

     if(iimage.cols>new_width){
     calculate_vertical_energy(iimage,oimage,seamImage);
     iimage = oimage.clone();
     }
     }
   */
  out_image = oimage.clone ();
  return true;
}
