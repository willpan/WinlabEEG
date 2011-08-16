# file nnet/nnet.R
# copyright (C) 1994-2003 W. N. Venables and B. D. Ripley
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 or 3 of the License
#  (at your option).
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  A copy of the GNU General Public License is available at
#  http://www.r-project.org/Licenses/
#

# Note from the WinlabEEG authors:
#	The below function is modeled directly after the predict.nnet() function in the nnet package under the file 
#	name of nnet.R. The original disclaimer of this package is reprinted above. We, the authors of WinlabEEG, merely 
#	modified the code of predict.nnet() slightly so as to fit our purposes for our scripts and other R code we use. All C 
#	functions are from the same nnet package source file named nnet.c, which the authors of WinlabEEG has not modified 
#	in any way and only included in this repository for this code.
#

predict.nnet_init <- function(object, newdata, type=c("raw","class"), ...)
{
	type <- match.arg(type)
	dyn.load("~/WinlabEEG/src/C/nnet.so")
    	if(is.null(dim(newdata))){
        	dim(newdata) <- c(1L, length(newdata)) # a row vector
        	x <- as.matrix(newdata)     # to cope with dataframes
        	if(any(is.na(x))) stop("missing values in 'x'")
        	keep <- 1L:nrow(x)
        	rn <- rownames(x)
    	}
    	ntr <- nrow(x)
    	nout <- object$n[3L]
        .C("VR_set_net",
           as.integer(object$n), as.integer(object$nconn),
           as.integer(object$conn), rep(0.0, length(object$wts)),
           as.integer(object$nsunits), as.integer(0L),
           as.integer(object$softmax), as.integer(object$censored))
        z <- matrix(NA, nrow(newdata), nout,
                    dimnames = list(rn, object$classes))

        z[keep, ] <- matrix(.C("VR_nntest",
                               as.integer(ntr),
                               as.double(x),
                               tclass = double(ntr*nout),
                               as.double(object$wts)
                               )$tclass, ntr, nout)
        .C("VR_unset_net")
	switch(type, raw = z,
           class = {
               if(is.null(object$lev)) stop("inappropriate fit for class")
               if(ncol(z) > 1L) object$lev[max.col(z)]
               else object$lev[1L + (z > 0.5)]
           })
}
