/*
 * HOWTO write an External for Pure data
 * (c) 2001-2006 IOhannes m zmölnig zmoelnig[AT]iem.at
 *
 * this is the source-code for the third example in the HOWTO
 * it creates an object that increments and outputs a helios
 * whenever it gets banged.
 * the helios value can be "set" to a special value, or "reset" to a default
 * an upper and lower boundary can be specified: whenever the helios crosses
 * such boundary a "bang" is emitted at the 2nd outlet and the helios value wraps
 *
 * for legal issues please see the file LICENSE.txt
 */

extern "C" {
  void helios_setup(void);
}

#include "m_pd.h"

#include "pd_helios.h"

using namespace std;

static t_class *helios_class;

typedef struct _helios {
  t_object  x_obj;       
  Helios *helios;
  t_outlet *f_out;
} t_helios;

void helios_bang(t_helios *x)
{
  t_float f=0.0f;
  
  outlet_float(x->f_out, f);
}

static void helios_list(t_helios *x, t_symbol *s, int argc, t_atom *argv)
{
  if (argc==1){
    x->helios->set_enabled(atom_getint(&argv[0]));

    //on enable, redraw and emit the number of pounts
    if (atom_getint(&argv[0])){
      int num_drawn = x->helios->draw();

      outlet_float(x->f_out, (float)num_drawn);
    }
    return;
  }

    std::vector <point> points;

    for (int i=0;i<argc/5;i++){
      points.push_back(point{
        atom_getfloat(&argv[i*5]),
        atom_getfloat(&argv[i*5+1]),
        (uint8_t)atom_getfloat(&argv[i*5+2]),
        (uint8_t)atom_getfloat(&argv[i*5+3]),
        (uint8_t)atom_getfloat(&argv[i*5+4])
      });

    }

    int num_drawn = x->helios->draw(points);

    outlet_float(x->f_out, (float)num_drawn);
}

static void redraw(t_helios *x){
  int num_drawn = x->helios->draw();
  outlet_float(x->f_out, (float)num_drawn);
}

void intensity_set(t_helios *x, t_floatarg f)
{
  int intensity=min(255,max(0,(int)f));
  x->helios->set_intensity(intensity);
  redraw(x);
}

void maxangle_set(t_helios *x, t_floatarg f)
{
  float maxangle=min(90,max(0,(int)f));
  x->helios->set_maxangle(maxangle);
  redraw(x);
}

void subdivide_set(t_helios *x, t_floatarg f)
{
  int subdivide=min(255,max(1,(int)f));
  x->helios->set_subdivide(subdivide);
  redraw(x);
}

void blanknum_set(t_helios *x, t_floatarg f)
{
  int blanknum=min(255,max(0,(int)f));
  x->helios->set_blanknum(blanknum);
  redraw(x);
}

void *helios_new(t_symbol *s, int argc, t_atom *argv)
{
  t_helios *x = (t_helios *)pd_new(helios_class);

  post("pd_helios new: opening devices");

  
  
  /* depending on the number of arguments we interprete them differently */
  switch(argc){
    default:
    case 2:{
      t_float f2=atom_getfloat(argv+1);
    }
    case 1:{
      int pps=atom_getint(argv);
      x->helios=new Helios(pps=pps);
      break;
    }
    case 0:
      x->helios=new Helios();
      break;
  }


  x->f_out = outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}


/**
 * define the function-space of the class
 */
void helios_setup(void) {

  post("pd_helios setup: starting");

  helios_class = class_new(gensym("helios"),
                            (t_newmethod)helios_new,
                            0, sizeof(t_helios),
                            CLASS_DEFAULT, 
                            A_GIMME, /* an arbitrary number of arguments 
                                      * which are of arbitrary type */
                            0);

  /* call a function when a "bang" message appears on the first inlet */
  class_addbang(helios_class, helios_bang); 

  class_addlist(helios_class, helios_list);

  class_addmethod(helios_class,
        (t_method)intensity_set, gensym("intensity"), A_DEFFLOAT, 0);

  class_addmethod(helios_class,
        (t_method)maxangle_set, gensym("maxangle"), A_DEFFLOAT, 0);

  class_addmethod(helios_class,
        (t_method)subdivide_set, gensym("subdivide"), A_DEFFLOAT, 0);

  class_addmethod(helios_class,
        (t_method)blanknum_set, gensym("blanknum"), A_DEFFLOAT, 0);

  /* set the name of the help-patch to "help-helios"(.pd) */
  class_sethelpsymbol(helios_class, gensym("help-helios"));

}
