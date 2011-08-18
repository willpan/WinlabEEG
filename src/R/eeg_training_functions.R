library(fastICA)

min_certainty <- 0.65

fs <- 200

# Supplementary functions
make_eigenvalues_positive <- function (eigenvalue_object){
	values_to_change <- seq_along(eigenvalue_object$values)[eigenvalue_object$values < 0]
	for (i in values_to_change){
		eigenvalue_object$vectors[,i] <- -1*eigenvalue_object$vectors[,i]
		eigenvalue_object$values[i] <- -1*eigenvalue_object$values[i]
	}
	return (list(values = eigenvalue_object$values, vectors = eigenvalue_object$vectors))
}

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

mean_rows <- function (M){
	numSamps <- dim(M)[1]
	m <- dim(M)[2]
	for (i in 1:numSamps) m <- m + M[i,]
	return (m / numSamps)
}

get_var_mat <- function (S, mean){
	numVals <- length(mean)
	S2 <- matrix(rep(0, numVals*numVals), numVals, numVals)
	for (i in 1:dim(S)[1]) S2 <- S2 + (S[i,] - mean) %*% t(S[i,] - mean)
	return (S2)
}

edist <- function (v1, v2){
	dist <- 0
	for (i in 1:length(v1)) dist <- dist + (v1[i] - v2[i])^2
	return (dist^(1/2))
}

get_nnc_class <- function (testVector, featureVectors){
	dist <- edist(testVector, featureVectors[1])
	max_dist <- dist
	min_dist <- dist
	for (i in 1:length(featureVectors)){
		dist <- edist(testVector, featureVectors[i])
		if (dist > max_dist) max_dist <- dist
		if (dist < min_dist) min_dist <- dist
	}
	return(list(DMAX = max_dist, DMIN = min_dist))
}

get_nnc_rcspa <- function (testVector, featureVectors, featureVectorLabels){
	numFV <- dim(featureVectors)[1]
	ind_classone <- seq_along(featureVectorLabels)[featureVectorLabels == "1"]
	C1_metrics <- get_nnc_class(testVector, featureVectors[ind_classone,])
	C2_metrics <- get_nnc_class(testVector, featureVectors[-ind_classone,])
	DMAX <- max(C1_metrics$DMAX, C2_metrics$DMAX)
	DMIN <- min(C1_metrics$DMIN, C2_metrics$DMIN)
	DE_C1 <- (C1_metrics$DMIN - DMIN) / (DMAX - DMIN)
	DE_C2 <- (C2_metrics$DMIN - DMIN) / (DMAX - DMIN)
	return (list(D_C1 = DE_C1, D_C2 = DE_C2))
}

get_nnc_rcspa_mat <- function (testMat, featureVectors, featureVectorLabels){
	numTests <- dim(testMat)[1]
	scores <- matrix(rep(0, 2*numTests),numTests,2)
	for (i in 1:numTests) {
		x <- get_nnc_rcspa(testMat[i,], featureVectors, featureVectorLabels)
		scores[i,1] <- x$D_C1
		scores[i,2] <- x$D_C2
	}
	return (scores)
}

# Pre-processing functions
fda_components <- function (data_mat, dataTrials){
	ind_classone <- seq_along(dataTrials)[dataTrials == "1"]
	ind_classtwo <- (1:length(dataTrials))[-ind_classone]
	
	S1_samples <- data_mat[ind_classone,]
	S2_samples <- data_mat[ind_classtwo,]

	S1_mean <- mean_rows(S1_samples)
	S2_mean <- mean_rows(S2_samples)

	S1_var <- get_var_mat(S1_samples, S1_mean)
	S2_var <- get_var_mat(S2_samples, S2_mean)

	SW <- S1_var + S2_var
	SB <- (S1_mean - S2_mean) %*% t(S1_mean - S2_mean)

	#return(list(S1VAR=S1_var, S2VAR=S2_var, SW=SW, SB=SB))

	v <- solve(SW) %*% (S1_mean - S2_mean)
	return(v)
}

ica_components <- function (trainingData, number_components){
	scaling <- scale(trainingData, scale = FALSE)
	#trainingData <- trainingData - scaling
	ica <- fastICA(trainingData, number_components, alg.typ = "parallel", fun = "logcosh", alpha = 1, method = "C", row.norm = FALSE, maxit = 200, tol = 0.0001)
	return (list(centering = scaling, whitening = ica$K, unmixing = ica$W, mixing = ica$A))
}

