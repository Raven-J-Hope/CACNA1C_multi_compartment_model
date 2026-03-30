: Ca-dependent K channels (BK) - alphabeta4 and alpha
: Bin Wang, Robert Brenner, and David Jaffe - Originally written May 27, 2010
: 
: June 1, 2010 - added double exponential function for voltage-dependent activation 
:
: July 3, 2010 - changed voltage-dependence for the two channels based on revised data
:
: April 2, 2011 - adjusted parameters based on updated Bin data

: Mar 2016 - added instant N-type Calcium concentration (Beining et al (2016), "A novel comprehensive and consistent electrophysiologcal model of dentate granule cells")

: Mar 2026 - made cav21 spefics

: Ca-dependent K channels (BK) driven specifically by Cav2.1 current

NEURON {
	SUFFIX BK_Cav21
	USEION k READ ek WRITE ik
	USEION pca READ ipca VALENCE 0
	RANGE gakbar, gabkbar, gak, gabk, atau, ainf, a, ab, abinf, abtau, ik, diff, acai
	GLOBAL base, ca0, tau, B
}

UNITS {
	(molar) = (1/liter)
	(mM) = (millimolar)
	(mV) = (millivolt)
	(mA) = (milliamp)
	(S) = (siemens)
}

PARAMETER {
	diff = 1 (1)
	gakbar = .01	(S/cm2)
	gabkbar = .01	(S/cm2)
	base = 4  	(mV)

	ca0 = 0.00007 (mM)
	tau = 9 (ms)
	B = 0.26 (mM-cm2/mA-ms)
}

ASSIGNED {
	v		(mV)
	ek		(mV)

	ik		(mA/cm2)
	ipca		(mA/cm2)

	gak		(S/cm2)
	gabk		(S/cm2)
	ainf
	atau		(ms)
	abinf
	abtau		(ms)
	acai		(mM)
}

STATE {ab a ca_i}

BREAKPOINT {
	SOLVE state METHOD cnexp
	gak = gakbar*a
	gabk = gabkbar*ab
	ik = (gabk+gak)*(v - ek)
}

DERIVATIVE state {
	ca_i' = -B * ipca - (ca_i - ca0) / tau

	acai = ca_i / diff
	if (acai < ca0) {
		acai = ca0
	}

	rates(v, acai)
	a' = (ainf-a)/atau
	ab' = (abinf-ab)/abtau
}

INITIAL {
	ca_i = ca0
	acai = ca_i / diff
	rates(v, acai)
	a = ainf
	ab = abinf
}

FUNCTION shifta(ca (mM))  {
	shifta = 25 - 50.3 + (107.5*exp(-.12*ca*1e3))
}

FUNCTION peaka(ca (mM))  {
	peaka = 2.9 + (6.3*exp(-.36*ca*1e3))
}

FUNCTION shiftab(ca (mM))  {
	shiftab = 25 - 55.7 + 136.9*exp(-.28*ca*1e3)
}

FUNCTION peakab(ca (mM))  {
	peakab = 13.7 + 234*exp(-.72*ca*1e3)
}

FUNCTION taufunc(v (mV)) {
	 taufunc = 1 / ( (10*(exp(-v/63.6) + exp (-(150-v)/63.6))) - 5.2 )
	 if (taufunc <= 0.2) {
	    taufunc = 0.2
	 }
}

PROCEDURE rates(v (mV), c (mM)) {
	LOCAL range, vv, ashift, bshift

	ashift = -32 + (59.2*exp(-.09*c*1e3)) + (96.7*exp(-.47*c*1e3))
	ainf = 1/(1+exp((ashift-v)/(25/1.6)))

	vv = v + 100 - shifta(c)
	atau = taufunc(vv)
	range = peaka(c)-1
	atau = (range*((atau-.2)/.8)) + 1

	bshift = -56.449 + 104.52*exp(-.22964*c*1e3) + 295.68*exp(-2.1571*c*1e3)
	abinf = 1/(1+exp((bshift-v)/(25/1.6)))

	vv = v + 100 - shiftab(c)
	abtau = taufunc(vv)
	range = peakab(c)-base
	abtau = (range*((abtau-.2)/.8)) + base
}
