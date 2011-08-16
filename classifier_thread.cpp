#include "classifier_thread.h"

classifier_thread::classifier_thread()
{
  stopped = false;
  
}

void classifier_thread::run()
{
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

  /*
  outf << "\n\nSensor:\t";
  for(it = sensor_data.begin(); it != sensor_data.end(); it++)
    {
      double mu = 0;
      
      for(int n = 0; n < (*it).second.size(); n++)
	{
	  mu = mu + (*it).second[n]; 
	}
      mu = mu/(*it).second.size();
      
      int var = 0;
      for(int n = 0; n < (*it).second.size(); n++)
	{
	  var = var + ( (*it).second[n] - mu ) * ( (*it).second[n] - mu);
	}
      double sig = sqrt(var/(*it).second.size() );
      mean[(*it).first] = mu;
      std_dev[(*it).first] = sig;
      outf << (*it).first << "\t";
	}
  cout << "2\n";
  outf << "\nMean:\t";
  for(it = sensor_data.begin(); it != sensor_data.end(); it++)
    {
      outf << mean[(*it).first] << "\t";
    }

  outf << "\nSTD:\t";
  for(it = sensor_data.begin(); it != sensor_data.end(); it++)
    {
      outf << std_dev[(*it).first] << "\t";
    }
  outf << endl;
  */
  outf.close();
  
  stopped = true;
}

