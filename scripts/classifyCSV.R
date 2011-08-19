#!/usr/lib/R/bin/Rscript --vanilla

args <- commandArgs()
#args <- args[-(1:(match("--args", args) + 1))]

source("~/WinlabEEG/src/R/eeg_training_functions.R")

CDIR <- "~/WinlabEEG/training_data/OBJ"
numObjs <- 7

writeLines(sprintf("CSV FILE: %s", paste(args[6],sep="")))
trial <- read.csv(file=paste(args[6],sep=""),head=TRUE,sep=",")#[,3:16]
trial <- t(trial)
trial <- trial [-c(dim(trial)[1], 1, 2),]#,seq(from = 1, to = 499, by = 20) + 500*sample(0:1,1) + sample(0:9,1)]
trial <- matrix(as.numeric(trial), dim(trial)[1], dim(trial)[2])#as.matrix(t(trial)

decisions <- rep(0,numObjs)

for (k in 1:numObjs){
	cdir <- paste(CDIR, k, "/", sep="")
	#writeLines(sprintf("\nINF>: OBJ #%s",k))
	Ws <- list(A=1,B=1,C=1,D=1,E=1)
	FDA <- list(A=1,B=1,C=1,D=1,E=1)
	Zs <- list(A=1,B=1,C=1,D=1,E=1)
	totalList <- list(W=Ws,F=FDA,Z=Zs,D=1)
	nums <- 1:5
	pre <- c("W", "FDA", "Z")

	for (i in 1:(length(totalList)-1)){
		curObj <- totalList[[i]]
		for (j in 1:length(curObj)){
			#writeLines(sprintf("CMD>: read.csv(file=\"%s\", sep=\",\",head=TRUE)", paste(cdir,pre[i], nums[j], ".csv",sep="")))
			curObj[[j]] <- as.matrix(read.csv(file=paste(cdir,pre[i], nums[j], ".csv",sep=""),head=TRUE,sep=","))[,-1]
		}
		totalList[[i]] <- curObj
	}
	#writeLines(sprintf("CMD>: read.csv(file=\"%s\", sep=\",\",head=TRUE)", paste(cdir,"Labels.csv",sep="")))
	totalList$D <- read.csv(file=paste(cdir,"Labels.csv",sep=""), head=TRUE,sep=",")[,2]
	decisions[k] <- get_rcspa_decision(trial, totalList$W, totalList$F, totalList$D, totalList$Z)
	writeLines(sprintf("DECISION for %s is %s", k, decisions[k]))
}
classOneInd <- seq_along(decisions)[decisions == "1"]
classOneVotes <- length(classOneInd)
classTwoVotes <- length(decisions[-classOneInd])

totalDec <- "1"
if (classTwoVotes > classOneVotes) totalDec <- "2"



writeLines(sprintf("FINAL_DECISION=%s", totalDec))
