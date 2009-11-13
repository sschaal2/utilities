/*!=============================================================================
  ==============================================================================

  \file    parm_opt_test.old.c

  \author  Stefan Schaal
  \date    February 1994

  ==============================================================================
  \remarks

      simulation of planar three joint robot arm

  ============================================================================*/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <utility.h>
#include <frame.h>
#include <mdefs.h>


/* window paramters */

#ifdef __mac_compatiblity__
#define MAGNIFICATION       2.0
#else
#define MAGNIFICATION       1.5
#endif


/* possible actions */

#define SETUP   0 
#define INIT    1
#define RUN     2
#define PAUSE   3

#define N_LINKS 3

int action = SETUP;

double l[N_LINKS+1] = {0.,1.,1.,.4};
double th[N_LINKS+1] = {0., -PI/4., 1./4.*PI, 1./4. * PI};

double x0,y0;  /* the cartesian coordinates to be kept */
double th0[N_LINKS+1] = {0., -PI/2., PI/2., PI};


/* functions */

void create_windows(void);
void initialize(void);
void draw_arm(void);
void f_dMda(double *a, double **j);
void f_M(double *a, double *c);
void f_dLda(double *a, double *g);
double f_L(double *a);



/*!*****************************************************************************
 *******************************************************************************
\note  doUserpgm
\date  10/20/91

\remarks 

      Main program for cooperation with the frame.c program. Distributes
      the different tasks according to >>action<< to other functions.

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

      none

 ******************************************************************************/
void
doUserpgm()
{

  int                 i, m, count, rc;
  char                string[100];
  double              tol = 0.000001;



  switch (action) {


  case SETUP:

	  initialize();
	  create_windows();
	  
    action = INIT;

    break;


  case INIT:
  

    action = RUN;
    
    draw_arm();
    
    break;


  case RUN:
  
  	getchar();
  
  	parm_opt(th,3, 2, tol, f_dLda, f_dMda, f_M, f_L);
  
    draw_arm();
    
    action = PAUSE;
    
    break;


  case PAUSE:
	
		draw_arm();

    break;


  default:

    printf("Unknown action! Hit return to exit ...\n");
    getchar();
    exit(-1);

  }


}


/*!*****************************************************************************
 *******************************************************************************
\note  create_windows
\date  10/20/91

\remarks 

      create necessary windows for this programs

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

      none

 ******************************************************************************/
void
create_windows(void)
{

  myWindowData        windata;
  int                 i, m;
  int                 num=0;
  char                name[100];


  windata.ident = 1;
  windata.rect.top = 40 * MAGNIFICATION;
  windata.rect.left = 1 * MAGNIFICATION;
  windata.rect.bottom = 240 * MAGNIFICATION;
  windata.rect.right = 121 * MAGNIFICATION;
  sprintf((char *) windata.title, "Robot Arm\0");
  windata.visible = TRUE;
  windata.type = documentProc;
  windata.goaway = TRUE;
  windata.goawayhide = TRUE;
  windata.grow = TRUE;
  windata.windowkind = AnimateWin;
  windata.muchdata = TRUE;
  windata.fronttimeout = 1;
  windata.backtimeout = 1;
  windata.resolution_x = 10000.;
  windata.resolution_y = 10000.;
  windata.myworld.top = 2.;
  windata.myworld.left = 0.0;
  windata.myworld.bottom = -2.0;
  windata.myworld.right = 2.;
  windata.margin_x = .01 * (windata.myworld.right - windata.myworld.left);
  windata.margin_y = .01 * (windata.myworld.top - windata.myworld.bottom);
  windata.draw_axes = TRUE;
  windata.draw_frame = TRUE;

  gen_window(windata);


}

/*!*****************************************************************************
 *******************************************************************************
\note  draw_arm
\date  10/20/91

\remarks 

      draws all oscillators 

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

      none

 ******************************************************************************/
