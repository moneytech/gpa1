#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vmw_texture.h"
#include "vmw_glfont.h"

#include "gl_helper.h"

#include "guinea_pig.h"
#include "spaceship.h"
#include "matrix_math.h"

#include "keyboard.h"

#include "main_menu.h"

#include "textures.h"
#include "terrain.h"

#include "game_state.h"

    /* As much as I have memorized... */
#define PI 3.141592653589793238462643383279502884197169399
  



extern int use_lighting;
extern int show_fps;

   GLuint leonard;
extern GLuint spaceships[2];
   GLuint terrain[NUM_TERRAINS];




   /* icky globals */
int show_menu=0;

float camera_direction=90*(180.0/PI),camerax=-10.0,cameray=0,cameraz=5.0;
    
int turn_right=0,turn_left=0,draw_splash=0,accelerating=0;

   
#define D2R(x) ((x*PI)/180.0)


map_element world_map[40][40];


void render_world(game_state_type *gs) {
   
   int i,j,distance_seen,land_type,tempx,tempy;
   GLfloat shadow_width;
     GLfloat light_position[]={5.0,5.0,100.0,0.0
     };
   
//     GLfloat light_ambient[]={0.5,0.5,0.5,1.0
//     };
       GLfloat lmodel_ambient[]={0.9,0.9,0.9,1.0
       };
   
   
       GLfloat white_light[]={1.0,1.0,1.0,1.0
       };
   
   
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   glLoadIdentity();
   gluLookAt(camerax,cameray,cameraz,
	     0.0,0.0,0.0,
	     0.0,0.0,1.0);
   
   if (use_lighting) {
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//      glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
      glLightfv(GL_LIGHT0, GL_SPECULAR,white_light);
      
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
//      glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);

      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
   }
   

   
     {
	GLfloat default_a[]={0.6,0.6,0.6,1.0};
        GLfloat default_d[]={   0.8,0.8,0.8,1.0};
	GLfloat default_s[]={0.0,0.0,0.0,1.0};
	GLfloat default_e[]={0.0,0.0,0.0,1.0
	};
	
        glMaterialfv(GL_FRONT,GL_AMBIENT,default_a);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,default_d);
	glMaterialfv(GL_FRONT,GL_SPECULAR,default_s);
	glMaterialfv(GL_FRONT,GL_EMISSION,default_e);
     }

   glPushMatrix();
   glRotatef( (360-gs->direction),0,0,1);   
   glTranslatef(-gs->pigx,-gs->pigy,-gs->pigz);

   
   glColor3f(0.4102,0.543,0.1328);
   
   glEnable(GL_TEXTURE_2D);
       /* Draw Sky */

   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
   glBindTexture(GL_TEXTURE_2D,textures[SKY_TEXTURE]);
   glBegin(GL_QUADS);
   
