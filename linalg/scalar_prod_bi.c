/***********************************************************************
 * Copyright (C) 2006 Thomas Chiarappa
 *
 * This file is part of tmLQCD.
 *
 * tmLQCD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * tmLQCD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with tmLQCD.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/
/* $Id: scalar_prod_bi.c 1150 2009-02-16 16:52:09Z urbach $ */

/****************************************************************************
 *
 *     Scalar product routine adapted for the bispinor case
 *
 * Author: Thomas Chiarappa
 *         Thomas.Chiarappa@mib.infn.it
 *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include<config.h>
#endif
#include <stdlib.h>
#ifdef MPI
#include <mpi.h>
#endif
#include "su3.h"
#include "scalar_prod_bi.h"


/*  <S,R>=S^* times R */
_Complex double scalar_prod_bi(bispinor * const S, bispinor * const R, const int N){

  int ix;
  static double ks,kc,ds,tr,ts,tt;
  spinor *s,*r, *t, *u;
  _Complex double c = 0.0;
  
  /* Real Part */

  ks=0.0;
  kc=0.0;
  
  for (ix = 0; ix < N; ix++){

    s=(spinor *) &S[ix].sp_up;
    r=(spinor *) &R[ix].sp_up;
    t=(spinor *) &S[ix].sp_dn;
    u=(spinor *) &R[ix].sp_dn;

    
    ds= creal(r->s0.c0) * creal(s->s0.c0) + cimag(r->s0.c0) * cimag(s->s0.c0) +
        creal(r->s0.c1) * creal(s->s0.c1) + cimag(r->s0.c1) * cimag(s->s0.c1) +
        creal(r->s0.c2) * creal(s->s0.c2) + cimag(r->s0.c2) * cimag(s->s0.c2) +
        creal(r->s1.c0) * creal(s->s1.c0) + cimag(r->s1.c0) * cimag(s->s1.c0) +
        creal(r->s1.c1) * creal(s->s1.c1) + cimag(r->s1.c1) * cimag(s->s1.c1) +
        creal(r->s1.c2) * creal(s->s1.c2) + cimag(r->s1.c2) * cimag(s->s1.c2) +
        creal(r->s2.c0) * creal(s->s2.c0) + cimag(r->s2.c0) * cimag(s->s2.c0) +
        creal(r->s2.c1) * creal(s->s2.c1) + cimag(r->s2.c1) * cimag(s->s2.c1) +
        creal(r->s2.c2) * creal(s->s2.c2) + cimag(r->s2.c2) * cimag(s->s2.c2) +
        creal(r->s3.c0) * creal(s->s3.c0) + cimag(r->s3.c0) * cimag(s->s3.c0) +
        creal(r->s3.c1) * creal(s->s3.c1) + cimag(r->s3.c1) * cimag(s->s3.c1) +
        creal(r->s3.c2) * creal(s->s3.c2) + cimag(r->s3.c2) * cimag(s->s3.c2) +
        creal(u->s0.c0) * creal(t->s0.c0) + cimag(u->s0.c0) * cimag(t->s0.c0) +
        creal(u->s0.c1) * creal(t->s0.c1) + cimag(u->s0.c1) * cimag(t->s0.c1) +
        creal(u->s0.c2) * creal(t->s0.c2) + cimag(u->s0.c2) * cimag(t->s0.c2) +
        creal(u->s1.c0) * creal(t->s1.c0) + cimag(u->s1.c0) * cimag(t->s1.c0) +
        creal(u->s1.c1) * creal(t->s1.c1) + cimag(u->s1.c1) * cimag(t->s1.c1) +
        creal(u->s1.c2) * creal(t->s1.c2) + cimag(u->s1.c2) * cimag(t->s1.c2) +
        creal(u->s2.c0) * creal(t->s2.c0) + cimag(u->s2.c0) * cimag(t->s2.c0) +
        creal(u->s2.c1) * creal(t->s2.c1) + cimag(u->s2.c1) * cimag(t->s2.c1) +
        creal(u->s2.c2) * creal(t->s2.c2) + cimag(u->s2.c2) * cimag(t->s2.c2) +
        creal(u->s3.c0) * creal(t->s3.c0) + cimag(u->s3.c0) * cimag(t->s3.c0) +
        creal(u->s3.c1) * creal(t->s3.c1) + cimag(u->s3.c1) * cimag(t->s3.c1) +
        creal(u->s3.c2) * creal(t->s3.c2) + cimag(u->s3.c2) * cimag(t->s3.c2) ;

    /* Kahan Summation */    
    tr=ds+kc;
    ts=tr+ks;
    tt=ts-ks;
    ks=ts;
    kc=tr-tt;
  }
  kc=ks+kc;

#if defined MPI
  MPI_Allreduce(&kc, &ks, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  kc = ks;
#endif

  c += kc;

  /* Imaginary Part */

  ks=0.0;
  kc=0.0;
  
  for (ix=0;ix<N;ix++){

    s=(spinor *) &S[ix].sp_up;
    r=(spinor *) &R[ix].sp_up;
    t=(spinor *) &S[ix].sp_dn;
    u=(spinor *) &R[ix].sp_dn;
    
    ds=- creal(r->s0.c0) * cimag(s->s0.c0) + cimag(r->s0.c0) * creal(s->s0.c0) -
        creal(r->s0.c1) * cimag(s->s0.c1) + cimag(r->s0.c1) * creal(s->s0.c1) -
        creal(r->s0.c2) * cimag(s->s0.c2) + cimag(r->s0.c2) * creal(s->s0.c2) -
        creal(r->s1.c0) * cimag(s->s1.c0) + cimag(r->s1.c0) * creal(s->s1.c0) -
        creal(r->s1.c1) * cimag(s->s1.c1) + cimag(r->s1.c1) * creal(s->s1.c1) -
        creal(r->s1.c2) * cimag(s->s1.c2) + cimag(r->s1.c2) * creal(s->s1.c2) -
        creal(r->s2.c0) * cimag(s->s2.c0) + cimag(r->s2.c0) * creal(s->s2.c0) -
        creal(r->s2.c1) * cimag(s->s2.c1) + cimag(r->s2.c1) * creal(s->s2.c1) -
        creal(r->s2.c2) * cimag(s->s2.c2) + cimag(r->s2.c2) * creal(s->s2.c2) -
        creal(r->s3.c0) * cimag(s->s3.c0) + cimag(r->s3.c0) * creal(s->s3.c0) -
        creal(r->s3.c1) * cimag(s->s3.c1) + cimag(r->s3.c1) * creal(s->s3.c1) -
        creal(r->s3.c2) * cimag(s->s3.c2) + cimag(r->s3.c2) * creal(s->s3.c2) -
        creal(u->s0.c0) * cimag(t->s0.c0) + cimag(u->s0.c0) * creal(t->s0.c0) -
        creal(u->s0.c1) * cimag(t->s0.c1) + cimag(u->s0.c1) * creal(t->s0.c1) -
        creal(u->s0.c2) * cimag(t->s0.c2) + cimag(u->s0.c2) * creal(t->s0.c2) -
        creal(u->s1.c0) * cimag(t->s1.c0) + cimag(u->s1.c0) * creal(t->s1.c0) -
        creal(u->s1.c1) * cimag(t->s1.c1) + cimag(u->s1.c1) * creal(t->s1.c1) -
        creal(u->s1.c2) * cimag(t->s1.c2) + cimag(u->s1.c2) * creal(t->s1.c2) -
        creal(u->s2.c0) * cimag(t->s2.c0) + cimag(u->s2.c0) * creal(t->s2.c0) -
        creal(u->s2.c1) * cimag(t->s2.c1) + cimag(u->s2.c1) * creal(t->s2.c1) -
        creal(u->s2.c2) * cimag(t->s2.c2) + cimag(u->s2.c2) * creal(t->s2.c2) -
        creal(u->s3.c0) * cimag(t->s3.c0) + cimag(u->s3.c0) * creal(t->s3.c0) -
        creal(u->s3.c1) * cimag(t->s3.c1) + cimag(u->s3.c1) * creal(t->s3.c1) -
        creal(u->s3.c2) * cimag(t->s3.c2) + cimag(u->s3.c2) * creal(t->s3.c2) ;
    
    tr=ds+kc;
    ts=tr+ks;
    tt=ts-ks;
    ks=ts;
    kc=tr-tt;
  }
  kc=ks+kc;


#if defined MPI
  MPI_Allreduce(&kc, &ks, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  kc = ks;
#endif

  c += kc * I;
  return(c);

}
