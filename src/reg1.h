//=====================================================================================
//
//           Filename:  src/reg1.h
//
//        Description:  ProbABEL
//
//            Version:  0.1-3
//            Created:  ---
//           Revision:  none
//  last modification:  11-Jan-2009
//
//             Author:  Yurii S. Aulchenko
//			  modified by: 	Maksim V. Struchalin, 11-Jan-2009
//
// Modified by Han Chen (hanchen@bu.edu) on Nov 9, 2009
// based on src/reg1.h version 0.2 as of Oct 19, 2009
//
//            Company:  ErasmusMC, Epidemiology & Biostatistics Department, The Netherlands.
//              Email:  i.aoultchenko@erasmusmc.nl, m.struchalin@erasmusmc.nl
//
//=====================================================================================

#include <cmath>

extern "C" {
#include "survproto.h"
}

mematrix<double> apply_model(mematrix<double> &X, int model, int interaction, int ngpreds, bool iscox=false, int nullmodel = 0)
// model 0 = 2 df
// model 1 = additive 1 df
// model 2 = dominant 1 df
// model 3 = recessive 1 df
// model 4 = over-dominant 1 df
{
	if (model==0)
	{
		if(interaction !=0 && !nullmodel)
		{
			if(ngpreds == 2)
			{
				mematrix<double> nX;
				nX.reinit(X.nrow,X.ncol+2);
				int csnp_p1=nX.ncol-4;
				int csnp_p2=nX.ncol-3;
				int c1 = nX.ncol-2;
				int c2 = nX.ncol-1;
				for (int i=0;i<X.nrow;i++)
					for (int j=0;j<(X.ncol);j++)
						nX[i*nX.ncol+j] = X[i*X.ncol+j];
				for (int i=0;i<nX.nrow;i++)
				{
					if (iscox)
					{
						nX[i*nX.ncol+c1] = X[i*X.ncol+csnp_p1] * X[i*X.ncol + interaction-1];//Maksim: interaction with SNP;;
						nX[i*nX.ncol+c2] = X[i*X.ncol+csnp_p2] * X[i*X.ncol + interaction-1];//Maksim: interaction with SNP;;
					}
					else
					{
						nX[i*nX.ncol+c1] = X[i*X.ncol+csnp_p1] * X[i*X.ncol + interaction];//Maksim: interaction with SNP;;
						nX[i*nX.ncol+c2] = X[i*X.ncol+csnp_p2] * X[i*X.ncol + interaction];//Maksim: interaction with SNP;;
					}
				}
				//________________________
				int col_new;
				if(is_interaction_excluded)
				{
					mematrix<double> nX_without_interact_phe;
					nX_without_interact_phe.reinit(nX.nrow,nX.ncol-1);
					for(int row=0 ; row < nX.nrow ; row++)
					{
						//Han Chen
						col_new=-1;
						for(int col=0 ; col<nX.ncol ; col++)
						{
							if(col != interaction && !iscox)
							{
								col_new++;
								nX_without_interact_phe[row*nX_without_interact_phe.ncol+col_new] = nX[row*nX.ncol+col];
							}
							if(col != interaction-1 && iscox)
							{
								col_new++;
								nX_without_interact_phe[row*nX_without_interact_phe.ncol+col_new] = nX[row*nX.ncol+col];
							}
						}//interaction_only, model==0, ngpreds==2
						//Oct 26, 2009
					}
					return nX_without_interact_phe;
				}//end of is_interaction_excluded
				//________________________



				return(nX);
			}
			if(ngpreds == 1)
			{
				mematrix<double> nX;
				nX.reinit(X.nrow,X.ncol+1);
				int csnp_p1=nX.ncol-2;
				int c1 = nX.ncol-1;
				for (int i=0;i<X.nrow;i++)
					for (int j=0;j<(X.ncol);j++)
						nX[i*nX.ncol+j] = X[i*X.ncol+j];
				for (int i=0;i<nX.nrow;i++)
				{
					if (iscox)
					{
						nX[i*nX.ncol+c1] = X[i*X.ncol+csnp_p1] * X[i*X.ncol + interaction-1];//Maksim: interaction with SNP;;
					}
					else
					{
						nX[i*nX.ncol+c1] = X[i*X.ncol+csnp_p1] * X[i*X.ncol + interaction];//Maksim: interaction with SNP;;
					}
				}


				//________________________
				int col_new=-1;
				if(is_interaction_excluded)
				{
					mematrix<double> nX_without_interact_phe;
					nX_without_interact_phe.reinit(nX.nrow,nX.ncol-1);
					for(int row=0 ; row < nX.nrow ; row++)
					{
						col_new=-1;
						for(int col=0 ; col<nX.ncol ; col++)
						{
							if(col != interaction && !iscox)
							{
								col_new++;
								nX_without_interact_phe[row*nX_without_interact_phe.ncol+col_new] = nX[row*nX.ncol+col];
							}
							if(col != interaction-1 && iscox)
							{
								col_new++;
								nX_without_interact_phe[row*nX_without_interact_phe.ncol+col_new] = nX[row*nX.ncol+col];
							}

						}
					}
					return nX_without_interact_phe;
				}//end of is_interaction_excluded
				//________________________


				return(nX);
			}
		}
		else
		{
			return(X);
		}

	}
	mematrix<double> nX;
	if(interaction != 0)
	{
		nX.reinit(X.nrow,(X.ncol));
	}
	else
	{
		nX.reinit(X.nrow,(X.ncol-1));
	}
	int c1 = X.ncol-2;
	int c2 = X.ncol-1;
	for (int i=0;i<X.nrow;i++)
		for (int j=0;j<(X.ncol-2);j++)
			nX[i*nX.ncol+j] = X[i*X.ncol+j];
	for (int i=0;i<nX.nrow;i++)
	{
		if (model==1)
			nX[i*nX.ncol+c1] = X[i*X.ncol+c1] + 2.*X[i*X.ncol+c2];
		else if (model==2)
			nX[i*nX.ncol+c1] = X[i*X.ncol+c1] + X[i*X.ncol+c2];
		else if (model==3)
			nX[i*nX.ncol+c1] = X[i*X.ncol+c2];
		else if (model==4)
			nX[i*nX.ncol+c1] = X[i*X.ncol+c1];
		if(interaction != 0)
			nX[i*nX.ncol+c2] = X[i*nX.ncol+interaction] * nX[i*nX.ncol+c1];//Maksim: interaction with SNP
	}
	//Han Chen
	int col_new=-1;
	if(is_interaction_excluded)
	{
		mematrix<double> nX_without_interact_phe;
		nX_without_interact_phe.reinit(nX.nrow,nX.ncol-1);
		for(int row=0 ; row < nX.nrow ; row++)
		{
			col_new=-1;
			for(int col=0 ; col<nX.ncol ; col++)
			{
				if(col != interaction && !iscox)
				{
					col_new++;
					nX_without_interact_phe[row*nX_without_interact_phe.ncol+col_new] = nX[row*nX.ncol+col];
				}
				if(col != interaction-1 && iscox)
				{
					col_new++;
					nX_without_interact_phe[row*nX_without_interact_phe.ncol+col_new] = nX[row*nX.ncol+col];
				}

			}
		}
		return nX_without_interact_phe;
	}//interaction_only, model!=0, ngpreds==2
	//Oct 26, 2009
	return nX;
}

