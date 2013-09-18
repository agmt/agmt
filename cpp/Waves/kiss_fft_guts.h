/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* kiss_fft.h
   defines kiss_fft_scalar as either short or a float type
   and defines
   typedef struct { kiss_fft_scalar r; kiss_fft_scalar i; }kiss_fft_cpx; */
#ifndef KISS_FFT_GUTS_H
#define KISS_FFT_GUTS_H

#include "kiss_fft.h"
#include <limits.h>

#define MAXFACTORS 32
/* e.g. an fft of length 128 has 4 factors 
 as far as kissfft is concerned
 4*4*4*2
 */

struct kiss_fft_state{
    int nfft;
    int inverse;
    int factors[2*MAXFACTORS];
    kiss_fft_cpx twiddles[1];
};

/*
  Explanation of macros dealing with complex math:

   C_MUL(m,a,b)         : m = a*b
   C_FIXDIV( c , div )  : if a fixed point impl., c /= div. noop otherwise
   C_SUB( res, a,b)     : res = a - b
   C_SUBFROM( res , a)  : res -= a
   C_ADDTO( res , a)    : res += a
 * */



#define CHECK_OVERFLOW

#if defined(CHECK_OVERFLOW)
#  define CHECK_OVERFLOW_OP(a,op,b)  \
	if ( (SAMPPROD)(a) op (SAMPPROD)(b) > SAMP_MAX || (SAMPPROD)(a) op (SAMPPROD)(b) < SAMP_MIN ) { \
		fprintf(stderr,"WARNING:overflow @ " __FILE__ "(%d): (%d " #op" %d) = %ld\n",__LINE__,(a),(b),(SAMPPROD)(a) op (SAMPPROD)(b) );  }
#endif


#   define smul(a,b) ( ((SAMPPROD)(a))*((SAMPPROD)(b)) )
#   define sround( x )  (kiss_fft_scalar)( ( ((x>=0)?(x + (1<<(FRACBITS-1))):(x - (1<<(FRACBITS-1))) )) >> FRACBITS )

#   define S_MUL(a,b) sround( smul(a,b) )

#	define S_DIV(a,b) (kiss_fft_scalar)( ((((SAMPPROD)(a))<<VALBITS) + (((SAMPPROD)b))) / ((SAMPPROD)(b)) )

#   define C_MUL(m,a,b) \
      do{ (m).r = sround( smul((a).real(),(b).real()) - smul((a).i,(b).imag()) ); \
          (m).i = sround( smul((a).real(),(b).imag()) + smul((a).i,(b).real()) ); }while(0)


#   define C_MULBYSCALAR( c, s ) \
    do{ (c).r =  sround( smul( (c).real() , s ) ) ;\
        (c).i =  sround( smul( (c).imag() , s ) ) ; }while(0)

#  define HALF_OF(x) ((x)>>1)

#  define kf_cos(phase)  floor(.5+SAMP_MAX * cos (phase))
#  define kf_sin(phase)  floor(.5+SAMP_MAX * sin (phase))


#define  kf_cexp(x,phase) \
	do{ \
		(x)->real(kf_cos(phase));\
		(x)->imag(kf_sin(phase));\
	}while(0)


/* a debugging function */
#define pcpx(c)\
    fprintf(stderr,"%g + %gi\n",(double)((c)->r),(double)((c)->i) )


#define  KISS_FFT_TMP_ALLOC(nbytes) malloc(nbytes)
#define  KISS_FFT_TMP_FREE(ptr) free(ptr)
#define  KISS_FFT_MALLOC(nbytes) malloc(nbytes)
#define  KISS_FFT_FREE(ptr) free(ptr)

#endif