void
draw_arm(void)
{
	
	
	if (StartOutput(1)){
	
		MoveTo(Sx(0.0),Sy(0.0));
		LineTo(Sx(l[1]*cos(th[1])),Sy(l[1]*sin(th[1])));
		LineTo(Sx(l[1]*cos(th[1])-l[2]*cos(th[1]+th[2])),
			Sy(l[1]*sin(th[1])+l[2]*sin(th[1]+th[2])));
		LineTo(Sx(l[1]*cos(th[1])-l[2]*cos(th[1]+th[2])+l[3]*cos(th[1]+th[2]+th[3])),
			Sy(l[1]*sin(th[1])+l[2]*sin(th[1]+th[2])+l[3]*sin(th[1]+th[2]+th[3])));
		
		StopOutput(1);
		
	}
		
}

/*!*****************************************************************************
 *******************************************************************************
\note  initialize
\date  10/20/91

\remarks 

      setup all necessary parameters

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

      none

 ******************************************************************************/
void 
initialize(void)

{

	int             i,j,m,n;
	double          vec[3];
	
	f_M(th,vec);
	
	x0 = vec[1];
	y0 = vec[2];
		
}

/*!*****************************************************************************
 *******************************************************************************
\note  f_L
\date  10/20/91

\remarks 

      calculates the simple costs (without constraints)

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

 \param[in]     a: vector of parameters
      returns the cost;

 ******************************************************************************/
double 
f_L(double *a)

{

	int    i;
	double cost=0.;
	
	for (i=1; i<=N_LINKS; ++i) {
		cost += 0.5*sqr(th[i]-th0[i]);
	}
	
	return cost;

}

/*!*****************************************************************************
 *******************************************************************************
\note  f_dLda
\date  10/20/91

\remarks 

      calculates the gradient of the costs

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

 \param[in]     a: vector of parameters
 \param[out]    g: gradient vector

 ******************************************************************************/
void 
f_dLda(double *a, double *g)

{

	int    i;
	
	for (i=1; i<=N_LINKS; ++i) {
		g[i] = (th[i]-th0[i]);
	}

}

/*!*****************************************************************************
 *******************************************************************************
\note  f_M
\date  10/20/91

\remarks 

      calculates the constraints; a proper fulfillment of the constraints
      results in 0 result of this function for each constraint

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

 \param[in]     a: vector of parameters
 \param[out]    c: constraint result vector

 ******************************************************************************/
void 
f_M(double *a, double *c)

{

	int    i;
	
	c[1] = l[1]*cos(a[1])-l[2]*cos(a[1]+a[2])+l[3]*cos(a[1]+a[2]+a[3])-x0;
	c[2] = l[1]*sin(a[1])+l[2]*sin(a[1]+a[2])+l[3]*sin(a[1]+a[2]+a[3])-y0;
}

/*!*****************************************************************************
 *******************************************************************************
\note  f_dMda
\date  10/20/91

\remarks 

      calculates the Jacobian of the constraints

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

 \param[in]     a: vector of parameters
 \param[out]    j: constraint jacobian matrix

 ******************************************************************************/
void 
f_dMda(double *a, double **j)

{

		
	j[1][1] = -1.*Sin(a[1]) + 1.*Sin(a[1] + a[2]) - 1.*Sin(a[1] + a[2] + a[3]);
	j[1][2] = 1.*Sin(a[1] + a[2]) - 1.*Sin(a[1] + a[2] + a[3]);
	j[1][3] = -1.*Sin(a[1] + a[2] + a[3]);
	j[2][1] = 1.*Cos(a[1]) + 1.*Cos(a[1] + a[2]) + 1.*Cos(a[1] + a[2] + a[3]);
	j[2][2] = 1.*Cos(a[1] + a[2]) + 1.*Cos(a[1] + a[2] + a[3]);
	j[2][3] = 1.*Cos(a[1] + a[2] + a[3]);;
	
}

