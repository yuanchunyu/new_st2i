#ifndef KDTRIP_HPP
#define KDTRIP_HPP
#include <boost/lexical_cast.hpp> 
#include <iomanip>
#include <iostream>


uint32_t float2uint(float f) {
  register uint32_t t(*((uint32_t*)&f));
  return t ^ ((-(t >> 31)) | 0x80000000);
}

class KdTrip
{
public:
  
    uint32_t  pickup_time;
    uint32_t  dropoff_time;
    float   pickup_long;
   float    pickup_lat;
    float     dropoff_long;
    float   dropoff_lat;
    uint16_t  id_taxi;
    uint16_t  distance;     // in 0.01 miles unit
    uint16_t  fare_amount;  // in cents
    uint16_t  surcharge;    // in cents
    uint16_t  mta_tax;      // in cents
    uint16_t  tip_amount;   // in cents
    uint16_t  tolls_amount; // in cents
    uint8_t   payment_type;
    uint8_t   passengers;
  

  void display(){
	 
	    std::cout<<"pickup_time "<<this->pickup_time<<std::endl;
	   std::cout<<"dropoff_time "<<this->dropoff_time<<std::endl;
	     std::cout<<"pickup_long "<<this->pickup_long<<std::endl;
	   std::cout<<"pickup_lat "<<this->pickup_lat<<std::endl;
	}  
  KdTrip (std::vector<std::string> tokens){
	 //  std::cout<<this->pickup_time<<std::endl;
	 //  long int _diff=time_to_long(tokens[1]);
	
	pickup_time=time_to_long(tokens[1]);
	dropoff_time=time_to_long(tokens[2]);

      //  passengers    = boost::lexical_cast<uint8_t>( tokens[3]);
      //  distance      = (uint16_t)(boost::lexical_cast<float>( tokens[4])*100);

        //
        pickup_long   = (boost::lexical_cast<float>( tokens[5]));
        pickup_lat    = (boost::lexical_cast<float>( tokens[6]));
	//
        dropoff_long  =(boost::lexical_cast<float>( tokens[9]));
        dropoff_lat   = (boost::lexical_cast<float>( tokens[10]));

	//
       
//       payment_type  = boost::lexical_cast<uint8_t>( tokens[11]);
//
//	//
//        fare_amount   = (uint16_t)(boost::lexical_cast<float>( tokens[12])*100);
//        surcharge     = (uint16_t)(boost::lexical_cast<float>( tokens[13])*100);
//        mta_tax       = (uint16_t)(boost::lexical_cast<float>( tokens[14])*100);
//        tip_amount    = (uint16_t)(boost::lexical_cast<float>( tokens[15])*100);
//      tolls_amount  = (uint16_t)(boost::lexical_cast<float>( tokens[16])*100);

        //
        id_taxi       = 1;

        //
   
		
	} 
	uint32_t getUKey( int keyIndex)
{
  switch (keyIndex) {
  case 0:
    return pickup_time;
  case 1:
    return dropoff_time;
  case 2:
    return float2uint(pickup_long);
  case 3:
    return float2uint(pickup_lat);
  case 4:
    return float2uint(dropoff_long);
  case 5:
    return float2uint(dropoff_lat);
  case 6:
    return id_taxi;
  default:
    break;
  }
  return 0;
}

	
  long int time_to_long(std::string s1){
	 std::tm t = {};
	
    std::istringstream ss(s1);
	
  //  ss.imbue(std::locale("de_DE.utf-8"));
    ss >> std::get_time(&t, "%Y-%d-%d %H:%M:%S");
	    std::tm t1 = {};
	
    std::istringstream ss1("1970-01-01 00:00:00");
	
  //  ss.imbue(std::locale("de_DE.utf-8"));
    ss1 >> std::get_time(&t1, "%Y-%d-%d %H:%M:%S");
		uint32_t _diff;
    if (ss.fail()) {
        std::cout << "Parse failed\n";
    } else {
		
     //  std::cout << std::put_time(&t, "%c") << '\n';
	//	std::cout << std::put_time(&t1, "%c") << '\n';
		_diff = difftime(mktime(&t), mktime(&t1));
		
    }
	return _diff;
}
  };
  
 
 class KNode
{
public:
    uint64_t child_node;

    std::vector<uint32_t> median_value;
	
	 void create(int len){
		median_value.resize(len);
	 }


        // KNode(int len)
//             : median_value(len){};
};
 
#endif