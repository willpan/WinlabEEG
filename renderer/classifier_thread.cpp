#include "classifier_thread.h"

classifier_thread::classifier_thread()
{
  stopped = false;
}

void classifier_thread::run()
{
  /* Writing to output file called data.csv.
     Output format: First row, sensor names
                    1000 rows, sensor values */
  ofstream outf("data.csv");
  if(!outf)
    {
      cerr << "Error opening data.csv" << endl;
      return;
    }
  map<string, deque<int> >::iterator it;
  map<string, double > mean;
  map<string, double > std_dev;
  
  outf << "Count,";
  for(it = sensor_data.begin(); it != sensor_data.end(); it++)
      outf << (*it).first << ",";
  outf << endl;
  for(int i = 0; i < 1000; i++)
    {
      outf << i << ",";
      for(it = sensor_data.begin(); it != sensor_data.end(); it++)
	{
	  outf << (*it).second[i] << ",";
	}
      outf << endl;
    }
  outf.close();
  
  /*
    CLASSIFIER CODE GOES HERE
   */
  
  stopped = true;
}

