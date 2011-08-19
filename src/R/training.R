library(nnet)
source("~/WinlabEEG/src/R/eeg_training_functions.R")

nn_size <- 10
nn_decay <- 5e-4
nn_maxit <- 900
nn_rang <- 0.1

alpha <- 5

test.cl <- function(true, pred) {
	true <- max.col(true)
	cres <- max.col(pred)
	table(true, cres, useNA = "always")
}

assemble_trial <- function (tdata, c1_data, c2_data, curTrial, samp){
	len <- length(tdata)
	for (i in 1:(len / 2)){
		tdata[[i]] <- as.matrix(t(c1_data[(curTrial - 1)*1000 + (i - 1)*500 + samp, 3:16]))
		tdata[[(i + len / 2)]] <- as.matrix(t(c2_data[(curTrial - 1)*1000 + (i - 1)*500 + samp, 3:16]))
	}
	return (tdata)
}

combine_list <- function (newList, list1, list2, list3){
	len1 <- length(list1)
	len2 <- length(list2)
	len3 <- length(list3)
	for (i in 1:len1) newList[[i]] <- list1[[i]]
	for (i in 1:len2) newList[[i + len1]] <- list2[[i]]
	for (i in 1:len3) newList[[i + len1 + len2]] <- list3[[i]]
	return(newList)
}

run_rcspa <- function (c1_data, c2_data){
	siter <- sample(0:9,1)
	samps <- seq(from = 1, to = 499, by = 20) + siter
	samps2 <- seq(from = 1, to = 499, by = 20) + sample((0:9)[-(siter + 1)],1)
	testTrial <- sample(1:3,1)
	trainTrials <- c(1, 2, 3)[-testTrial]

	list4 <- list(A=1, B=1, C=1, D=1)
	trainData <- list(A=1, B=1, C=1, D=1, E=1, F=1, G=1, H=1, I=1, J=1, K=1,L=1)
	t1 <- assemble_trial(list4, c1_data, c2_data, 1, samps)
	t2 <- assemble_trial(list4, c1_data, c2_data, 2, samps)
	t3 <- assemble_trial(list4, c1_data, c2_data, 3, samps)
	trainData <- combine_list(trainData, t1, t2, t3)

	testData <- list(A=1, B=1, C=1, D=1, E=1, F=1, G=1, H=1, I=1, J=1, K=1, L=1)
	t1 <- assemble_trial(list4, c1_data, c2_data, 1, samps2)
	t2 <- assemble_trial(list4, c1_data, c2_data, 2, samps2)
	t3 <- assemble_trial(list4, c1_data, c2_data, 3, samps2)
	testData <- combine_list(testData, t1, t2, t3)

	classLabels <- rep(c(rep("1",2), rep("2",2)), 3)
	
	G <- seq(from = 0, to = 0.9, by = 0.2)
	Ws <- list(A=1, B=1, C=1, E=1, F=1)
	FDA_Vs <- list(A=1, B=1, C=1, E=1, F=1)
	trainFVs <- list(A=1, B=1, C=1, E=1, F=1)
	Scores_C1 <- matrix(rep(0, 5*12), 12, 5)
	Scores_C2 <- Scores_C1
	
	for (i in 1:length(G)){
		gamma <- G[i]
		Ws[[i]] <- csp_components(trainData, classLabels, gamma, alpha)
		curTrainX <- rcspa_fe_mat(trainData, Ws[[i]])	
		FDA_Vs[[i]] <- fda_components(curTrainX, classLabels)
		curTrainZ <- curTrainX %*% FDA_Vs[[i]] #fda_fe_mat(curTrainX, FDA_Vs[[i]])
		curTestX <- rcspa_fe_mat(testData, Ws[[i]])
		curTestZ <- curTestX %*% FDA_Vs[[i]] #fda_fe_mat(curTestX, FDA_Vs[[i]])
		getDE <- get_nnc_rcspa_mat(curTestZ, curTrainZ, classLabels)
		for (j in 1:12){
			Scores_C1[j,i] <- getDE[j,1]
			Scores_C2[j,i] <- getDE[j,2]
		}
		trainFVs[[i]] <- curTrainZ
	}
	OVERALL_SCORE <- rep(0, 12)
	for (i in 1:12){
		if (sum(Scores_C1[i,]) > sum(Scores_C2[i,])) OVERALL_SCORE[i] <- "2"
		if (sum(Scores_C1[i,]) < sum(Scores_C2[i,])) OVERALL_SCORE[i] <- "1"
		if (sum(Scores_C1[i,]) == sum(Scores_C2[i,])) OVERALL_SCORE[i] <- "0"
	}
	result <- test.cl(class.ind(classLabels), class.ind(OVERALL_SCORE))
	
	return (list(SC = sum(diag(result))/12, WS = Ws, FDA = FDA_Vs, Gammas = G, Zs = trainFVs, Labels=classLabels))
}

