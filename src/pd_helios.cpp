/*
 * pd_helios
 * © 2019 Tim Redfern
 * licenced under the LGPL v3.0
 * see LICENSE
 */

 #include "pd_helios.h"

#define _USE_MATH_DEFINES
 
#include <cmath>

#define RAD_2_DEG 180.0 / M_PI

using namespace std;

static float getLineDegreesAtIndex(vector <point> &line,int i){
	//return angle between this point and the next
	if ((i<1)||(i>(line.size()-2))){
		return 0.0f;
	}
	//https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors

	// dot product between [x1, y1] and [x2, y2]
	// dot = x1*x2 + y1*y2
	
	float dot = ((line[i].x-line[i-1].x)*(line[i+1].x-line[i].x)) + 
		((line[i].y-line[i-1].y)*(line[i+1].y-line[i].y));    

	// determinant det = x1*y2 - y1*x2

	float det = ((line[i].x-line[i-1].x)*(line[i+1].y-line[i].y)) + 
		((line[i].y-line[i-1].y)*(line[i+1].x-line[i].x));       
	
	return atan2(det, dot)* RAD_2_DEG;  // atan2(y, x) or atan2(sin, cos)
}

int Helios::draw(std::vector <point> &points){
	line=points;
	return draw();
}

int Helios::draw(){

    int xoffs=output_centre.x;
    int yoffs=output_centre.y;

    //save data
    vector <HeliosPoint> points;

    //insert blank points to get laser to shape starting point
    for (int l=0;l<blank_num;l++){
        points.push_back(HeliosPoint{
            (uint16_t)(line[0].x+xoffs),
            (uint16_t)(line[0].y+yoffs),
            (uint8_t)(0),
            (uint8_t)(0),
            (uint8_t)(0),
            (uint8_t)255}
            );
    }

    //step through line points
    int i;
    for (i=0;i<line.size();i++){
        	            
        float angle=abs(getLineDegreesAtIndex(line,i));
        //cout << "point "<<i<<" - "<<angle<<" degrees, inserting "<<(angle>max_angle?((angle/180)*blank_num):0)<<" dwell points"<<endl;
            
        //insert dwell points to wait on a corner for laser to change direction
        int num_dwellpoints=0;

        if (i==line.size()-1) {
        	num_dwellpoints=blank_num/2;
        }
        	
        if (angle>max_angle) {
        	num_dwellpoints=((angle/180)*blank_num);
        }

    	for (int l=0;l<num_dwellpoints;l++){
            points.push_back(HeliosPoint{
                (uint16_t)(line[i].x+xoffs),
                (uint16_t)(line[i].y+yoffs),
                (uint8_t)(line[i].r*intensity/255.0),
                (uint8_t)(line[i].g*intensity/255.0),
                (uint8_t)(line[i].b*intensity/255.0),
                (uint8_t)255}
                );
        }

        if (i+1<line.size()){
	        float dist=point(line[i]).distance(point(line[i+1]));
	        int inserted=0;
	        for (float j=0;j<dist;j+=subdivide){
	        	inserted++;
	            //draw way points
	            float amt=j/dist;
	            points.push_back(HeliosPoint{
	                (uint16_t)((line[i].x*(1.0-amt))+(line[i+1].x*amt)+xoffs),
	                (uint16_t)((line[i].y*(1.0-amt))+(line[i+1].y*amt)+yoffs),
	                (uint8_t)((((line[i].r*(1.0-amt))+(line[i+1].r*amt))*intensity)/255.0),
	                (uint8_t)((((line[i].g*(1.0-amt))+(line[i+1].g*amt))*intensity)/255.0),
	                (uint8_t)((((line[i].b*(1.0-amt))+(line[i+1].b*amt))*intensity)/255.0),
	                (uint8_t)255}
	            );
	        }
	        //cout << "segment "<<i<<" - "<<dist<<" length, inserted "<<inserted<<" points"<<endl;
	    }
    }

    for (auto& p:points){ //avoid problems with excessive scale
        p.x=min((uint16_t)0xfff,p.x);
        p.y=min((uint16_t)0xfff,p.y);
    }

	if (device!=HELIOS_NODEVICE){
    	if (enabled){
    		while (!dac.GetStatus(device)); //timeout for this?
	        int ret=dac.WriteFrame(device, pps, HELIOS_FLAGS_DEFAULT, &points[0], min(HELIOS_MAX_POINTS,(int)points.size()));
	        if (ret==HELIOS_SUCCESS){
	        	return points.size();
	        }
	        return ret;
	    }
	}

	return -2;
}
