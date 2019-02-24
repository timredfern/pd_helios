#ifndef Helios_h
#define Helios_h
#define HELIOS_VERSION 0.1
#define HELIOS_NODEVICE -1

#define DEFAULT_SUBDIVIDE 15
#define DEFAULT_BLANK_NUM 8
#define DEFAULT_MAX_ANGLE 15.0f

#include "sdk/HeliosDac.h"

#include <iostream>
#include <math.h>

//class that resembles a HeliosPoint but with floats for coords
class point {
public:
	point(){
		x=y=0;
		r=g=b=0;
	}
	point(float _x, float _y){
		x=_x;
		y=_y;
	}
	point(int _x, int _y){
		x=_x;
		y=_y;
	}
	point(float _x, float _y,uint8_t _r,uint8_t _g,uint8_t _b){
		x=_x;
		y=_y;
		r=_r;
		g=_g;
		b=_b;
	}
	point(const point &p){
		x=p.x;
		y=p.y;
		r=p.r;
		g=p.g;
		b=p.b;
	}
	point operator+ (const point& p){
		point np(x+p.x,y+p.y);
		return np;
	}
	point& operator+= (const point& p){
		x+=p.x;
		y+=p.y;
		return *this;
	}
	bool operator!=( const point&  p){
		return x==p.x&&y==p.y;
	}
	float distance(const point&  p){
		return pow((pow(p.x-x,2)+pow(p.y-y,2)),0.5);
	}
	float x,y;
	std::uint8_t r,g,b;	
};

class colour {
public:
	colour(){
		r=g=b=0;
	}
	colour(uint8_t _r,uint8_t _g, uint8_t _b){
		r=_r;
		g=_g;
		b=_b;
	}
	uint8_t r,g,b;
};

class Helios
{
public:
    
    Helios(int _pps=20000,int _intensity=255,int _device = 0,int _subdivide=DEFAULT_SUBDIVIDE,int _blank_num=DEFAULT_BLANK_NUM,int _max_angle=DEFAULT_MAX_ANGLE,bool _enabled=false)
    {
        int numdevices=dac.OpenDevices();
        for (int i=0;i<numdevices;i++){
            std::cout << "Helios v "<<HELIOS_VERSION<<": found laser DAC: firmware v "<<dac.GetFirmwareVersion(i)<<std::endl;
        }
        if (!numdevices){
            std::cout << "Helios v "<<HELIOS_VERSION<<": no devices found"<<std::endl;

        }
        if (_device>=numdevices){
            std::cout << "Helios v "<<HELIOS_VERSION<<": could not open device "<<_device<<std::endl;
            device=HELIOS_NODEVICE;
        }
        else {
            device=_device;
            pps=_pps;
            intensity=_intensity;
            subdivide=_subdivide;
            blank_num=_blank_num;
            max_angle=_max_angle;
            enabled=_enabled;
            dac.SetShutter(device,false);
        }
        output_centre=point(0x800,0x800);
    }
    
    ~Helios()
    {
        dac.CloseDevices();
    }

    void set_centre(point c){
        point cp=c+point(0x800,0x800);
        if (cp!=output_centre){
            output_centre=cp;
            //std::cout << "Helios v "<<HELIOS_VERSION<<": set output centre to "<<output_centre.x<<","<<output_centre.y<<std::endl;
        }
    }
    void set_pts(int n){
        if (n!=pps){
            pps=n;
            //std::cout << "Helios v "<<HELIOS_VERSION<<": set point output to "<<pps<<std::endl;
        }
    }
    void set_intensity(int i){
        if (i>-1&&i<256&&i!=intensity){
            intensity=i;
            //std::cout << "Helios v "<<HELIOS_VERSION<<": set intensity to "<<intensity<<std::endl;
        }
    }
    void set_subdivide(int i){
        if (i>0&&i<256&&i!=subdivide){
            subdivide=i;
            //std::cout << "Helios v "<<HELIOS_VERSION<<": set subdivide to "<<subdivide<<std::endl;
        }
    }
    void set_blanknum(int i){
        if (i>-1&&i<256&&i!=blank_num){
            blank_num=i;
            //std::cout << "Helios v "<<HELIOS_VERSION<<": set blank_num to "<<blank_num<<std::endl;
        }
    }
    void set_maxangle(float f){
        if (f>=0.0f&&f<90.0f&&f!=max_angle){
            max_angle=f;
            //std::cout << "Helios v "<<HELIOS_VERSION<<": set max_angle to "<<max_angle<<std::endl;
        }
    }
    void set_enabled(bool e){
        if (e!=enabled){
        	enabled=e;
            //std::cout << "Helios v "<<HELIOS_VERSION<<": "<<(enabled?"enabled":"disabled")<<std::endl;
        	if (!enabled){
        		dac.Stop(device);
        	}
        	
        }
    }
    int get_pts(){
        return pps;
    }

    //draw a new set of points
    int draw(std::vector <point> &points);

    //repeat previous drawing (intensity is re-calculated)
    int draw();

    private:

        int device;
        HeliosDac dac;
        int pps;
        int intensity;

        bool enabled;

        int subdivide,blank_num;
        float max_angle;

        point output_centre;

        std::vector <point> line;
    
};

#endif //Helios_h