get_ave_res <- function (d1, d2){
	curObj <- run_rcspa(d1,d2)
	T_AVE <- curObj$SC
	for (j in 2:10){
		curObj <- run_rcspa(d1,d2)
		T_AVE <- T_AVE * (j - 1) / j + curObj$SC / j
	}
	return (T_AVE)
}

get_score <- function (dec1, dec2){
	cor <- 0
	if (dec1 == "1") cor <- cor + 1
	if (dec2 == "2") cor <- cor + 1
	return (cor)
}

custom_test <- function(d1_data, d2_data, csp_objs, numIter, inter){
	samp <- seq(from = 1, to = 499, by = inter)
	scores <- matrix(rep(0, numIter), numIter, length(csp_objs))
	for (i in 1:numIter){
		writeLines("!")
		x <- sample(0:1, 1)
		siter <- sample(0:9, 1)
		d1_data <- t(dataLarm[samp + siter + 500*x, 3:16])
		d2_data <- t(dataRarm[samp + siter + 500*x, 3:16])
		for (j in 1:length(csp_objs)){
			curObj <- csp_objs[[j]]
			dec1 <- get_rcspa_decision(d1_data, curObj$WS, curObj$FDA, curObj$Labels, curObj$Zs)
			dec2 <- get_rcspa_decision(d2_data, curObj$WS, curObj$FDA, curObj$Labels, curObj$Zs)
			scores[i,j] <- get_score(dec1, dec2)
		}
	}
	oscores <- rep(0, length(csp_objs))
	for (j in 1:length(oscores)) oscores[j] <- sum(scores[,j]) / (2*numIter)
	return (oscores)
}

write_out.csv <- function(curObj, floc){
	system(paste("if [ ! -e ", floc, " ]]; then mkdir ", floc, "; fi", sep = ""))
	WS <- curObj$WS
	FDA <- curObj$FDA
	labels <- curObj$Labels
	Zs <- curObj$Zs
	write.csv(WS[[1]], paste(floc, "/W1.csv", sep=""))
	write.csv(WS[[2]], paste(floc, "/W2.csv", sep=""))
	write.csv(WS[[3]], paste(floc, "/W3.csv", sep=""))
	write.csv(WS[[4]], paste(floc, "/W4.csv", sep=""))
	write.csv(WS[[5]], paste(floc, "/W5.csv", sep=""))
	write.csv(FDA[[1]], paste(floc, "/FDA1.csv", sep=""))
	write.csv(FDA[[2]], paste(floc, "/FDA2.csv", sep=""))
	write.csv(FDA[[3]], paste(floc, "/FDA3.csv", sep=""))
	write.csv(FDA[[4]], paste(floc, "/FDA4.csv", sep=""))
	write.csv(FDA[[5]], paste(floc, "/FDA5.csv", sep=""))
	write.csv(labels, paste(floc, "/Labels.csv", sep=""))
	write.csv(Zs[[1]], paste(floc, "/Z1.csv", sep=""))
	write.csv(Zs[[1]], paste(floc, "/Z2.csv", sep=""))
	write.csv(Zs[[1]], paste(floc, "/Z3.csv", sep=""))
	write.csv(Zs[[1]], paste(floc, "/Z4.csv", sep=""))
	write.csv(Zs[[1]], paste(floc, "/Z5.csv", sep=""))
}