mematrix<double> t_apply_model(mematrix<double> &X, int model, int interaction, int ngpreds, bool iscox, int nullmodel=0)
										{
	mematrix<double> tmpX = transpose(X);
	mematrix<double> nX = apply_model(tmpX, model, interaction, ngpreds, iscox, nullmodel);
	mematrix<double> out = transpose(nX);
	return out;
										}

class linear_reg
{
public:
	mematrix<double> beta;
	mematrix<double> sebeta;
	//Han Chen
	mematrix<double> covariance;
	//Oct 26, 2009
	mematrix<double> residuals;
	double sigma2;
	double loglik;
	double chi2_score;

	linear_reg(regdata &rdatain)
	{
		regdata rdata = rdatain.get_unmasked_data();
		//fprintf(stdout,"linear_reg: %i %i %i\n",rdata.nids,(rdata.X).ncol,(rdata.Y).ncol);
		int length_beta = (rdata.X).ncol;
		beta.reinit(length_beta,1);
		sebeta.reinit(length_beta,1);
		//Han Chen
		if (length_beta>1)
		{covariance.reinit(length_beta-1,1);}
		//Oct 26, 2009
		residuals.reinit(rdata.nids,1);
		sigma2=-1.;
		loglik=-9.999e+32;
		chi2_score=-1.;
	}
	~linear_reg()
	{
		//		delete beta;
		//		delete sebeta;
		//		delete residuals;
	}
	//	void mmscore(regdata &rdata,int verbose, double tol_chol, int model, int interaction, int ngpreds, mematrix<double> & invvarmatrix, int nullmodel=0)
	//	{
	//			int length_beta = (rdata.X).ncol;
	//			beta.reinit(length_beta,1);
	//			sebeta.reinit(length_beta,1);
	//			mematrix<double> G;
	//		  mematrix<double> Y = rdata.Y;
	//
	//			G.reinit((rdata.X).nrow,1);
	//			for (int i=0;i<(rdata.X).nrow;i++) G[i] = (rdata.X).get(i,1);
	////			double sumgt = column_sum(G).get(0,0);
	//			double meangt = G.column_mean(0);
	//			double meany = Y.column_mean(0);
	//
	//			G = G - meangt;
	//			Y = Y - meany;
	//			double U, V;
	//			mematrix<double> U_mema, V_mema;
	//
	//
	//			U_mema = transpose(G)*invvarmatrix;
	//			U = (U_mema * Y).get(0, 0);
	//
	//			V_mema = transpose(G)*invvarmatrix;
	//			V = (V_mema*G).get(0, 0);
	//
	//
	//			chi2_score = U*U/V;
	//
	//			beta.put(-100, 0,0);
	//			double bb = U/V;
	//			beta.put(bb,1,0);
	//			sebeta.put(-100.,0,0);
	//			double sebe=sqrt(1./V);
	//			sebeta.put(sebe,1,0);
	//
	//
	//
	//
	//			/*
	//			for(snp in snps)
	//					{
	//					print(snp)
	//							ns <- which(mm$snpnames==snp)
	//							X <- gt[,snp]
	//							V <- h2ht$InvSigma #*var(h2ht$residualY,na.rm=T)
	//							UU <- ((X-mean(X))%*%V%*%(Y-mean(Y)))
	//							VV <- ((X-mean(X))%*%V%*%(X-mean(X)))
	//							beta <- UU/VV
	//							se <- sqrt(1./VV)
	//							chi2 <- UU*UU/VV
	//							print(c(re$chi2.1df[ns],qt$chi2.1df[ns],gr$chi2.1df[ns],mm$chi2.1df[ns],(beta/se)^2))
	//							print(c(re$effB[ns],qt$effB[ns],gr$effB[ns],mm$effB[ns],beta))
	//					}
	//
	//			*/
	//
	//
	//
	///*
	//		  int length_beta = (rdata.X).ncol;
	//		  beta.reinit(length_beta,1);
	//		  sebeta.reinit(length_beta,1);
	//
	//			mematrix<double> G;
	//			G.reinit((rdata.X).nrow,1);
	//			for (int i=0;i<(rdata.X).nrow;i++) G[i] = (rdata.X).get(i,1);
	//			double sumgt = column_sum(G).get(0,0);
	//			double meangt = G.column_mean(0);
	//			G = G - meangt;
	//			mematrix<double> YY = rdata.Y;
	//			YY = YY - YY.column_mean(0);
	//			mematrix<double> tG = transpose(G);
	//			tG = tG*invvarmatrix;
	//			double U = (tG*YY).get(0,0);
	//			double V = (tG*G).get(0,0);
	//
	//			beta.put(-100, 0,0);
	//			double bb = (U/V)*var(YY);
	//			beta.put(bb,1,0);
	//			sebeta.put(-100.,0,0);
	//			sebeta.put(abs(bb)/sqrt(U*U/V),1,0);
	//
	//*/
	//
	//	}
	void estimate(regdata &rdatain,int verbose, double tol_chol, int model, int interaction,
			int ngpreds, mematrix<double> & invvarmatrixin, int robust, int nullmodel=0)
	{
		//suda ineraction parameter
		// model should come here
		regdata rdata = rdatain.get_unmasked_data();

		mematrix<double> invvarmatrix;
		if(invvarmatrixin.nrow!=0 && invvarmatrixin.ncol!=0)
		{
			invvarmatrix.reinit(rdata.nids,rdata.nids);
			int i1=0, j1=0;
			for (int i=0;i<rdata.nids;i++)
				if (rdatain.masked_data[i]==0) {
					j1 = 0;
					for (int j=0;j<rdata.nids;j++) if (rdatain.masked_data[j]==0) {
						invvarmatrix.put(invvarmatrixin.get(i,j),i1,j1);
						j1++;
					}
					i1++;
				}

		}

		//fprintf(stdout,"estimate: %i %i %i %i\n",rdata.nids,(rdata.X).nrow,(rdata.X).ncol,(rdata.Y).ncol);
		mematrix<double> X = apply_model(rdata.X,model, interaction, ngpreds, false, nullmodel);

		int length_beta = X.ncol;
		beta.reinit(length_beta,1);
		sebeta.reinit(length_beta,1);
		//Han Chen
		if (length_beta>1)
		{if (model==0 && interaction!=0 && ngpreds==2 && length_beta>2)
		{covariance.reinit(length_beta-2,1);}
		else
		{covariance.reinit(length_beta-1,1);}
		}
		//Oct 26, 2009
		mematrix<double> tX = transpose(X);



		if(invvarmatrix.nrow!=0 && invvarmatrix.ncol!=0)
		{
			tX = tX*invvarmatrix;
			//			X = invvarmatrix*X; std::cout<<"new tX.nrow="<<X.nrow<<" tX.ncol="<<X.ncol<<"\n";
		}


		mematrix<double> tXX = tX*X;

		//		double N = tXX.get(0,0);
		double N = X.nrow;
		if (verbose) {printf("tXX:\n");tXX.print();}
		//
		// use cholesky to invert
		//
		mematrix<double> tXX_i = tXX;
		cholesky2_mm(tXX_i,tol_chol);
		if (verbose) {printf("chole tXX:\n");tXX_i.print();}
		chinv2_mm(tXX_i);
		// before was
		// mematrix<double> tXX_i = invert(tXX);
		if (verbose) {printf("tXX-1:\n");tXX_i.print();}
		mematrix<double> tXY = tX*(rdata.Y);
		if (verbose) {printf("tXY:\n");tXY.print();}
		beta = tXX_i*tXY;
		if (verbose) {printf("beta:\n");(beta).print();}

		// now compute residual variance
		sigma2 = 0.;

		mematrix<double> ttX = transpose(tX);
		mematrix<double> sigma2_matrix = rdata.Y;
		mematrix<double> sigma2_matrix1 = ttX*beta;
		sigma2_matrix = sigma2_matrix - sigma2_matrix1;

		static double val;

		//	std::cout<<"sigma2_matrix.nrow="<<sigma2_matrix.nrow<<"sigma2_matrix.ncol"<<sigma2_matrix.ncol<<"\n";

		for(int i=0 ; i<sigma2_matrix.nrow ; i++)
		{
			val = sigma2_matrix.get(i,0);
			sigma2 += val * val;
		}



		double sigma2_internal = sigma2/(N - double(length_beta));



		// now compute residual variance
		//		sigma2 = 0.;
		//		for (int i =0;i<(rdata.Y).nrow;i++)
		//			sigma2 += ((rdata.Y).get(i,0))*((rdata.Y).get(i,0));
		//		for (int i=0;i<length_beta;i++)
		//			sigma2 -= 2. * (beta.get(i,0)) * tXY.get(i,0);
		//		for (int i=0;i<(length_beta);i++)
		//		for (int j=0;j<(length_beta);j++)
		//			sigma2 += (beta.get(i,0)) * (beta.get(j,0)) * tXX.get(i,j);

		//	std::cout<<"sigma2="<<sigma2<<"\n";
		//	std::cout<<"sigma2_internal="<<sigma2_internal<<"\n";

		//		replaced for ML
		//		sigma2_internal	= sigma2/(N - double(length_beta) - 1);
		sigma2 /= N;

		//	std::cout<<"N="<<N<<", length_beta="<<length_beta<<"\n";

		if (verbose) {printf("sigma2 = %f\n",sigma2);}

		/*
		loglik = 0.;
		double ss=0;
		for (int i=0;i<rdata.nids;i++) {
			double resid = rdata.Y[i] - beta.get(0,0); // intercept
			for (int j=1;j<beta.nrow;j++) resid -= beta.get(j,0)*X.get(i,j);
		//	residuals[i] = resid;
			ss += resid*resid;
		}
		sigma2 = ss/N;
		 */

		//cout << "estimate " << rdata.nids << "\n";
		//(rdata.X).print();
		//for (int i=0;i<rdata.nids;i++) cout << rdata.masked_data[i] << " ";
		//cout << endl;

		loglik = 0.;
		double halfrecsig2 = .5/sigma2;
		for (int i=0;i<rdata.nids;i++) {
			double resid = rdata.Y[i] - beta.get(0,0); // intercept
			for (int j=1;j<beta.nrow;j++) resid -= beta.get(j,0)*X.get(i,j);
			residuals[i] = resid;
			loglik -= halfrecsig2*resid*resid;
		}
		loglik -= double(rdata.nids)*log(sqrt(sigma2));

		//cout << "estimate " << rdata.nids << "\n";
		//
		//		ugly fix to the fact that if we do mmscore, sigma2 is already in the matrix...
		//		YSA, 2009.07.20
		//
		//cout << "estimate 0\n";

		if(invvarmatrix.nrow!=0 && invvarmatrix.ncol!=0) sigma2_internal=1.0;

		mematrix<double> robust_sigma2(X.ncol,X.ncol);
		if (robust) {
			mematrix<double> XbyR = X;
			for (int i=0;i<X.nrow;i++) for (int j=0;j<X.ncol;j++) {
				double tmpval = XbyR.get(i,j)*residuals[i];
				XbyR.put(tmpval,i,j);
			}
			XbyR = transpose(XbyR)*XbyR;
			robust_sigma2 = tXX_i*XbyR;
			robust_sigma2 = robust_sigma2*tXX_i;
		}

		//cout << "estimate 0\n";

		for (int i=0;i<(length_beta);i++)
		{
			if (robust) {
				double value = sqrt(robust_sigma2.get(i,i));
				sebeta.put(value,i,0);
				//Han Chen
				if (i>0)
				{
					if (model==0 && interaction!=0 && ngpreds==2 && length_beta>2)
					{if (i>1)
					{double covval=robust_sigma2.get(i,i-2);
					covariance.put(covval,i-2,0);}
					}
					else
					{
						double covval=robust_sigma2.get(i,i-1);
						covariance.put(covval,i-1,0);
					}
				}
				//Oct 26, 2009
			} else {
				double value = sqrt(sigma2_internal*tXX_i.get(i,i));
				sebeta.put(value,i,0);
				//Han Chen
				if (i>0)
				{
					if (model==0 && interaction!=0 && ngpreds==2 && length_beta>2)
					{if (i>1)
					{double covval=sigma2_internal*tXX_i.get(i,i-2);
					covariance.put(covval,i-2,0);}
					}
					else
					{
						double covval=sigma2_internal*tXX_i.get(i,i-1);
						covariance.put(covval,i-1,0);
					}
				}
				//Oct 26, 2009
			}
		}
		//cout << "estimate E\n";
		if (verbose) {printf("sebeta (%d):\n",sebeta.nrow);sebeta.print();}
	}