#define SKY_DISTANCE 60
   
          /* Back */
      glNormal3f(-1.0,0.0,0.0);
   
      glTexCoord2f(0.0, 0.0);
      glVertex3f(SKY_DISTANCE,-SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(SKY_DISTANCE,-SKY_DISTANCE,SKY_DISTANCE);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(SKY_DISTANCE,SKY_DISTANCE,SKY_DISTANCE);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(SKY_DISTANCE,SKY_DISTANCE,-SKY_DISTANCE);
   
         /* Right */
   
      glNormal3f(0.0,1.0,0.0);
   
      glTexCoord2f(0.0, 0.0);
      glVertex3f(SKY_DISTANCE,-SKY_DISTANCE,SKY_DISTANCE);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(SKY_DISTANCE,-SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(-SKY_DISTANCE,-SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(-SKY_DISTANCE,-SKY_DISTANCE,SKY_DISTANCE);
   
         /* Front */
      glNormal3f(1.0,0.0,0.0);
   
      glTexCoord2f(0.0, 0.0);
      glVertex3f(-SKY_DISTANCE,-SKY_DISTANCE,SKY_DISTANCE);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(-SKY_DISTANCE,-SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(-SKY_DISTANCE,SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(-SKY_DISTANCE,SKY_DISTANCE,SKY_DISTANCE);
   
         /* Left */
   
       glNormal3f(0.0,-1.0,0.0);
   
      glTexCoord2f(0.0, 0.0);
      glVertex3f(-SKY_DISTANCE,SKY_DISTANCE,SKY_DISTANCE);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(-SKY_DISTANCE,SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(SKY_DISTANCE,SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(SKY_DISTANCE,SKY_DISTANCE,SKY_DISTANCE);
   
   
          /* top */
      glNormal3f(0.0,0.0,-1.0);
   
      glTexCoord2f(0.0, 0.0);
      glVertex3f(SKY_DISTANCE,SKY_DISTANCE,SKY_DISTANCE);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(SKY_DISTANCE,-SKY_DISTANCE,SKY_DISTANCE);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(-SKY_DISTANCE,-SKY_DISTANCE,SKY_DISTANCE);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(-SKY_DISTANCE,SKY_DISTANCE,SKY_DISTANCE);
   
             /* bottom */
      glNormal3f(0.0,0.0,1.0);
   
      glTexCoord2f(0.0, 0.0);
      glVertex3f(SKY_DISTANCE,SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(SKY_DISTANCE,-SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(-SKY_DISTANCE,-SKY_DISTANCE,-SKY_DISTANCE);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(-SKY_DISTANCE,SKY_DISTANCE,-SKY_DISTANCE);
   
   glEnd();

//   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   
   
   
       /* Draw World */   


   
       /* 25.5 = "radius of world" */
    distance_seen=(int)sqrt(2*25.5*cameraz);

    for(i=-distance_seen;i<=distance_seen;i++) {
       for(j=-distance_seen;j<=distance_seen;j++) {
	    
	  tempx=gs->gridx+i;
	  if (tempx<0) tempx+=40;
	  if (tempx>39) tempx-=40;
	  tempy=gs->gridy+j;
	  if (tempy<0) tempy+=40;
	  if (tempy>39) tempy-=40;
	    
	  land_type=world_map[tempx][tempy].terrain_type;

	  glPushMatrix();
	  glTranslatef( (i*4)+2,(j*4)+2,0); 
	  glRotatef(world_map[tempx][tempy].rotation,0,0,1);
          glCallList(terrain[land_type]);
	  
	  if (!gs->in_spaceship) {
	     if ((tempx==gs->spaceship_gridx) && (tempy==gs->spaceship_gridy)) {
		glTranslatef(gs->spaceship_xoffset,gs->spaceship_yoffset,0.75);
		glRotatef(gs->spaceship_direction,0,0,1);
		glCallList(spaceships[0]);
	     }
	  }
	  
	  glPopMatrix();

       }
    }

   

    glPopMatrix();

    glPushMatrix(); 

    if (gs->in_spaceship) {  
          /* draw shadow */
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,textures[SHADOW_TEXTURE]);   
      
       glBegin(GL_QUADS);  
          shadow_width=(1.5+(-gs->pigz/5.0))/2.0;
      
          glNormal3f(0.0,0.0,1.0);
   
          glTexCoord2f(0.0, 0.0);
          glVertex3f(-0.75,-shadow_width,-gs->pigz+0.1);
      
          glTexCoord2f(1.0, 0.0);
          glVertex3f(0.75,-shadow_width,-gs->pigz+0.1);
      
          glTexCoord2f(1.0, 1.0);
          glVertex3f(0.75,shadow_width,-gs->pigz+0.1);
      
          glTexCoord2f(0.0, 1.0);
          glVertex3f(-0.75,shadow_width,-gs->pigz+0.1);         
      
       glEnd();
      
       if ((draw_splash) && (gs->pigz<0.75)) {
          glBindTexture(GL_TEXTURE_2D,textures[SPLASH_TEXTURE]);   
      
          glBegin(GL_QUADS);
               
             glNormal3f(0.0,0.0,1.0);
   
             glTexCoord2f(0.0, 0.0);
             glVertex3f(-8.0,-1.0,-0.85);
      
             glTexCoord2f(1.0, 0.0);
             glVertex3f(-8.0,1.0,-0.85);
      
             glTexCoord2f(1.0, 1.0);
             glVertex3f(-1.45,1.0,-0.85);
      
             glTexCoord2f(0.0, 1.0);
             glVertex3f(-1.25,-1.0,-0.85);         
          glEnd();
       }
       if (turn_right) {
          glRotatef(20,1.0,0.0,0.0);
       }
       if (turn_left) {
	  glRotatef(-20,1.0,0.0,0.0);
       }
      
       if (accelerating) glCallList(spaceships[1]);
       else glCallList(spaceships[0]);
    }
    else {
       glRotatef(90,1,0,0);
       glCallList(leonard);
    }
           
    glPopMatrix();
      
   
   
//   printf("%.2f %.2f %.2f %i %i\n",gs->pigx,gs->pigy,gs->pigz,gs->gridx,gs->gridy);
   

       /* Prototype to display overlay text */
    if (show_menu) {
       glDisable(GL_LIGHTING);
       glMatrixMode(GL_PROJECTION);
       glLoadIdentity();
       gluOrtho2D(0,320,0,200);
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();  
       
       glBegin(GL_QUADS);
          glNormal3f(0,0,1);
          glVertex3f(5,5,-0.1);
          glVertex3f(100,5,-0.1);
          glVertex3f(100,100,-0.1);
          glVertex3f(5,100,-0.1);
       glEnd();
      
       glColor3f(1.0,0.0,0.0);
       glRasterPos3f(5,5,0);
       vmwGLString("A VMW SOFTWARE PRODUCTION",font);
       
       reshape(640,480);
    }
   
       /* Flush it out to the device */
    glFlush();
    SDL_GL_SwapBuffers();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}



int handle_ss_keyboard(int keyspressed,game_state_type  *gs,float scale) {
  

    float x_offset,y_offset;
    int tempx,tempy;
   
    draw_splash=0;
    turn_right=0; turn_left=0; accelerating=0;
    
    if (keyspressed&RIGHT_PRESSED) {
       gs->direction-=10*scale;
       turn_right=1;
    }
    if (keyspressed&LEFT_PRESSED) {
       gs->direction+=10*scale;
       turn_left=1;
    }
    if (keyspressed&ACTION1_PRESSED) {
       gs->pigy+=sin( (gs->direction*PI)/180.0)*scale;
       gs->pigx+=cos( (gs->direction*PI)/180.0)*scale;
       accelerating=1;
    }
    if (keyspressed&ACTION2_PRESSED) {
       gs->pigy-=sin( (gs->direction*PI)/180.0)*scale;
       gs->pigx-=cos( (gs->direction*PI)/180.0)*scale;      
       accelerating=1;
    }
    if ((keyspressed>>16)&CONFIRM_PRESSED) {
       	     printf("TRYING TO GET OUT: %i,%i,%.2f,%.2f %i,%i,%.2f,%.2f\n",
		    gs->gridx,gs->gridy,gs->pigx,gs->pigy,
		    gs->spaceship_gridx,gs->spaceship_gridy,gs->spaceship_xoffset,gs->spaceship_yoffset);
       if (world_map[gs->gridx][gs->gridy].terrain_type==GRASS_TERRAIN) {
          gs->in_spaceship=0;
	  gs->spaceship_gridx=gs->gridx;
	  gs->spaceship_gridy=gs->gridy;
	  gs->spaceship_direction=gs->direction;
	  gs->spaceship_xoffset=gs->pigx;
	  gs->spaceship_yoffset=gs->pigy;
	  gs->pigz=0.5;
       }
       
    }
    if (keyspressed&UP_PRESSED) {
       gs->pigz+=1.0*scale;
    }
    if (keyspressed&DOWN_PRESSED) {
       gs->pigz-=1.0*scale;
    }
          
	     /* Collision detection */
	  
	  
	  /* spaceship is 3x1.5 more or less */
	  /* so check all 4 corners */
          /* Double check these later */
	  
	  x_offset=  sin(((gs->direction-26.565)*PI)/180.0)*1.677;
	  y_offset=  cos(((gs->direction-26.565)*PI)/180.0)*1.677;
	  
	  /* lower right */
	  
	  tempx=gs->gridx;  tempy=gs->gridy;
	  if ((x_offset+gs->pigx)>=2.0) {
	     tempx++;
	     if (tempx>=40) tempx=0;
	  }
	  if ((y_offset+gs->pigy)>=2.0) {
	     tempy++;
	     if (tempy>=40) tempy=0;
	  }
	  if ((gs->pigx-x_offset)<=-2.0) {
	     tempx--;
	     if (tempx<0) tempx=39;
	  }
	  if ((gs->pigy-y_offset)<=-2.0) {
	     tempy--;
	     if (tempy<0) tempy=39;
	  }
	  if (gs->pigz<terrain_heights[world_map[tempx][tempy].terrain_type]+0.89) { 

//	     printf("SPLASH %f %f\n",gs->pigz,terrain_heights[world_map[tempx][tempy].terrain_type]+0.9);
	     gs->pigz=terrain_heights[world_map[tempx][tempy].terrain_type]+0.9;
	     draw_splash=1;
	  }	  	  
	  
	  
	  	  /* lower left */
	  
	  tempx=gs->gridx;  tempy=gs->gridy;
	  if ((gs->pigx-x_offset)>=2.0) {
	     tempx++;
	     if (tempx>=40) tempx=0;
	  }
	  if ((y_offset+gs->pigy)>=2.0) {
	     tempy++;
	     if (tempy>=40) tempy=0;
	  }
	  if ((gs->pigx+x_offset)<=-2.0) {
	     tempx--;
	     if (tempx<0) tempx=39;
	  }
	  if ((gs->pigy-y_offset)<=-2.0) {
	     tempy--;
	     if (tempy<0) tempy=39;
	  }
	  if (gs->pigz<terrain_heights[world_map[tempx][tempy].terrain_type]+0.89) { 
	     gs->pigz=terrain_heights[world_map[tempx][tempy].terrain_type]+0.9;
	     draw_splash=1;
	  }	  	  
	  
	  	  /* upper left */
	  
	  tempx=gs->gridx;  tempy=gs->gridy;
	  if ((gs->pigx-x_offset)>=2.0) {
	     tempx++;
	     if (tempx>=40) tempx=0;
	  }
	  if ((gs->pigy-y_offset)>=2.0) {
	     tempy++;
	     if (tempy>=40) tempy=0;
	  }
	  if ((gs->pigx+x_offset)<=-2.0) {
	     tempx--;
	     if (tempx<0) tempx=39;
	  }
	  if ((gs->pigy+y_offset)<=-2.0) {
	     tempy--;
	     if (tempy<0) tempy=39;
	  }
	  if (gs->pigz<terrain_heights[world_map[tempx][tempy].terrain_type]+0.89) { 
	     gs->pigz=terrain_heights[world_map[tempx][tempy].terrain_type]+0.9;
	     draw_splash=1;
	  }	  	  
	  
	  
	  	  /* upper right */
	  
	  tempx=gs->gridx;  tempy=gs->gridy;
	  if ((x_offset+gs->pigx)>=2.0) {
	     tempx++;
	     if (tempx>=40) tempx=0;
	  }
	  if ((gs->pigy-y_offset)>=2.0) {
	     tempy++;
	     if (tempy>=40) tempy=0;
	  }
	  if ((gs->pigx-x_offset)<=-2.0) {
	     tempx--;
	     if (tempx<0) tempx=39;
	  }
	  if ((gs->pigy+y_offset)<=-2.0) {
	     tempy--;
	     if (tempy<0) tempy=39;
	  }
	  if (gs->pigz<terrain_heights[world_map[tempx][tempy].terrain_type]+0.89) { 
	     gs->pigz=terrain_heights[world_map[tempx][tempy].terrain_type]+0.9;
	     draw_splash=1;
	  }	  	  
       
	  
	  
	  if (gs->pigz>5) gs->pigz=5;
	  if (gs->pigx>=2.0) { 
	     gs->gridx++; 
	     gs->pigx-=4.0;
	  }
	  if (gs->pigx<=-2.0) {
	     gs->gridx--;
	     gs->pigx+=4.0;
	  }
	  if (gs->pigy>=2.0) {
	     gs->gridy++;
	     gs->pigy-=4.0;
	  }
	  if (gs->pigy<=-2.0) {
	     gs->gridy--;
	     gs->pigy+=4.0;
	  }
	      
	  if (gs->gridx>=40) gs->gridx=0;
	  if (gs->gridx<0) gs->gridx=39;
	  if (gs->gridy>=40) gs->gridy=0;
	  if (gs->gridy<0) gs->gridy=39;

    return 0;
}

int handle_gp_keyboard(int keyspressed, game_state_type *gs, float scale) {


          if (keyspressed&RIGHT_PRESSED) {
	     gs->direction-=10;
          }
          if (keyspressed&LEFT_PRESSED) {
             gs->direction+=10;    
          }
          if (keyspressed&UP_PRESSED) {
             gs->pigy+=sin( (gs->direction*PI)/180.0);
             gs->pigx+=cos( (gs->direction*PI)/180.0);
          }
          if (keyspressed&DOWN_PRESSED) {
             gs->pigy-=sin( (gs->direction*PI)/180.0);
             gs->pigx-=cos( (gs->direction*PI)/180.0);       
          }
             /* should monitor x_offset,y_offset too */
          if ( (keyspressed>>16)&CONFIRM_PRESSED) {
	     printf("TRYING TO GET IN: %i,%i,%.2f,%.2f %i,%i,%.2f,%.2f\n",
		    gs->gridx,gs->gridy,gs->pigx,gs->pigy,
		    gs->spaceship_gridx,gs->spaceship_gridy,gs->spaceship_xoffset,gs->spaceship_yoffset);
	     if ((gs->spaceship_gridx==gs->gridx) &&  
	         (gs->spaceship_gridy==gs->gridy)) {
		gs->in_spaceship=1;
		gs->pigx=gs->spaceship_xoffset;
		gs->pigy=gs->spaceship_yoffset;
		gs->direction=gs->spaceship_direction;
		gs->pigz=0.75;
	     }
	  }
   
	  if (keyspressed&ACTION1_PRESSED) {
          }
          if (keyspressed&ACTION2_PRESSED) {
          }
       
	     	             if (gs->pigx>=2.0) { 
				gs->gridx++; 
				gs->pigx-=4.0;
				
			     }
	                     if (gs->pigx<=-2.0) {
				gs->gridx--;
				gs->pigx+=4.0;
			     }
	                     if (gs->pigy>=2.0) {
				gs->gridy++;
				gs->pigy-=4.0;
			     }
	                     if (gs->pigy<=-2.0) {
				gs->gridy--;
				gs->pigy+=4.0;
			     }
	      
	                     if (gs->gridx>=40) gs->gridx=0;
	                     if (gs->gridx<0) gs->gridx=39;
	                      
	                     if (gs->gridy>=40) gs->gridy=0;
	                     if (gs->gridy<0) gs->gridy=39;
	     
    return 0;      
}
       
       
       

int do_world(int xsize,int ysize,game_state_type *gs) {

   
    int frames=0,fps_msecs=0,old_fps_msecs=0,keyspressed=0;
    int old_msecs=0,new_msecs=0;
   
    float scale=0.0;

    char key_alpha;
   
       /* Init screen and keyboard */
    reshape(xsize,ysize);
    check_keyboard(&key_alpha,1);

       /* Setup display Lists */
    leonard=setup_pig_list(0,0);
    spaceships[0]=setup_spaceship(0);
    spaceships[1]=setup_spaceship(1);
    gs->whoami=leonard;
   
    setup_map();
    setup_terrain();  
    
    old_msecs=SDL_GetTicks();

    gs->in_spaceship=0;
    gs->pigz=0.5;
   
    while (1) {
       
       frames++;
       new_msecs=SDL_GetTicks();
         
          /* * how far to go in 1s */
       scale=(new_msecs-old_msecs)/50.0;
       old_msecs=new_msecs;

       if (frames%100==0) {
	  old_fps_msecs=fps_msecs;
	  fps_msecs=SDL_GetTicks();
          printf("FPS=%.2f\n", frames/((fps_msecs-old_fps_msecs)/1000.0));	 
	  frames=0;
       }

          /* Check for input events */
       keyspressed=check_keyboard(&key_alpha,0);

       if (keyspressed&PAN_LEFT_PRESSED) {
          camera_direction-=(10.0*PI)/180;
	  if (camera_direction<0.0) camera_direction+=2*PI;
	  camerax=10*sin(camera_direction);
	  cameray=10*cos(camera_direction);
       }
       if (keyspressed&PAN_RIGHT_PRESSED) {
	  camera_direction+=(10.0*PI)/180;
	  if (camera_direction>2*PI) camera_direction-=2*PI;
	  camerax=10*sin(camera_direction);
	  cameray=10*cos(camera_direction);
       }
          /* Nonstandard key events */
       switch(key_alpha) {
          case 'l': use_lighting=!(use_lighting); break;
	  case 'm': cameraz-=0.5; break;
	  case 'i': cameraz+=0.5; break;
          case 'o': show_menu=!show_menu; break;
       }

       if (keyspressed&MENU_PRESSED) {
	  return -1;
       }
   
       if (gs->in_spaceship) handle_ss_keyboard(keyspressed,gs,scale);
       else handle_gp_keyboard(keyspressed,gs,scale);
       

       render_world(gs);       
/* Emulate low frame-rates */
//usleep(100000); 
    }
    return 1;
}