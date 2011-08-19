#!/usr/lib/R/bin/Rscript --vanilla

args <- commandArgs()
args <- as.numeric(args[-(1:(match("--args", args) + 1))])

#dyn.load("~/WinlabEEG/src/C/nnet.so")
source("~/WinlabEEG/src/R/nnet_extra.R")

net <- NULL
net$wts <- c(-0.1068133, -0.6500254, -2.0145936, 3.0505881, 1.4853258, 13.1365998, 10.5330092, 8.4960455, -16.9018747, -11.6667560, -16.4380323, 10.0198104, 12.6653360, 1.9200962, -8.5859009, 2.2922390, 4.3301011, 4.2420184, -17.057933)
net$n <- c(4, 2, 3)
net$nconn <- c(0,0,0,0,0,0,5,10,13,16,19)
net$conn <- c(0,1,2,3,4,0,1,2,3,4,0,5,6,0,5,6,0,5,6)
net$nsunits <- c(10)
net$softmax <- FALSE
net$censored <- FALSE
net$classes <- c("c","s","v")
net$lev <- NULL



predict.nnet_init(net, args)