	void score(mematrix<double> &resid,regdata &rdatain,int verbose, double tol_chol, int model, int interaction, int ngpreds, mematrix<double> & invvarmatrix, int nullmodel=0)
	{
		regdata rdata = rdatain.get_unmasked_data();

		//mematrix<double> resid(rdata.nids,1);
		//int j = 0;
		//for (int i=0;i<rdatain.nids;i++) if (rdata.masked_data[i]==0) resid[j++]=residin[i];

		mematrix<double> oX = rdata.extract_genotypes();
		mematrix<double> X = apply_model(oX,model,interaction, ngpreds, false, nullmodel);
		beta.reinit(X.ncol,1);
		sebeta.reinit(X.ncol,1);
		double N = double(resid.nrow);

		mematrix<double> tX =  transpose(X);

		if(invvarmatrix.nrow!=0 && invvarmatrix.ncol!=0) tX = tX*invvarmatrix;
		//if(invvarmatrix.nrow!=0 && invvarmatrix.ncol!=0) X = invvarmatrix*X;


		mematrix<double> u = tX*resid;
		mematrix<double> v = tX*X;
		mematrix<double> csum = column_sum(X);
		csum = transpose(csum)*csum;
		csum = csum*(1./N);
		v = v-csum;
		// use cholesky to invert
		mematrix<double> v_i = v;
		cholesky2_mm(v_i,tol_chol);
		chinv2_mm(v_i);
		// before was
		// mematrix<double> v_i = invert(v);
		beta = v_i*u;
		double sr = 0.;
		double srr =0.;
		for (int i=0;i<resid.nrow;i++)
		{
			sr += resid[i];
			srr += resid[i]*resid[i];
		}
		double mean_r = sr/N;
		double sigma2_internal = (srr-N*mean_r*mean_r)/(N-beta.nrow);
		for (int i=0;i<beta.nrow;i++)
			sebeta[i] = sqrt(v_i.get(i,i)*sigma2_internal);
		//sebeta[i] = sqrt(v_i.get(i,i)*sigma2_internal);
		mematrix<double> chi2 = transpose(u)*v_i*u;
		chi2 = chi2*(1./sigma2_internal);
		chi2_score = chi2[0];
	}

};