pca_components <- function (trainingData, number_components){
	icaComp <- ica_components (trainingData, number_components)
	return (list(centering = icaComp$centering, whitening = icaComp$whitening))
}

# single-subject
csp_components <- function (trainingTrials, trainingTrialClasses, gamma, alpha){
	numchannels <- dim(trainingTrials[[1]])[1]
	trainingTrialLen <- length(trainingTrials)
	trainingTrialVars <- trainingTrials
	for (i in 1:trainingTrialLen) {
		curMat <- trainingTrials[[i]]
		curMat <- curMat %*% t(curMat)
		trainingTrialVars[[i]] <- curMat/matrix.trace(curMat)
	}
	ind_classone <- seq_along(trainingTrialClasses)[trainingTrialClasses == 1]
	ind_classtwo <- (1:trainingTrialLen)[-ind_classone]
	sum_classone <- trainingTrialVars[[ind_classone[1]]]
	for (i in ind_classone[-1]) sum_classone <- sum_classone + trainingTrialVars[[i]]
	sum_classtwo <- trainingTrialVars[[ind_classtwo[1]]]
	for (i in ind_classtwo[-1]) sum_classtwo <- sum_classtwo + trainingTrialVars[[i]]
	sum_classone <- sum_classone / length(ind_classone)
	sum_classtwo <- sum_classtwo / length(ind_classtwo)
	sum_classone <- (1 - gamma)*sum_classone + (gamma / numchannels)*diag(numchannels)
	sum_classtwo <- (1 - gamma)*sum_classtwo + (gamma / numchannels)*diag(numchannels)
	sigma <- sum_classone + sum_classtwo
	eig <- make_eigenvalues_positive(eigen(sigma))
	Lamda <- diag(eig$values^(-1/2))
	U <- eig$vectors
	B <- make_eigenvalues_positive(eigen(Lamda %*% t(U) %*% sum_classone %*% U %*% Lamda))$vectors
	#return (list(B = B, Lamda = Lamda, U = U, Sigma = sigma, Sigma_1 = sum_classone))
	W_0 <- t(B) %*% Lamda %*% t(U)
	W <- W_0 [, c(1:alpha, (numchannels - (alpha - 1)):numchannels)]
	return (W)
}

# Feature extraction functions

pca_fe <- function (testingData, pcaComp){
	testingData <- as.numeric(as.matrix(testingData))
	testingData <- testingData - pcaComp$centering [1,]
	testingData <- testingData %*% pcaComp$whitening
	#testingData <- pcaComp$whitening %*% testingData
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
	alpha <- max(data_dft[(16*fi + 1):(28*fi)])
	beta <- max(data_dft[(28*fi + 1):(60*fi)])
	gamma <- max(data_dft[(60*fi):(length(data_dft))])
	return (c(delta, theta, alpha, beta, gamma) / (delta + theta + alpha + beta + gamma))
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

#return (list(Sigma_1 = sum_classone, Sigma_2 = sum_classtwo, Lambda = diag(eig$values), U = eig$vectors))

# MUST HAVE LAMDA BE ALL POSITIVE!
rcspa_fe <- function (testingTrial, W){
	X <- t(W) %*% testingTrial
	Q <- dim(W)[2]
	XVar <- 1:Q
	for (i in 1:Q) XVar[i] <- var(X[i,])
	XVarSum <- sum(XVar)
	Y <- 1:Q
	for (i in 1:length(Y)) Y[i] <- log(XVar[i] / XVarSum)
	return (Y)
}

rcspa_fe_mat <- function (testingTrials, W){
	numFeatures <- length(testingTrials)
	Q <- dim(W)[2]
	features <- matrix(rep(0, numFeatures*Q), numFeatures,Q)
	for (i in 1:length(testingTrials)) features[i,] <- rcspa_fe(testingTrials[[i]], W)
	return (features)
}

fda_fe <- function (testingTrial, V){
	return (t(V) %*% t(testingTrial))
}

fda_fe_mat <- function(testingTrials, V){
	numTrials <- dim(testingTrials)[1]
	numFeats <- 1#dim(testingTrials)[2]
	features <- matrix(rep(0, numTrials*numFeats), numTrials, numFeats)
	for (i in 1:numTrials) features[i,] <- fda_fe(testingTrials, V)
	return(features)
}
