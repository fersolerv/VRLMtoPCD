#include <stdio.h>
#include <stdlib.h>
#include <iostream>             // for std::cout
#include <string>
#include <fstream>
#include <algorithm>
#include <pcl/io/pcd_io.h>      // header that contains the definitions for PCD I/O operations
#include <pcl/point_types.h>    // header that contains definitions for several PointT type structures
#include <pcl/visualization/cloud_viewer.h>
#include <boost/filesystem.hpp> // includes all needed Boost.Filesystem declarations to find all data base vrml files
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>
#include <iterator>
#include <vector>

using namespace std;
using namespace boost::algorithm;

void split(const string& s, char c, vector<string>& v) {
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}


int main(int argc, char* argv[])
{
        //searching in data base folder for all VRML object models to transform them later into pcl poinclouds
        //using boost library
        namespace fs = boost::filesystem;

                stringstream str;
                str << "./in";
        string sIn;
        sIn = str.str();
        fs::path dbDir(sIn); //the database dir is the dir you put the compiled file

                stringstream str2;
                str2 << "./out";
        string sOut;
        sOut = str2.str();

        fs::directory_iterator end_iter;
        typedef std::multimap<std::time_t, fs::path> result_set_t;
        result_set_t result_set;

        pcl::PointCloud<pcl::PointXYZ> cloud; //initialize point cloud
        fstream file;
        string s;
        string filename;

        //boost search in file data base
        if ( fs::exists(dbDir) && fs::is_directory(dbDir))
                {
                 for( fs::directory_iterator dir_iter(dbDir) ; dir_iter != end_iter ; ++dir_iter)
                 {
                   //open data base file
                   file.open(dir_iter->path().c_str(), ios::in);
                                         bool passado = false;

                        //while not end of file

                        while (!file.eof())
                        {
                                //get line and save as string
                                getline(file, s);
                                //if string = point [ (start of the coordinates in wrl file)
                                if((s.find("point [") != string::npos)&&(!passado))
                                {
                                        //continue reading
                                        getline(file, s);
                                        int j = 1; //int for cloud.width for unstructured point cloud
                                        size_t i = 0; //point cloud element

                                        //until string = ] (end of the coordinates)
                                        while(s.find("]") == string::npos)
                                        {
                                                //split string with seperator
                                                vector<string> temp;
                                                                                           split(trim_copy(s),' ',temp);

                                                // Fill in the cloud data for unstructured point clouds
                                                cloud.width    = j;
                                                cloud.height   = 1;
                                                cloud.is_dense = false;
                                                cloud.points.resize (cloud.width * cloud.height);

                                                for (; i < cloud.points.size (); ++i)
                                                {
                                                        //connect cartesian coordinates from splitted vector
                                                        cloud.points[i].x = atof(temp[0].c_str());
                                                        cloud.points[i].y = atof(temp[1].c_str());
                                                        cloud.points[i].z = atof(temp[2].c_str());
                                                }

                                                //read next line
                                                getline(file, s);
                                                j++;
                                        }
                                                                        passado = true;
                                }
                         }

                        //close file
                        file.close();

                        string sName = dir_iter->path().stem().string() + ".pcd";
                        string sAll = sOut + "\/" + sName;
                                          if(cloud.points.size() > 0){
                                                        pcl::io::savePCDFileASCII (sAll, cloud);
                                                }
                        std::cerr << endl << "Saved " << cloud.points.size () << " data points to " << sName;
                 }
        }
        else cout << "Data base directory not found!";


        cout << endl << endl;

        return 0;
}