class logistic_reg
{
public:
	mematrix<double> beta;
	mematrix<double> sebeta;
	//Han Chen
	mematrix<double> covariance;
	//Oct 26, 2009
	mematrix<double> residuals;
	double sigma2;
	double loglik;
	double chi2_score;
	int niter;

	logistic_reg(regdata &rdatain)
	{
		regdata rdata = rdatain.get_unmasked_data();
		int length_beta = (rdata.X).ncol;
		beta.reinit(length_beta,1);
		sebeta.reinit(length_beta,1);
		//Han Chen
		if (length_beta>1)
		{covariance.reinit(length_beta-1,1);}
		//Oct 26, 2009
		residuals.reinit((rdata.X).nrow,1);
		sigma2=-1.;
		loglik=-9.999e+32;
		niter=-1;
		chi2_score=-1.;
	}
	~logistic_reg()
	{
		//		delete beta;
		//		delete sebeta;
	}
	void estimate(regdata &rdatain, int verbose, int maxiter, double eps, double tol_chol, int model, int interaction, int ngpreds, mematrix<double> & invvarmatrix, int robust, int nullmodel=0)
	{
		regdata rdata = rdatain.get_unmasked_data();

		mematrix<double> X = apply_model(rdata.X,model, interaction, ngpreds, false, nullmodel);
		int length_beta = X.ncol;
		beta.reinit(length_beta,1);
		sebeta.reinit(length_beta,1);
		//Han Chen
		if (length_beta>1)
		{if (model==0 && interaction!=0 && ngpreds==2 && length_beta>2)
		{covariance.reinit(length_beta-2,1);}
		else
		{covariance.reinit(length_beta-1,1);}
		}
		//Oct 26, 2009
		mematrix<double> W((X).nrow,1);
		mematrix<double> z((X).nrow,1);
		mematrix<double> tXWX(length_beta,length_beta);
		mematrix<double> tXWX_i(length_beta,length_beta);
		mematrix<double> tXWz(length_beta,1);

		double prev = (rdata.Y).column_mean(0);
		if (prev>=1. || prev <=0.)
		{
			fprintf(stderr,"prevalence not within (0,1)\n");
			exit(1);
		}
		for (int i = 0;i<length_beta;i++) beta.put(0.,i,0);
		beta.put(log(prev/(1.-prev)),0,0);

		mematrix<double> tX = transpose(X);

		//		if(invvarmatrix.nrow!=0 && invvarmatrix.ncol!=0) tX = X*invvarmatrix;



		double N;

		niter=0;
		double delta=1.;
		double prevlik=0.;
		while (niter<maxiter && delta>eps)
		{
			mematrix<double> eMu = (X)*beta;
			mematrix<double> eMu_us = eMu;
			for (int i=0;i<eMu.nrow;i++)
			{
				double emu = eMu.get(i,0);
				double value = emu;
				double zval = 0.;
				value = exp(value)/(1.+exp(value));
				residuals[i] = (rdata.Y).get(i,0)-value;
				eMu.put(value,i,0);
				W.put(value*(1.-value),i,0);
				zval = emu + (1./(value*(1.-value)))*(((rdata.Y).get(i,0))-value);
				z.put(zval,i,0);
			}

			mematrix<double> tmp = productMatrDiag(tX,W);
			if (verbose) {printf("tXW:\n");tmp.print();}
			mematrix<double> tXWX = tmp*(X);
			N = tXWX.get(0,0);

			if (verbose) {printf("tXWX:\n");tXWX.print();}
			//
			// use cholesky to invert
			//
			tXWX_i = tXWX;
			cholesky2_mm(tXWX_i,tol_chol);
			if (verbose) {printf("chole tXWX:\n");tXWX_i.print();}
			chinv2_mm(tXWX_i);
			// was before
			// tXWX_i=invert(tXWX);
			if (verbose) {printf("tXWX-1:\n");tXWX_i.print();}
			mematrix<double> tmp1 = productMatrDiag(tX,W);
			mematrix<double> tXWz = tmp1*z;
			if (verbose) {printf("tXWz:\n");tXWz.print();}
			beta = tXWX_i*tXWz;
			if (verbose) {printf("beta:\n");beta.print();}
			// compute likelihood
			prevlik = loglik;
			loglik=0.;
			for (int i=0;i<eMu.nrow;i++)
				loglik += rdata.Y[i]*eMu_us[i]-log(1.+exp(eMu_us[i]));
			delta = fabs(1.-(prevlik/loglik));
			niter++;
		}
		sigma2 = 0.;

		mematrix<double> robust_sigma2(X.ncol,X.ncol);
		if (robust) {
			mematrix<double> XbyR = X;
			for (int i=0;i<X.nrow;i++) for (int j=0;j<X.ncol;j++) {
				double tmpval = XbyR.get(i,j)*residuals[i];
				XbyR.put(tmpval,i,j);
			}
			XbyR = transpose(XbyR)*XbyR;
			robust_sigma2 = tXWX_i*XbyR;
			robust_sigma2 = robust_sigma2*tXWX_i;
		}

		for (int i=0;i<(length_beta);i++)
		{
			if (robust)
			{
				double value = sqrt(robust_sigma2.get(i,i));
				sebeta.put(value,i,0);
				//Han Chen
				if (i>0)
				{
					if (model==0 && interaction!=0 && ngpreds==2 && length_beta>2)
					{if (i>1)
					{double covval=robust_sigma2.get(i,i-2);
					covariance.put(covval,i-2,0);}
					}
					else
					{
						double covval=robust_sigma2.get(i,i-1);
						covariance.put(covval,i-1,0);
					}
				}
				//Oct 26, 2009
			}
			else {
				double value = sqrt(tXWX_i.get(i,i));
				sebeta.put(value,i,0);
				//Han Chen
				if (i>0)
				{
					if (model==0 && interaction!=0 && ngpreds==2 && length_beta>2)
					{if (i>1)
					{double covval=tXWX_i.get(i,i-2);
					covariance.put(covval,i-2,0);}
					}
					else
					{
						double covval=tXWX_i.get(i,i-1);
						covariance.put(covval,i-1,0);
					}
				}
				//Oct 26, 2009
			}
		}
		if (verbose) {printf("sebeta (%d):\n",sebeta.nrow);sebeta.print();}
	}
	// just a stupid copy from linear_reg
	void score(mematrix<double> &resid,regdata &rdata,int verbose, double tol_chol, int model, int interaction, int ngpreds, mematrix<double> & invvarmatrix, int nullmodel=0)
	{
		mematrix<double> oX = rdata.extract_genotypes();
		mematrix<double> X = apply_model(oX,model, interaction, ngpreds, false, nullmodel);
		beta.reinit(X.ncol,1);
		sebeta.reinit(X.ncol,1);
		double N = double(resid.nrow);

		mematrix<double> tX=transpose(X);
		if(invvarmatrix.nrow!=0 && invvarmatrix.ncol!=0) tX = tX*invvarmatrix;

		mematrix<double> u = tX*resid;
		mematrix<double> v = tX*X;
		mematrix<double> csum = column_sum(X);
		csum = transpose(csum)*csum;
		csum = csum*(1./N);
		v = v-csum;
		// use cholesky to invert
		mematrix<double> v_i = v;
		cholesky2_mm(v_i,tol_chol);
		chinv2_mm(v_i);
		// before was
		// mematrix<double> v_i = invert(v);
		beta = v_i*u;
		double sr = 0.;
		double srr =0.;
		for (int i=0;i<resid.nrow;i++)
		{
			sr += resid[i];
			srr += resid[i]*resid[i];
		}
		double mean_r = sr/N;
		double sigma2_internal = (srr-N*mean_r*mean_r)/(N-beta.nrow);
		for (int i=0;i<beta.nrow;i++)
			sebeta[i] = sqrt(v_i.get(i,i)*sigma2_internal);
		mematrix<double> chi2 = transpose(u)*v_i*u;
		chi2 = chi2*(1./sigma2_internal);
		chi2_score = chi2[0];
	}
};


