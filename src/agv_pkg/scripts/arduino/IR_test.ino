#include<ros.h>
#include<std_msgs/ByteMultiArray.h>

ros::NodeHandle node_data;
std_msgs::ByteMultiArray thermo;
ros::Publisher temp("/temp",&thermo);
char dim0_label[] = "thermo";

void setup()
{ 
  node_data.initNode();
  thermo.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  thermo.layout.dim[0].label = dim0_label;
  thermo.layout.dim[0].size = 8;
  thermo.layout.dim[0].stride = 1*8;
  thermo.layout.data_offset = 0;
  node_data.advertise(temp);
  thermo.data = (byte *)malloc(sizeof(byte)*8);
  
}
void loop()
{
  thermo.data[0]=1;
  temp.publish(&thermo);
  delay(1);
}
