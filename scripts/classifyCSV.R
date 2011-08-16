#!/usr/lib/R/bin/Rscript --vanilla

args <- commandArgs()
args <- as.numeric(args[-(1:(match("--args", args) + 1))])

data <- read.csv(file=args[1],head=TRUE,sep=",")
results <- t(rep(0, dim(data)[1]))

for (i in (1:dim(data)[1])){
	Rscript classifyRAW.R args[1] args[2] args[3] args[4] args[5] args[6] args[7] args[8]
}