class coxph_reg
{
public:
	mematrix<double> beta;
	mematrix<double> sebeta;
	mematrix<double> residuals;
	double sigma2;
	double loglik;
	double chi2_score;
	int niter;

	coxph_reg(coxph_data &cdatain)
	{
		coxph_data cdata = cdatain.get_unmasked_data();
		beta.reinit(cdata.X.nrow,1);
		sebeta.reinit(cdata.X.nrow,1);
		loglik=-9.999e+32;
		sigma2=-1.;
		chi2_score=-1.;
		niter=0;
	}
	~coxph_reg()
	{
		//		delete beta;
		//		delete sebeta;
	}
	void estimate(coxph_data &cdatain, int verbose, int maxiter, double eps, double tol_chol, int model, int interaction, int ngpreds, bool iscox, int nullmodel=0)
	{
		//		cout << "model = " << model << "\n";
		//		cdata.X.print();
		coxph_data cdata = cdatain.get_unmasked_data();
		mematrix<double> X = t_apply_model(cdata.X,model, interaction, ngpreds, iscox, nullmodel);
		//		X.print();
		int length_beta = X.nrow;
		beta.reinit(length_beta,1);
		sebeta.reinit(length_beta,1);
		mematrix<double> newoffset = cdata.offset;
		newoffset = cdata.offset - (cdata.offset).column_mean(0);
		mematrix<double> means(X.nrow,1);
		for (int i=0;i<X.nrow;i++) beta[i]=0.;
		mematrix<double> u(X.nrow,1);
		mematrix<double> imat(X.nrow,X.nrow);
		double work[X.ncol*2+2*(X.nrow)*(X.nrow)+3*(X.nrow)];
		double loglik_int[2];
		int flag;
		double sctest=1.0;

		coxfit2(&maxiter,&cdata.nids,&X.nrow,
				cdata.stime.data,cdata.sstat.data,X.data,
				newoffset.data,cdata.weights.data,cdata.strata.data,
				means.data,beta.data,u.data,
				imat.data,loglik_int,&flag,
				work,&eps,&tol_chol,
				&sctest);
		for (int i=0;i<X.nrow;i++) sebeta[i]=sqrt(imat.get(i,i));
		loglik = loglik_int[1];
		niter = maxiter;
	}
};
