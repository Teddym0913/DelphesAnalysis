#!/bin/bash

target=$1

pthiggs=100
met=20
mtata=90
mjj=800
detajj=38
sigmamax=0

while [ $pthiggs -le 200 ]
  do
  	while [ $met -le 100 ]
  	  do
  	  	while [ $mtata -le 130 ]
  	  	  do
  	  	  	while [ $mjj -le 2000 ]
  	  	  	  do
  	  	  	  	while [ $detajj -le 52 ]
  	  	  	  	  do
  	  	  	  	  	detajjtrue=$(echo "$detajj/10" | bc -l)
  	  	  	  	  	sed -e "s/__pthiggs__/"$pthiggs"/" -e "s/__met__/"$met"/" -e "s/__mtata__/"$mtata"/" -e "s/__mjj__/"$mjj"/" -e "s/__detajj__/"$detajjtrue"/" ./config/Cuts_card_VBF_highPT_temp.dat > ./config/Cuts_card_VBF_highPT.dat
  	  	  	  	  	./Delphes_Analysis 1 input_config_Advanced $target
  	  	  	  	  	signalcs=$(awk '$1=="hjj_vbf_CP0x785" {print $3}' $target/VBF_HighPt_Af_Advanced.dat)
  	  	  	  	  	bkgcs=$(awk '$1=="zjj_vbf" {print $3}' $target/VBF_HighPt_Af_Advanced.dat)
  	  	  	  	    sigEve=$(echo "$signalcs*300" | bc -l)
  	  	  	  	    bkgEve=$(echo "$bkgcs*300" | bc -l)
  	  	  	  	    good=$(echo "$sigEve>3&&$bkgEve>3" | bc -l)
  	  	  	  	    sigma=0
  	  	  	  	    if [[ $good -eq 1 ]]; then
  	  	  	  	    	sigma=$(echo "$sigEve/sqrt($bkgEve)" | bc -l)
  	  	  	  	    fi
  	  	  	  	    echo "$pthiggs  $met  $mtata  $mjj  $detajjtrue  $signalcs  $bkgcs  $sigma" >> $target/Scan_Result.dat
  	  	  	  	    max=$(echo "$sigma>$sigmamax" | bc -l)
                    if [ $max -eq 1 ]
                      then
                        sigmamax=$sigma
                        pthiggs0=$pthiggs
                        met0=$met
                        mtata0=$mtata
                        mjj0=$mjj
                        detajjtrue0=$detajjtrue
                        signalcs0=$signalcs
                        bkgcs0=$bkgcs
                        echo "$sigmamax\r"
                    fi
                    detajj=$[$detajj+2]
  	  	  	  	  done
  	  	  	  	detajj=38  
  	  	  	  	mjj=$[$mjj+200]
  	  	  	  done
  	  	  	mjj=800
  	  	  	mtata=$[$mtata+5]
  	  	  done
  	  	mtata=90
  	  	met=$[$met+20]
  	  done
  	met=20
  	pthiggs=$[$pthiggs+20]
  done

  echo "$pthiggs0  $met0  $mtata0  $mjj0  $detajjtrue0  $signalcs0  $bkgcs0  $sigmamax" >> $target/Scan_Result.dat