library(fastICA)

nn_size <- 3
nn_decay <- 5e-4
nn_maxit <- 600
nn_rang <- 0.1

min_certainty <- 0.65

fs <- 250

test.cl <- function(true, pred) {
	true <- max.col(true)
	cres <- max.col(pred)
	table(true, cres, useNA = "always")
}

# Supplementary functions
pst <- function (df) {
	len <- length(df)
	ps <- rep(0, len / 2)
	dft <- abs(df)
	ps[1] <- (dft[1] / len)^2
	ps[len/2] <- (dft[len / 2] / len)^2
	for (i in 2:(len/2)){
		ps[i] <- (dft[i]^2 + dft[len - i]^2)/(len^2)
	}
	return(ps)
}

matrix.trace <- function (mat) {
	len <- dim(mat)[1]
	sum <- 0
	for (i in 1:len) sum <- sum + mat[i,i]
	return (sum)
}

make_decision <- function (prob_vector){
	len <- length(prob_vector)
	max_index <- 0
	max_value <- 0
	for (i in 1:len){
		if (prob_vector [i] > max_value){
			max_index <- i
			max_value <- prob_vector[i]
		}
	}
	if (max_value < min_certainty) max_index <- 0
	return (max_index)
}

# Pre-processing functions

ica_components <- function (trainingData, number_components){
	scaling <- scale(trainingData, scale = FALSE)
	trainingData <- trainingData - scaling
	ica <- fastICA(trainingData, number_components, alg.typ = "parallel", fun = "logcosh", alpha = 1, method = "R", row.norm = FALSE, maxit = 200, tol = 0.0001)
	return (list(centering = scaling, whitening = ica$K, unmixing = ica$W, mixing = ica$A))
}

pca_components <- function (trainingData, number_components){
	icaComp <- ica_components (trainingData, number_components)
	return (list(centering = icaComp$centering, whitening = icaComp$whitening))
}

csp_components <- function (trainingTrials, trainingTrialClasses, trainingTrialSubjects, numTrialClassOne, numTrialClassTwo, gamma, beta){
	trainingTrialLen <- length(trainingTrials)
	trainingTrialVars <- trainingTrialLen	
	for (i in 1:trainingTrialLen) {
		curMat <- trainingTrials[[i]]
		curMat <- curMat %*% t(curMat)
		trainingTrialVars[[i]] <- curMat/matrix.trace(curMat)
	}
	
}

# Feature extraction functions

pca_fe <- function (testingData, pcaComp){
	testingData <- testingData - pcaComp$centering
	testingData <- testingData %*% pcaComp$whitening
	return (testingData)
}

ica_fe <- function (testingData, icaComp){
	testingData <- pca_fe (testingData, icaComp)
	testingData <- testingData %*% icaComp$unmixing
	return (testingData)
}

sfa_fe <- function (data) {
	data_dft <- abs(fft(data))
	fi <- 0.5 / (fs / length(data_dft))
	delta <- max(data_dft[fi:(8*fi)])
	theta <- max(data_dft[(8*fi + 1):(16*fi)])
	alpha <- max(data_dft[(16*fi + 1):(28*fi)]
	beta <- max(data_dft[(28*fi + 1):(60*fi)]
	gamma <- max(data_dft[(60*fi):(length(data_dft))])
	return (c(delta, theta, alpha, beta, gamma) / (delta + theta + alpha + beta + gamma)
}

psa_fe <- function (data) {
	data_dft <- abs(fft(data))
	fi <- 0.5 / (fs / length(data_dft))
	data_bpf <- c(rep(0, 16*fi), data_dft[(16*fi + 1):(80*fi)], rep(0, length(data_dft) - 80*fi))	
	data_ps <- pst(data_bpf)
	alpha <- max(data_ps[(16*fi + 1):(28*fi)])
	beta <- max(data_ps[(28*fi + 1):(60*fi)])
	gamma <- max(data_ps[(60*fi + 1):(80*fi)])
	return (c(alpha, beta, gamma) / (alpha + beta + gamma))
}

stacking_fe <- function (classifier_probs) {
	len <- length(classifier_probs)
	fv <- rep(0, len)
	for (i in 1:len){
		fv[i] <- make_decision(classifier_probs[[i]])
	}
	return (fv)
}

rcspa_fe <- function (testingTrial, cspComp){

}
