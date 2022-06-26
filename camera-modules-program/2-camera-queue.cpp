#include <opencv2/highgui.hpp>
#include <iostream>
//#include <ctime>
#include <cstdio>
//#include <time.h>
#include <stdio.h>
#include <queue>
#include <cstdlib>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>


using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{

  queue<Mat> myQ; //queue for normal recording of camera-1

  queue<Mat> accQ; // queue for recording the video after crash for camera-1

  queue<Mat> myQ_2; // queue for normal recording of camera-2

  queue<Mat> accQ_2; // queue for recording the video after crash for camera-2

  
  
  //srand((unsigned) time(0)); //random number generator
  //int randomNumber;

  	int fd;														// File descrition
	char *fileName = "/dev/i2c-1";								// Name of the port we will be using
	int  address = 0x53;										// Address of the SRF02 shifted right one bit
	char buf[6];										// Buffer for data being read/ written on the i2c bus
	short x,y,z;

	if ((fd = open(fileName, O_RDWR)) < 0) {					// Open port for reading and writing
		printf("Failed to open i2c port\n");
		exit(1);
	}
	
	if (ioctl(fd, I2C_SLAVE, address) < 0) {					// Set the port options and set the address of the device we wish to speak to
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}

	buf[0] = 0x2d;													// Commands for performing a ranging
	buf[1] = 0x18;
	
	if ((write(fd, buf, 2)) != 2) {								// Write commands to the i2c port
		printf("Error writing to i2c slave\n");
		exit(1);
	}

	buf[0] = 0x31;													// Commands for performing a ranging
	buf[1] = 0x09;
	
	if ((write(fd, buf, 2)) != 2) {								// Write commands to the i2c port
		printf("Error writing to i2c slave\n");
		exit(1);
	}



  
  
  
    
    //Open the default video camera
    VideoCapture cap(0);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video camera" << endl;
        cin.get(); //wait for any key press
        return -1;
    }


       //Open the default video camera
    VideoCapture cap1(2);

    // if not success, exit program
    if (cap1.isOpened() == false)
    {
        cout << "Cannot open the video camera" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    static_cast<int> (cap.set(CAP_PROP_FRAME_WIDTH , 480)); 
    static_cast<int> (cap.set(CAP_PROP_FRAME_HEIGHT , 360));
    
    static_cast<int> (cap1.set(CAP_PROP_FRAME_WIDTH , 480)); 
    static_cast<int> (cap1.set(CAP_PROP_FRAME_HEIGHT , 360));
    

    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

    int frame_width_new = static_cast<int>(cap1.get(CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
    int frame_height_new = static_cast<int>(cap1.get(CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

    Size frame_size(frame_width, frame_height);
    Size frame_size_acc(frame_width_new, frame_height_new);
    
    int frames_per_second = 10;

    
    //Create and initialize the VideoWriter object
    VideoWriter oVideoWriter("/home/pi/Smart-Video/Before-Crash-Camera-1.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                                               frames_per_second, frame_size, true);

    VideoWriter accVideoWriter("/home/pi/Smart-Video/After-Crash-Camera-1.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                                               frames_per_second, frame_size_acc, true);

      //Create and initialize the VideoWriter object
    VideoWriter oVideoWriter1("/home/pi/Smart-Video/Before-Crash-Camera-2.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                                               frames_per_second, frame_size, true);

    VideoWriter accVideoWriter1("/home/pi/Smart-Video/After-Crash-Camera-2.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                                               frames_per_second, frame_size_acc, true);




    

    //If the VideoWriter object is not initialized successfully, exit the program
    if (oVideoWriter.isOpened() == false)
    {
        cout << "Cannot save the video to a file" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

       //If the VideoWriter object is not initialized successfully, exit the program
    if (accVideoWriter.isOpened() == false)
    {
        cout << "Cannot save the video to a file" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

       //If the VideoWriter object is not initialized successfully, exit the program
    if (oVideoWriter1.isOpened() == false)
    {
        cout << "Cannot save the video to a file" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

       //If the VideoWriter object is not initialized successfully, exit the program
    if (accVideoWriter1.isOpened() == false)
    {
        cout << "Cannot save the video to a file" << endl;
        cin.get(); //wait for any key press
        return -1;
    }



    
    

	
    string window_name = "My Camera Feed";
    namedWindow(window_name); //create a window called "My Camera Feed"

    string window_name_1 = "My Camera Feed--1";
    namedWindow(window_name_1); //create a window called "My Camera Feed"

    

    while (myQ.size()  != 100)
    {
       
        Mat frame;
	Mat frame1;

        bool isSuccess = cap.read(frame); // read a new frame from the video camera
        bool isSuccess1 = cap1.read(frame1); // read a new frame from the video camera

      
        if (isSuccess == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }

	  if (isSuccess1 == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }
          

	myQ.push(frame);
	myQ_2.push(frame1);
        
        imshow(window_name, frame);
	
	 
	 imshow(window_name_1, frame1);

	 cout << "1st loop"<<endl;
        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by the user. Stopping the video" << endl;
            break;
        }
   
    }

    


    while(1)
      {
	  //randomNumber = (rand() % 1000) + 1;
	  
	  	buf[0] = 0x32;													// This is the register we wish to read from
	if ((write(fd, buf, 1)) != 1) {								// Send the register to read from
		printf("Error writing to i2c slave\n");
		exit(1);
	}
	

usleep(1000);
  memset(&buf,0,sizeof(buf));

	if (read(fd, buf, 6) != 6) {								// Read back data into buf[]
		printf("Unable to read from slave\n");
		exit(1);
	}
	else { x=y=z=0;
//               memset(&buf,0,sizeof(buf));

		x = ((short)buf[1]<<8) | (short) buf[0]; 
 		y = ((short)buf[3]<<8) | (short) buf[2];
 		z = ((short)buf[5]<<8) | (short) buf[4];
 		std::cout<<"x:"<<x<<"\ty:"<<y<<"\tz:"<<z<<std::endl;
	}






	  

	  Mat frame_process;
	  Mat frame_process_1;

        bool isSuccess = cap.read(frame_process); // read a new frame from the video camera
        bool isSuccess2 = cap1.read(frame_process_1); // read a new frame from the video camera

      
        if (isSuccess == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }

	 if (isSuccess2 == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }
          
          
   


	myQ.push(frame_process);
	myQ_2.push(frame_process_1);
	
	
	

	if((x<=180 && x>=-180) && (y<=180 && y>=-180) )
	  {
	      
	    myQ.pop();
	    myQ_2.pop();
	    cout << "poping old frames ..... adding new frames  "<< endl;
	  }
      

	else
	  {
	      
	      std::ofstream outfile("/home/pi/Smart-Video/gps-activate.txt"); 
	      outfile.close();
	      
	    while (accQ.size() != 100)
	   {
	     cout <<"ACC recording" <<endl;
        Mat frame_acc;
	Mat frame_acc_1;

        bool isSuccess = cap.read(frame_acc); // read a new frame from the video camera
	bool isSuccess3 = cap1.read(frame_acc_1); // read a new frame from the video camera

      
        if (isSuccess == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }

	 if (isSuccess3 == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }
          
          

	accQ.push(frame_acc);
	accQ_2.push(frame_acc_1);

	 string window_name_2 = "My Camera Feed--2";
         namedWindow(window_name_2); //create a window called "My Camera Feed"

	 string window_name_3 = "My Camera Feed--3";
         namedWindow(window_name_3); //create a window called "My Camera Feed"

	
        imshow(window_name_2, frame_acc);
	
	imshow(window_name_3, frame_acc_1);

       
        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by the user. Stopping the video" << endl;
            break;
        }
   
    }

	    
	  break;
      }
      }
    
      while(!myQ.empty())
      {
       	Mat frame_o;
	frame_o = myQ.front();
	myQ.pop();
	oVideoWriter.write(frame_o);
      }

         while(!myQ_2.empty())
      {
       	Mat frame_o1;
	frame_o1 = myQ_2.front();
	myQ_2.pop();
	oVideoWriter1.write(frame_o1);
      }


      
       while(!accQ.empty())
      {
       	Mat frame_acc;
	frame_acc = accQ.front();
	accQ.pop();
	accVideoWriter.write(frame_acc);
      }


        while(!accQ_2.empty())
      {
       	Mat frame_acc1;
	frame_acc1 = accQ_2.front();
	accQ_2.pop();
	accVideoWriter1.write(frame_acc1);
      } 

    //Flush and close the video file
    oVideoWriter.release();
    accVideoWriter.release();
    oVideoWriter1.release();
    accVideoWriter1.release();
	

	
      

    

    return 0;
}
