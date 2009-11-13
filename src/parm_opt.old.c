/*!=============================================================================
  ==============================================================================

  \file    parm_opt.old.c

  \author  Stefan Schaal
  \date    February 1994

  ==============================================================================
  \remarks

      program for parameter optimization with hard constraints, e.g.,
      typically modeled by Lagrange multipliers. Both Newton's method
      and normal gradient method are implemented

  ============================================================================*/
#include <stdio.h>
#include <math.h>
#include <utility.h>


/*!*****************************************************************************
 *******************************************************************************
\note  parm_opt
\date  10/20/91

\remarks 
		this is the major optimzation program

 *******************************************************************************
 Function Parameters: [in]=input,[out]=output

 \param[in]     tol      : error tolernance to be achieved
 \param[in]     n_parm   : number of parameters to be optimzed
 \param[in]     n_con    : number of contraints to be taken into account
 \param[in]     f_dLda   : function which calculates the derivative of the 
      							optimziation criterion with respect to the parameters;
      							must return vector
 \param[in]     f_dMda   : function which calculates the derivate of the 
      							constraints with respect to parameters
      							must return matrix
 \param[in]     f_M      : constraints function, must always be formulted to
      							return 0 for properly fulfilled constraints
 \param[in]     f_L      : function to calculate simple cost (i.e., constraint
      							cost NOT included), the constraint costs are added
      							by this program automatically, the function returns
      							a double scalar
 \param[in,out] a        : initial setting of parameters and also return of
      							optimal value (must be a vector, even if scalar)
 \param[out]    final_cost: the final cost
 \param[out]    err      : the sqrt of the squared error of all constraints
      				
      NOTE: - program returns TRUE if everything correct, otherwise FALSE
      			- always minimizes the cost!!!
      			-	algorithms come from Dyer McReynolds

 ******************************************************************************/
int
parm_opt(double *a,int n_parm, int n_con, double tol, void (*f_dLda)(), 
	void (*f_dMda)(), void (*f_M)(), double (*f_L)(), double *final_cost, double *err)

{

	double cost= HUGE_VAL;
	double last_cost = 0.0;
	double *mult; /* this is the vector of Lagrange mulitplier */
	double **dMda, **dMda_t;
	double *dLda;
	double *K; /* the error in the constraints */
	double eps = 0.5; /* the learning rate */
	double **aux_mat; /* needed for inversion of matrix */
	double *aux_vec;
	double *new_a;
	int    rc=TRUE;
	long   count = 0;
	int    last_sign = 1;
	int    pending1 = FALSE, pending2 = FALSE;
	
	mult    = my_vector(1,n_con);
	dLda    = my_vector(1,n_parm);
	dMda    = my_matrix(1,n_con,1,n_parm);
	dMda_t  = my_matrix(1,n_parm,1,n_con);
	K       = my_vector(1,n_con);
	aux_mat = my_matrix(1,n_con,1,n_con);
	aux_vec = my_vector(1,n_con);
	new_a   = my_vector(1,n_parm);
	
	
	while (fabs(cost-last_cost) > tol) {
	
		pending1 = FALSE;
		pending2 = FALSE;
	
	AGAIN:
	
		++count;
	
		/* calculate the current Lagrange multipliers */
		
		(*f_M)(a,K);       /* takes the parameters, returns residuals */
		(*f_dMda)(a,dMda); /* takes the parameters, returns the Jacobian */
		(*f_dLda)(a,dLda); /* takes the parameters, returns the gradient */
		
			
		/* the mulitpliers are updated according:
		
								mult = (dMda dMda_t)^(-1) (K/esp - dMda dLda_t)       */
								
		mat_trans(dMda,n_con,n_parm,dMda_t);
		mat_mult(dMda,n_con,n_parm,dMda_t,n_parm,n_con,aux_mat);
		
		if (!my_inv_ludcmp(aux_mat, n_con, aux_mat)) {
			rc = FALSE;
			break;
		}
		
		mat_vec_mult(dMda,n_con,n_parm,dLda,n_parm,aux_vec);
		
		vec_mult_scalar(K,n_con,1./eps,K);
		
		vec_sub(K,aux_vec,n_con,aux_vec);
		
		mat_vec_mult(aux_mat,n_con,n_con,aux_vec,n_con,mult);
		
		if (count == 1) {
		
			(*f_M)(a,K);
			last_cost  = (*f_L)(a);
			last_cost += vec_mult_inner(K,mult,n_con);
			
		}
		
		
		/* the update step looks the following:
		
								a_new = a - eps * (dLda + mult_t * dMda)_t     */
								
								
		vec_mat_mult(mult,n_con,dMda,n_con,n_parm,new_a);
		
		vec_add(dLda,new_a,n_parm,new_a);
		
		vec_mult_scalar(new_a,n_parm,eps,new_a);
		
		vec_sub(a,new_a,n_parm,new_a);
		
		
		/* calculate the updated cost */
		
		last_cost = cost;
		
		cost = (*f_L)(new_a);
		
		(*f_M)(new_a,K);
		
		cost += vec_mult_inner(K,mult,n_con);
		
		/* check out whether we reduced the cost */
				
		if (cost > last_cost) {
				
			/* reduce the gradient climbing rate: sometimes a reduction of eps
				 causes an increase in cost, thus leave an option to increase
				 eps */
			
			if (pending1 && pending2) {
			
				/* this means that either increase nor decrease
					 of eps helps, ==> leave the program */
				
				rc = FALSE;
				break;
				
			} else if (pending1) {
			
				eps *= 2.0;
				pending2 = TRUE;
				
			} else {
			
				eps /= 2.0;
				pending1 = TRUE;
				
			}
			
			goto AGAIN;
			
		}
		
		vec_equal(new_a,n_parm,a);
		
	}

	my_free_vector(mult,1,n_con);
	my_free_vector(dLda,1,n_parm);
	my_free_matrix(dMda,1,n_con,1,n_parm);
	my_free_matrix(dMda_t,1,n_parm,1,n_con);
	my_free_vector(K,1,n_con);
	my_free_matrix(aux_mat,1,n_con,1,n_con);
	my_free_vector(aux_vec,1,n_con);
	my_free_vector(new_a,1,n_parm);
	
	*final_cost = cost;
	*err = sqrt(vec_mult_inner(K,K,n_con));
	
	printf("count=%ld\n",count);
			
	return rc;

}