# Actions
# larm, lclench, lpivot, pull, push, rarm, rclench, rpivot
data <- read.csv(file="~/WinlabEEG/training_data/trainData.csv",head=TRUE,sep=",")
dataLarm <- read.csv(file="~/WinlabEEG/training_data/trainDataLarm4.csv",head=TRUE,sep=",")
dataRarm <- read.csv(file="~/WinlabEEG/training_data/trainDataRarm4.csv",head=TRUE,sep=",")

#res <- rep(0,4)
#res[1] <- get_ave_res(data[(data$Action == "larm"),], data[(data$Action == "rarm"),])
#res[2] <- get_ave_res(data[(data$Action == "lpivot"),], data[(data$Action == "rpivot"),])
#res[3] <- get_ave_res(data[(data$Action == "lclench"),], data[(data$Action == "rclench"),])
#res[4] <- get_ave_res(data[(data$Action == "pull"),], data[(data$Action == "push"),])

curObj1 <- run_rcspa(data[(data$Action == "larm"),], data[(data$Action == "rarm"),])
curObj2 <- run_rcspa(data[(data$Action == "larm"),], data[(data$Action == "rarm"),])
curObj3 <- run_rcspa(data[(data$Action == "larm"),], data[(data$Action == "rarm"),])
samp <- seq(from = 1, to = 499, by = 10)
scores <- matrix(rep(0, 150), 150, 3)


for (i in 1:150){
	x <- sample(0:1, 1)
	siter <- sample(0:9, 1)
	trial1 <- t(dataLarm[samp + siter + 500*x, 3:16])
	trial2 <- t(dataRarm[samp + siter + 500*x, 3:16])
	
	dec11 <- get_rcspa_decision(trial1, curObj1$WS, curObj1$FDA, curObj1$Labels, curObj1$Zs)
	dec12 <- get_rcspa_decision(trial2, curObj1$WS, curObj1$FDA, curObj1$Labels, curObj1$Zs)

	dec21 <- get_rcspa_decision(trial1, curObj2$WS, curObj2$FDA, curObj2$Labels, curObj2$Zs)
	dec22 <- get_rcspa_decision(trial2, curObj2$WS, curObj2$FDA, curObj2$Labels, curObj2$Zs)

	dec31 <- get_rcspa_decision(trial1, curObj3$WS, curObj3$FDA, curObj3$Labels, curObj3$Zs)
	dec32 <- get_rcspa_decision(trial2, curObj3$WS, curObj3$FDA, curObj3$Labels, curObj3$Zs)
	
	scores[i,1] <- get_score(dec11, dec12)
	scores[i,2] <- get_score(dec21, dec22)
	scores[i,3] <- get_score(dec31, dec32)
}

oscore <- c(sum(scores[,1]), sum(scores[,2]), sum(scores[,3])) / (2*150)
writeLines(sprintf("curObj1 :: %s\ncurObj2 :: %s\ncurObj3 :: %s", oscore[1], oscore[2], oscore[3]))

#training_data <- list(A = as.matrix(t(larm_data[samp,3:16])), B = as.matrix(t(larm_data[samp + 500, 3:16])), C = as.matrix(t(larm_data[samp + 1000, 3:16])), D = as.matrix(t(larm_data[samp + 1500, 3:16])), E = as.matrix(t(larm_data[samp + 2000, 3:16])), F = as.matrix(t(larm_data[samp + 2500, 3:16])), I = as.matrix(t(rarm_data[samp,3:16])), J = as.matrix(t(rarm_data[samp + 500, 3:16])), K = as.matrix(t(rarm_data[samp + 1000, 3:16])), L = as.matrix(t(rarm_data[samp + 1500, 3:16])), M = as.matrix(t(rarm_data[samp + 2000, 3:16])), N = as.matrix(t(rarm_data[samp + 2500, 3:16])), O = as.matrix(t(larm_data[samp + 1, 3:16])), P = as.matrix(t(rarm_data[samp + 1, 3:16])))
